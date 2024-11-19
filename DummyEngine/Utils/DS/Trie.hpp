#pragma once

#include "../Types/Types.h"

namespace DE {
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

        Ref<Node> getSuffixLink(Ref<Node> v);
        Ref<Node> getNext(Ref<Node> v, char c);
        Ref<Node> getUp(Ref<Node> v);

    public:
        void                          Build(std::vector<std::string>& dict);
        void                          AddWord(std::string& str);
        std::vector<Ref<std::string>> Search(std::string needle);

    private:
        Ref<Node>                     m_root;
        std::vector<Ref<std::string>> m_words;
    };
}  // namespace DE