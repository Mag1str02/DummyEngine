#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <unordered_map>
#include <vector>

namespace DummyEngine {

    class Trie {
        struct Node {
            Node(Ref<Node>& p, char prev) {
                Parent       = p;
                CharToParent = prev;
            }

            std::vector<size_t> TerminalIdx;

            Ref<Node> Parent;
            char      CharToParent;

            std::unordered_map<char, Ref<Node>> Next;
            std::unordered_map<char, Ref<Node>> Sons;
            Ref<Node>                           SuffixLink;
            Ref<Node>                           SuperSuffixLink;
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