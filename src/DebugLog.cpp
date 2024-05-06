#include "DebugLog.hpp"

DebugLog::DebugLog() : AutoScroll(true)
{
    Clear();
}

void DebugLog::Draw(const char *title)
{
    ImVec2 sizeRatio(0.25f, 1.0f);
    ImVec2 posRatio(0.75f, 0.0f);

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
        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}

void DebugLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}
