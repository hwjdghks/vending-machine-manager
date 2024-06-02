#include "Program.hpp"

Program::Program() : _password("password1@") {}

Program::~Program() {}

bool Program::equalPassword(const char *str) const noexcept
{
    if (_password == std::string(str))
        return true;
    else
        return false;
}

bool Program::validatePassword(const char *str) const noexcept
{
    std::string validate(str);

    if (validate.length() < 8) {
        return false;
    }

    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (char c : validate) {
        if (isdigit(c)) {
            hasDigit = true;
        }
        if (std::ispunct(c)) {
            hasSpecialChar = true;
        }
    }
    return hasDigit && hasSpecialChar;
}

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
