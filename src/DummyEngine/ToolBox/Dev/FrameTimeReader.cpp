#include "ToolBox/Dev/FrameTimeReader.h"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{

    void FrameTimeReader::EnterScope(std::string area_name)
    {
        if (area_name == "")
        {
            m_Current = m_Root;
            m_Current->begin_time = std::chrono::system_clock::now();
            m_Current->current_child_id = 0;
            return;
        }
        Node* next_node;
        if (m_Current->current_child_id == m_Current->children.size())
        {
            m_Current->children.push_back(new Node());
            m_Current->children.back()->name = area_name;
            m_Current->children.back()->parent = m_Current;
        }

        next_node = m_Current->children[m_Current->current_child_id];
        next_node->begin_time = std::chrono::system_clock::now();
        next_node->current_child_id = 0;

        ++(m_Current->current_child_id);
        m_Current = next_node;
    }
    void FrameTimeReader::LeaveScope()
    {
        m_Current->delta_time = std::chrono::system_clock::now() - m_Current->begin_time;
        m_Current = m_Current->parent;
    }

    void FrameTimeReader::PrintFrameTimes(Node* node, std::string tabulation)
    {
        if (node == m_Root)
        {
            COORD cursorPosition;
            cursorPosition.X = 0;
            cursorPosition.Y = 0;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
            std::cout << "FPS: " << int(1.0 / m_Root->delta_time.count()) << "        " << std::endl;
        }
        std::cout << tabulation << node->name << std::fixed << std::setprecision(4) << " (" << node->delta_time.count() * 1000 << "ms)           \n";
        if (!node->children.empty())
        {
            std::cout << tabulation << "{\n";
            for (const auto& child : node->children)
            {
                PrintFrameTimes(child, tabulation + "   ");
            }
            std::cout << tabulation << "}\n";
        }
        if (node == m_Root)
        {
            std::cout << std::endl;
            FrameStatistics statistics = Renderer::GetStatistics();
            std::cout << "DrawCalls: " << statistics.m_DrawCallsAmount << std::endl;
            std::cout << "DrawnInstances: " << statistics.m_DrawnInstances << std::endl;
            std::cout << "DrawCalls save by instancing: " << statistics.m_DrawnInstances - statistics.m_DrawCallsAmount << std::endl;
        }
    }

    void FrameTimeReader::PrintFrame()
    {
        PrintFrameTimes(m_Root, "");
    }

    FrameTimeReader::FrameTimeReader()
    {
        system("cls");
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false;  // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);

        m_Root = new Node();
        m_Root->name = "Frame";
        m_Root->current_child_id = 0;
        m_Root->parent = nullptr;
        m_Current = m_Root;
    }

    FrameTimeReader& FrameTimeReader::Get()
    {
        static FrameTimeReader frame_time_reader;
        return frame_time_reader;
    }

}  // namespace DE