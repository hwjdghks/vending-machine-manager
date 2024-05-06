#include "utils.hpp"
#include <type_traits>

ImVec2 getVec2(const ImVec2 &ratio)
{
    ImVec2 display = ImGui::GetIO().DisplaySize;
    int x = static_cast<int>(display.x * ratio.x);
    int y = static_cast<int>(display.y * ratio.y);
    return ImVec2(x, y);
}