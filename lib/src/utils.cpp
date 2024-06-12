#include "utils.hpp"

/*
 * 현재 프로그램 창의 크기와 매개변수로 압력받은 비율을 곱한다
 * 계산 결과를 바탕으로 벡터를 반환
 */
ImVec2 getVec2(const ImVec2 &ratio)
{
    ImVec2 display = ImGui::GetIO().DisplaySize;
    int x = static_cast<int>(display.x * ratio.x);
    int y = static_cast<int>(display.y * ratio.y);
    return ImVec2(x, y);
}

/*
 * 문자열이 기존 명령어 리스트에 있는지 검사
 * 일치시 인덱스에 해당하는 enum class 반환
 * 불일치시 NOMATCH 반환
 */
CMD parseFlow(const std::string &str)
{
    for (int i = 0; i < static_cast<int>(CMD::_COUNT); i++)
        if (str == g_cmd_list[i])
            return static_cast<CMD>(i);
    return CMD::NOMATCH;
}