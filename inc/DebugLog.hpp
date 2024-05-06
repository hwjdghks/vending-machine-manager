#ifndef DEBUGLOG_HPP
#define DEBUGLOG_HPP

#include <utility>
#include "imgui.h"
#include "utils.hpp"

class DebugLog
{
private:
    ImGuiTextBuffer Buf;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool AutoScroll;           // Keep scrolling if already at the bottom.

public:
    DebugLog();

    template <typename... Args>
    void AddLog(const char *fmt, Args &&...args)
    {
        int old_size = Buf.size();
        Buf.append(fmt, std::forward<Args>(args)...);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }
    void Draw(const char *title);
    void Clear();
};


#endif
