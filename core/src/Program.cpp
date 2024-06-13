#include "Program.hpp"

Program::Program() : _password("password1@") {}

Program::~Program() {}

/*
 * 비밀번호 일치 여부를 확인하는 함수
 */
bool Program::equalPassword(const char *str) const noexcept
{
    if (_password == std::string(str))
        return true;
    else
        return false;
}

/*
 * 비밀번호의 형식이 맞는지 확인하는 함수
 * 8자리 이상, 숫자 1개 이상 포함, 특수문자 1개 이상 포함시 성공
 */
bool Program::validatePassword(const char *str) const noexcept
{
    std::string validate(str);
    // 자리수 체크
    if (validate.length() < 8) {
        return false;
    }

    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (char c : validate) {
        // 숫자 유무 체크
        if (isdigit(c)) {
            hasDigit = true;
        }
        // 특수문자 유무 체크
        if (std::ispunct(c)) {
            hasSpecialChar = true;
        }
    }
    return hasDigit && hasSpecialChar;
}

/*
 * 비밀번호를 변경하는 함수
 * 조건을 만족하면 변경사항 적용
 */
void Program::changePassword(const char *str)
{
    if (!validatePassword(str))
        throw std::logic_error("비밀번호 형식 틀림");
    _password = str;
}

VendingMachine& Program::getMachine(void) noexcept
{
    return _machine;
}

Client& Program::getClient(void) noexcept
{
    return _client;
}

std::string Program::getPassword(void) const noexcept
{
    return _password;
}

void Program::parse(void)
{
    std::string buf = getClient().getFromRead();

    if (buf.empty())
        return ;
    splitByNewLine(buf);
    while (!_lines.empty()) {
        std::string line = _lines.front();
        _lines.pop();
        std::cout << "From Server: " << line << '$' << '\n';
        parseEachLine(line);
    }
}

void Program::splitByNewLine(const std::string &buf)
{
    std::istringstream stream(buf);
    std::string line;

    while (std::getline(stream, line))
        _lines.push(line);
}

void Program::parseEachLine(std::string &line)
{
    DebugLog::AddLog("From Server: %s", line.c_str());
    switch (getCommand(line))
    {
    case CMD::WELCOME: // 서버 연결 성공
        parseWELCOME(line);
        break;
    case CMD::UPDATE: // 정보 업데이트
        parseUPDATE(line);
        break;
    case CMD::PRINT: // 자판기 정보 전달
        parsePRINT(line);
        break;
    default: // 올바르지 않은 명령어의 경우
        std::cout << ">> 잘못된 명령어 입력" << '\n';
        break;
    }
}

void Program::parseWELCOME(std::string &line)
{
    VendingMachine &machine = getMachine();
    int id = getInt(line);

    /* Need Edit */
    (void)id;

    std::string msg = "SET ";
    for (int i = 0; i < 6; i++) {
        Shelf &rack = machine.getRack(i);
        msg = concatenate(msg,
                        rack.getLabel(), ' ',
                        rack.getPrice(), ' ',
                        rack.getAmount(), ' ',
                        0, ' ');
    }
    msg = concatenate(msg, '\n');
    getClient().addToWrite(msg);
}

void Program::parseUPDATE(std::string &line)
{
    int id = getInt(line);
    Shelf &rack = _machine.getRack(id);

    try
    {
        std::string type = getWord(line);
        if (type == "LABEL") {
            rack.setLabel(getWord(line));
        }
        else if (type == "PRICE") {
            rack.setPrice(getInt(line));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Program::parsePRINT(std::string &line)
{
    /* Need Edit */
}