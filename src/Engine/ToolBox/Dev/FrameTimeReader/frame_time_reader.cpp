#include "frame_time_reader.h"

#include <windows.h>

#include <iomanip>
#include <iostream>

namespace DE {

void FrameTimeReader::EnterArea(std::string area_name) {
    if (area_name == "") {
        // std::cout << "enter root" << std::endl;
        _current = _root;
        _current->begin_time = std::chrono::system_clock::now();
        _current->current_child_id = 0;
        return;
    }
    // std::cout << "enter " << area_name << std::endl;
    Node* next_node;
    if (_current->current_child_id == _current->children.size()) {
        _current->children.push_back(new Node());
        _current->children.back()->name = area_name;
        _current->children.back()->parent = _current;
    }
    next_node = _current->children[_current->current_child_id];
    next_node->begin_time = std::chrono::system_clock::now();
    next_node->current_child_id = 0;
    ++(_current->current_child_id);
    _current = next_node;
}
void FrameTimeReader::LeaveArea() {
    // std::cout << "leave " << _current->name << std::endl;
    _current->delta_time = std::chrono::system_clock::now() - _current->begin_time;
    _current = _current->parent;
}

void FrameTimeReader::PrintFrameTimes(Node* node, std::string tabulation) {
    if (node == _root) {
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    }
    std::cout << tabulation << node->name << std::fixed << std::setprecision(4) << " (" << node->delta_time.count() * 1000 << "ms)           \n";
    if (!node->children.empty()) {
        std::cout << tabulation << "{\n";
        for (const auto& child : node->children) {
            PrintFrameTimes(child, tabulation + "   ");
        }
        std::cout << tabulation << "}\n";
    }
    if (node == _root) {
        std::cout << std::endl;
    }
}

void FrameTimeReader::PrintFrame() {
    PrintFrameTimes(_root, "");
}

FrameTimeReader::FrameTimeReader() {
    system("cls");
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;  // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);

    _root = new Node();
    _root->name = "Frame";
    _root->current_child_id = 0;
    _root->parent = nullptr;
    _current = _root;
}

FrameTimeReader& FrameTimeReader::Get() {
    static FrameTimeReader frame_time_reader;
    return frame_time_reader;
}

}  // namespace DE