#include "utils.hpp"
#include <type_traits>

template <typename T>
constexpr int Enum<T>::toInt(const T &value)
{
    static_assert(std::is_enum<T>::value,
                "Template argument must be an enum class type");
    return static_cast<int>(value);
}

template <typename T>
constexpr bool Enum<T>::toBool(const T &value)
{
    static_assert(std::is_enum<Try>::value || std::is_enum<Result>::value,
                "Template argument must be an Try or Result class");
    if (static_cast<int>(value) == -1)
        return false;
    else
        return true;
}