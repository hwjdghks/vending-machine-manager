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
