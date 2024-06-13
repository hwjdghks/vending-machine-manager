#ifndef DEBUGLOG_HPP
#define DEBUGLOG_HPP

#include <utility>
#include "imgui.h"
#include "utils.hpp"

/*
 * 로그 출력 화면용 클래스
 * ImGui 예제에서 수정하여 사용
 */
class DebugLog
{
private:
    static ImGuiTextBuffer Buf;
    static ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.

public:
    DebugLog();

    static void AddLog(const char* fmt, ...) IM_FMTARGS(1);
    static void Draw(const char *title);
    static void Clear();
};

#endif
