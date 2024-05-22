#ifndef UTILS_HPP
#define UTILS_HPP

#include <type_traits> /* static_assert */
#include "imgui.h"

#define DUMMY (void *)0

enum class Try {
    CRITICAL = -2,
    FAILURE = -1,
    SUCCESS = 0
};

enum class Result {
    ERROR = -1,
    OK = 0
};

enum class ViewMode {
    SALES,
    ADMIN
};

// enum class Currency {
//     WON_10 = 10,
//     WON_50 = 50,
//     WON_100 = 100,
//     WON_500 = 500,
//     WON_1000 = 1000
// };

template <typename T>
class Enum
{
public:
    static constexpr int toInt(const T &value)
    {
        static_assert(std::is_enum<T>::value,
                      "Template argument must be an enum class type");
        return static_cast<int>(value);
    }
    static constexpr bool toBool(const T &value)
    {
        static_assert(std::is_enum<Try>::value || std::is_enum<Result>::value,
                      "Template argument must be an Try or Result class");
        if (static_cast<int>(value) == -1)
            return false;
        else
            return true;
    }
};

ImVec2 getVec2(const ImVec2 &ratio);
#endif
