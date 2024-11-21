#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class Trie {
        struct Node {
            Node(Ref<Node>& p, char prev) {
                parent         = p;
                char_to_parent = prev;
            }

            std::vector<size_t> terminal_idx;

            Ref<Node> parent;
            char      char_to_parent;

            std::unordered_map<char, Ref<Node>> next;
            std::unordered_map<char, Ref<Node>> sons;
            Ref<Node>                           suffix_link;
            Ref<Node>                           super_suffix_link;
        };

        Ref<Node> GetSuffixLink(Ref<Node> v);
        Ref<Node> GetNext(Ref<Node> v, char c);
        Ref<Node> GetUp(Ref<Node> v);

    public:
        void                          Build(std::vector<std::string>& dict);
        void                          AddWord(std::string& str);
        std::vector<Ref<std::string>> Search(std::string needle);

    private:
        Ref<Node>                     root_;
        std::vector<Ref<std::string>> words_;
    };

}  // namespace DummyEngine