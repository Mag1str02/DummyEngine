#include "Trie.hpp"

#include <set>

namespace DummyEngine {

    Ref<Trie::Node> Trie::GetSuffixLink(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->suffix_link) {
            if (v == root_ || v->parent == root_) {
                v->suffix_link = root_;
            } else {
                v->suffix_link = GetNext(GetSuffixLink(v->parent), v->char_to_parent);
            }
        }
        return v->suffix_link;
    }

    Ref<Trie::Node> Trie::GetNext(Ref<Trie::Node> v, char c) {
        if (!v) {
            return v;
        }
        if (v->next.contains(c)) {
            return v->next[c];
        } else if (v->sons.contains(c)) {
            v->next[c] = v->sons[c];
        } else if (v == root_) {
            v->next[c] = root_;
        } else {
            v->next[c] = GetNext(GetSuffixLink(v), c);
        }
        return v->next[c];
    }

    Ref<Trie::Node> Trie::GetUp(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->super_suffix_link) {
            if (!GetSuffixLink(v)->terminal_idx.empty()) {  // leaf
                v->super_suffix_link = GetSuffixLink(v);
            } else if (GetSuffixLink(v) == root_) {
                v->super_suffix_link = root_;
            } else {
                v->super_suffix_link = GetUp(GetSuffixLink(v));
            }
        }
        return v->super_suffix_link;
    }

    void Trie::AddWord(std::string& str) {
        Ref<Trie::Node> current = root_;
        for (const char c : str) {
            if (!current->sons.contains(c)) {
                current->sons[c] = CreateRef<Trie::Node>(current, c);
            }
            current = current->sons[c];
        }
        current->terminal_idx.push_back(words_.size());
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
                for (const auto& idx : ptr->terminal_idx) {
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
