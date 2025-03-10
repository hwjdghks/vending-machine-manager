#include "DebugLog.hpp"

ImGuiTextBuffer DebugLog::Buf;
ImVector<int> DebugLog::LineOffsets;

/*
 * ImGui 기본 함수를 약간 변경함.
 * 로그를 추가하는 함수
 */
void DebugLog::AddLog(const char* fmt, ...)
{
    Buf.append("\n");
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size + 1);
}

/*
 * ImGui 기본 함수를 약간 변경함.
 * 기록된 로그를 그리는 함수
 */
void DebugLog::Draw(const char *title)
{
    ImVec2 sizeRatio(0.30f, 1.0f);
    ImVec2 posRatio(0.70f, 0.0f);
    // 창 기본 셋팅
    ImGui::Begin(title, nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize(getVec2(sizeRatio));
    ImGui::SetWindowPos(getVec2(posRatio));
    // Main window
    bool clear = ImGui::Button("Clear");
    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (clear)
            Clear();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char *buf = Buf.begin();
        const char *buf_end = Buf.end();
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step())
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char *line_start = buf + LineOffsets[line_no];
                const char *line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
        ImGui::PopStyleVar();
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}

/*
 * 로그를 초기화하는 함수
 */
void DebugLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}
