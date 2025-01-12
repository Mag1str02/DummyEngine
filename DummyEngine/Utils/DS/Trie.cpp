#include "Trie.hpp"

#include <set>

namespace DummyEngine {

    Ref<Trie::Node> Trie::GetSuffixLink(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->SuffixLink) {
            if (v == root_ || v->Parent == root_) {
                v->SuffixLink = root_;
            } else {
                v->SuffixLink = GetNext(GetSuffixLink(v->Parent), v->CharToParent);
            }
        }
        return v->SuffixLink;
    }

    Ref<Trie::Node> Trie::GetNext(Ref<Trie::Node> v, char c) {
        if (!v) {
            return v;
        }
        if (v->Next.contains(c)) {
            return v->Next[c];
        } else if (v->Sons.contains(c)) {
            v->Next[c] = v->Sons[c];
        } else if (v == root_) {
            v->Next[c] = root_;
        } else {
            v->Next[c] = GetNext(GetSuffixLink(v), c);
        }
        return v->Next[c];
    }

    Ref<Trie::Node> Trie::GetUp(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->SuperSuffixLink) {
            if (!GetSuffixLink(v)->TerminalIdx.empty()) {  // leaf
                v->SuperSuffixLink = GetSuffixLink(v);
            } else if (GetSuffixLink(v) == root_) {
                v->SuperSuffixLink = root_;
            } else {
                v->SuperSuffixLink = GetUp(GetSuffixLink(v));
            }
        }
        return v->SuperSuffixLink;
    }

    void Trie::AddWord(std::string& str) {
        Ref<Trie::Node> current = root_;
        for (const char c : str) {
            if (!current->Sons.contains(c)) {
                current->Sons[c] = CreateRef<Trie::Node>(current, c);
            }
            current = current->Sons[c];
        }
        current->TerminalIdx.push_back(words_.size());
        words_.push_back(CreateRef<std::string>(str));
    }

    void Trie::Build(std::vector<std::string>& dict) {
        for (std::string& str : dict) {
            AddWord(str);
        }
    }

    std::vector<Ref<std::string>> Trie::Search(std::string needle) {
        std::vector<Ref<std::string>> result;
        std::set<size_t>              idx_found;
        Ref<Trie::Node>               current = root_;
        for (char c : needle) {
            current   = GetNext(current, c);
            auto& ptr = current;
            while (ptr != root_) {
                for (const auto& idx : ptr->TerminalIdx) {
                    if (idx_found.contains(idx)) {
                        continue;
                    }
                    idx_found.insert(idx);
                    result.push_back(words_[idx]);
                }
                ptr = GetUp(ptr);
            }
        }
        return result;
    }

}  // namespace DummyEngine
