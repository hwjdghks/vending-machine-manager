#include "Server.hpp"

Server::Server() : _mode(0) {}

Server::~Server() {}

void Server::init(void)
{
    int dummy = 1;

    Socket::init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 주소와 포트 재사용 옵션 추가
    // 서버 종료후 바로 재실행할 경우 기다려야하는 현상 방지
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &dummy, sizeof(dummy)) == -1)
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}

/*
 * 멀티 스레드 호출 함수
 * 클라이언트 연결 요청, 수신, 발신 함수를 각각의 스레드로 동작하게 함
 */
void Server::run(void)
{
    std::thread acceptThread(&Server::acceptLoop, std::ref(*this));
    std::thread recvThread(&Server::recvLoop, std::ref(*this));
    std::thread sendThread(&Server::sendLoop, std::ref(*this));
    std::thread inputThread(&Server::inputLoop, std::ref(*this));

    acceptThread.join();
    recvThread.join();
    sendThread.join();
    inputThread.join();
}

/*
 * 클라이언트 연결 수락 함수
 */
int Server::acceptClient(void)
{
    int client_fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    errno = 0;
    // 클라이언트 연결 수락
    client_fd = accept(_fd, reinterpret_cast<sockaddr *>(&addr), &len);
    if (client_fd == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
        throw std::runtime_error("accept() failed.");
    else if (client_fd == -1) // 재시도해야 할 경우 error throw 없이 종료
        return -1;
    // 논블록 옵션 설정
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(client_fd);
        throw std::runtime_error("fcntl() failed.");
    }
    /* 클라이언트 목록에 추가 */
    /* 연결 성공 메세지 버퍼에 추가(welcome) */
    addClient(client_fd);
    return client_fd;
}

void Server::addClient(int fd)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    _clients.insertNode(Socket(fd));
}

Socket &Server::getClient(int fd)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    MyTreeNode<Socket> *node = _clients.findNode(fd);
    if (node == nullptr)
        throw std::runtime_error("Client not found");
    return node->_data;
}

void Server::delClient(Socket &client)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    try
    {
        int fd = client.getFD();
        _clients.deleteNode(client);
        close(fd);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error closing fd " << client.getFD() << ": " << e.what() << '\n';
    }
}

void Server::addInfo(int id)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    _infos.insertNode(Info(id));
}

Info &Server::getInfo(int id)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    MyTreeNode<Info> *node = _infos.findNode(id);
    if (node == nullptr)
        throw std::runtime_error("Info Data not found");
    return node->_data;
}

void Server::delInfo(int id)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    try
    {
        _infos.deleteNode(Info(id));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
/*
 * 멀티 스레드용 accept 루프 함수
 */
void Server::acceptLoop(void)
{
    std::cout << "accpet threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 0) {
                try
                {
                    int fd = acceptClient();
                    if (fd != -1) {
                        std::cout << "accept success fd: " << fd << '\n';
                        addInfo(fd);
                        Socket &client = getClient(fd);
                        // 연결됐음을 알리는 WELCOME MESSAGE 전송
                        std::string msg = concatenate("WELCOME", ' ', fd, '\n');
                        client.addToWrite(msg);
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "accept Loop(): " << e.what() << '\n';
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 1;
            }
        }
        std::this_thread::yield();
    }
}

/*
 * 멀티 스레드용 recv 루프 함수
 */
void Server::recvLoop(void)
{
    std::cout << "recv threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 1) {
                for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext();) {
                    Socket &now = it.next();
                    try
                    {
                        now.recvMsg();
                        parse(now);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "recv Loop(): " << e.what() << '\n';
                        delInfo(now.getFD());
                        delClient(now);
                        break ; // 데이터가 변경되면 반복자가 무효화되므로 반복문 탈출
                    }
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 2;
            }
        }
        std::this_thread::yield();
    }
}

/*
 * 멀티 스레드용 send 루프 함수
 */
void Server::sendLoop(void)
{
    std::cout << "send threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 2) {
                for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext();) {
                    Socket &now = it.next();
                    try
                    {
                        now.sendMsg();
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "send Loop(): " << e.what() << '\n';
                        delInfo(now.getFD());
                        delClient(now);
                        break ; // 데이터가 변경되면 반복자가 무효화되므로 반복문 탈출
                    }
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 0;
            }
        }
        std::this_thread::yield();
    }
}
/*
 * 멀티 스레드용 Standard Input 루프 함수
 * 키보드로부터 명령어를 입력받아 자체적으로 처리하거나 클라이언트로 전송한다
 */
