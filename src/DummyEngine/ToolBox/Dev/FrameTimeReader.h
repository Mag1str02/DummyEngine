#pragma once

#include "Addition/Base.h"

namespace DE
{
    class FrameTimeReader
    {
    private:
        struct Node
        {
            std::chrono::_V2::system_clock::time_point begin_time;
            std::chrono::duration<double> delta_time;

            std::string name;
            size_t current_child_id;
            Node* parent;

            std::vector<Node*> children;
        };

        Node* m_Root;
        Node* m_Current;

        FrameTimeReader();
        void PrintFrameTimes(Node* node, std::string tabulation);

    public:
        void EnterScope(std::string area_name = "");
        void LeaveScope();

        void PrintFrame();
        static FrameTimeReader& Get();
    };

#if DE_ENABLE_FRAME_TIME_READER
#define DE_FTR_PRINT() FrameTimeReader::Get().PrintFrame()
#define DE_PROFILE_SCOPE(name, a)           \
    FrameTimeReader::Get().EnterScope(name); \
    a;                                      \
    FrameTimeReader::Get().LeaveScope()
#else
#define DE_PROFILE_SCOPE(name, a) a
#define DE_FTR_PRINT()
#endif

}  // namespace DE