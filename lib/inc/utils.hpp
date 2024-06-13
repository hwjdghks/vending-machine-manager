#ifndef UTILS_HPP
#define UTILS_HPP

#include "imgui.h"
#include <type_traits> /* static_assert */
#include <string>
#include <sstream>

#define BASIC_BUTTON_SIZE ImVec2(170, 60)

/*
 * 프로그램 창 그리기 모드
 */
enum class ViewMode {
    SALES,  // 판매
    ADMIN   // 관리자
};

/*
 * 서버와 클라이언트에서 사용되는 명령어 모음
 * 화살표 시작: sender
 * 화살표 끝: receiver
 */
enum class CMD {
    WELCOME, // server --> client
    BYE,    // server <--  client
    UPDATE, // server <--> client
    SELL,   // server <--  client
    PRINT,  // server  --> client or server internal
    SET,    // server <--  client
    ALERT,  // server <--  client
    _COUNT, // count of cmd list
    NOMATCH // error
};

/*
 * CMD에 해당하는 명령어 문자열 모음
 */
const std::string g_cmd_list[static_cast<int>(CMD::_COUNT)] = {
    "WELCOME",
    "BYE",
    "UPDATE",
    "SELL",
    "PRINT",
    "SET",
    "ALERT"
};

template <typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

inline std::string concatenate()
{
    return "";
}

// Recursive variadic template function
template <typename First, typename... Rest>
inline std::string concatenate(const First& first, const Rest&... rest)
{
    return toString(first) + concatenate(rest...);
}

ImVec2 getVec2(const ImVec2 &ratio);
CMD parseFlow(const std::string &str);
std::string getWord(std::string &str);
int getInt(std::string &str);
int getPrefix(std::string &str);
CMD getCommand(std::string &str);

#endif
