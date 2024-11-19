#include "Trie.hpp"

namespace DE {
    Ref<Trie::Node> Trie::getSuffixLink(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->suffix_link) {
            if (v == m_root || v->parent == m_root) {
                v->suffix_link = m_root;
            } else {
                v->suffix_link = getNext(getSuffixLink(v->parent), v->char_to_parent);
            }
        }
        return v->suffix_link;
    }

    Ref<Trie::Node> Trie::getNext(Ref<Trie::Node> v, char c) {
        if (!v) {
            return v;
        }
        if (v->next.contains(c)) {
            return v->next[c];
        } else if (v->sons.contains(c)) {
            v->next[c] = v->sons[c];
        } else if (v == m_root) {
            v->next[c] = m_root;
        } else {
            v->next[c] = getNext(getSuffixLink(v), c);
        }
        return v->next[c];
    }

    Ref<Trie::Node> Trie::getUp(Ref<Trie::Node> v) {
        if (!v) {
            return v;
        }
        if (!v->super_suffix_link) {
            if (!getSuffixLink(v)->terminal_idx.empty()) {  // leaf
                v->super_suffix_link = getSuffixLink(v);
            } else if (getSuffixLink(v) == m_root) {
                v->super_suffix_link = m_root;
            } else {
                v->super_suffix_link = getUp(getSuffixLink(v));
            }
        }
        return v->super_suffix_link;
    }

    void Trie::AddWord(std::string& str) {
        Ref<Trie::Node> current = m_root;
        for (const char c : str) {
            if (!current->sons.contains(c)) {
                current->sons[c] = CreateRef<Trie::Node>(current, c);
            }
            current = current->sons[c];
        }
        current->terminal_idx.push_back(m_words.size());
        m_words.push_back(CreateRef<std::string>(str));
    }

    void Trie::Build(std::vector<std::string>& dict) {
        for (std::string& str : dict) {
            AddWord(str);
        }
    }

    std::vector<Ref<std::string>> Trie::Search(std::string needle) {
        std::vector<Ref<std::string>> result;
        std::set<size_t>              idx_found;
        Ref<Trie::Node>               current = m_root;
        for (char c : needle) {
            current   = getNext(current, c);
            auto& ptr = current;
            while (ptr != m_root) {
                for (const auto& idx : ptr->terminal_idx) {
                    if (idx_found.contains(idx)) {
                        continue;
                    }
                    idx_found.insert(idx);
                    result.push_back(m_words[idx]);
                }
                ptr = getUp(ptr);
            }
        }
        return result;
    }

}  // namespace DE