void Server::inputLoop(void)
{
    std::string line;

    while (true) {
        try
        {
            line = "";
            // 키보드에서 개행 단위로 입력 받음
            std::getline(std::cin, line);
            // 개행 추가
            line = concatenate(line, '\n');
            int id = getInt(line);
            // PRINT 입력의 경우 저장된 정보를 출력
            if (line == "PRINT") {
                // 연결된 클라이언트의 저장된 정보를 출력
                Info &info = getInfo(id);
                std::cout << info;

                // 동작이 끝났으므로 루프의 처음으로 되돌아감
                continue;
            }
            // 그 외의 경우 id에 해당하는 클라이언트를 찾아 입력한 명령을 버퍼에 저장
            // 클라이언트 탐색
            Socket &client = getClient(id);
            // 터미널에 로그 출력
            std::cout << "cmd: " << line << '\n';
            // 클라이언트 버퍼에 명령어 저장
            client.addToWrite(line);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

/*
 * 입력받은 문자열을 파싱하여 명령어에 따른 동작을 수행하는 함수
 */
void Server::parse(Socket &client)
{
    try
    {
        // 클라이언트에 맞는 정보 가져오기
        Info &info = getInfo(client.getFD());
        // 버퍼를 개행 단위로 잘라 큐에 저장
        splitByNewLine(client.getFromRead());
        // 큐에 저장된 문자열이 있을 경우 하나씩 처리
        while (!_lines.empty()) {
            std::string line = _lines.front();
            _lines.pop();
            // 어떤 문자열인지 로그 출력
            std::cout << "fd " << client.getFD() << "=> " << std::flush;
            // 한 문장 단위로 처리
            parseEachLine(info, line);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

/*
 * 문장 단위로 명령어를 나누어 처리
 */
void Server::parseEachLine(Info &info, std::string &line)
{
    std::cout << line << '$' << '\n';
    if (line[0] == ':') {
        int id = getPrefix(line);
        std::cout << ":prefix" << line << '$' << '\n';
    }
    switch (getCommand(line))
    {
    case CMD::SELL: // 물건 판매된 경우
        parseSELL(info, line);
        break;
    case CMD::UPDATE: // 정보 업데이트
        break;
        parseUPDATE(info, line);
    case CMD::ALERT: // 여러 경고 알람
        parseALERT(info, line);
        break;
    case CMD::SET: // 정보 초기화
        parseSET(info, line);
        break;
    default: // 올바르지 않은 명령어의 경우
        std::cout << ">> 잘못된 명령어 입력" << '\n';
        break;
    }
}

void Server::splitByNewLine(const std::string &buf)
{
    std::istringstream stream(buf);
    std::string line;

    while (std::getline(stream, line))
        _lines.push(line);
}

void Server::parseSELL(Info &info, std::string &line)
{
    int id = getInt(line); // 판매한 상품의 슬롯 ID
    info._stock[id]--;
    info._salesTotal[id] += info._price[id];
}

void Server::parseUPDATE(Info &info, std::string &line)
{
    int id = getInt(line); // 슬롯 ID
    std::string type = getWord(line); // 업데이트할 요소

    if (type == "LABEL") // 상품명 업데이트
        info._label[id] = getWord(line);
    else if (type == "PRICE") // 판매 가격 업데이트
        info._price[id] = getInt(line);
    else if (type == "STOCK") // 상품 재고 업데이트
        info._stock[id] = getInt(line);
}

void Server::parseALERT(Info &info, std::string &line)
{
    std::string type = getWord(line); // 어떤 종류의 알람인지
    int data = getInt(line);
    if (type == "WARN") { // 재고 부족 경고
        std::cout << "ID: " << data << "번 슬롯 재고 보충이 필요합니다." << '\n';
    }
    else if (type == "STOCK") { // 재고 없음 알람
        std::cout << "ID: " << data << "번 슬롯 재고 없음!!" << '\n';
    }
    else if (type == "BALANCE") { // 거스름돈 부족 알람
        std::cout << data << "원 거스름돈 개수 부족!!" << '\n';
    }
}

void Server::parseSET(Info &info, std::string &line)
{
    for (int i = 0; i < 6; i++) {
        info._label[i] = getWord(line);
        info._price[i] = getInt(line);
        info._stock[i] = getInt(line);
        info._salesTotal[i] = getInt(line);
    }
    std::cout << info;
}
