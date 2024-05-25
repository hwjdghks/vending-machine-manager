#ifndef UTILS_HPP
#define UTILS_HPP

#include <type_traits> /* static_assert */
#include "imgui.h"

#define DUMMY (void *)0
#define BASIC_BUTTON_SIZE ImVec2(170, 60)

enum class ViewMode {
    SALES,
    ADMIN
};

ImVec2 getVec2(const ImVec2 &ratio);
#endif
