#pragma once

#include "DummyEngine/Addition/Base.h"

namespace DE {
class FrameTimeReader {
private:
    struct Node {
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
    void EnterArea(std::string area_name = "");
    void LeaveArea();

    void PrintFrame();
    static FrameTimeReader& Get();
};

#if DE_ENABLED_FRAME_TIME_READER
#define DE_FTR_ENTER(a) FrameTimeReader::Get().EnterArea(a)
#define DE_FTR_LEAVE() FrameTimeReader::Get().LeaveArea()
#define DE_FTR_PRINT() FrameTimeReader::Get().PrintFrame()
#else
#define DE_FTR_ENTER(a)
#define DE_FTR_LEAVE()
#define DE_FTR_PRINT()
#endif

}  // namespace DE