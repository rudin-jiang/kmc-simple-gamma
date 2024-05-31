#pragma once

#include <cassert>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>

template<typename Type, 
         typename Hash = std::hash<Type>,
         typename Equal = std::equal_to<Type>>
class RdSet {
private:
    using M_VEC_T = typename std::vector<Type>;
    using M_MAP_T = typename std::unordered_map<Type,
                                std::size_t, Hash, Equal>;

    M_VEC_T m_vec;
    M_MAP_T m_map;

public:
    RdSet() {}

    template <typename ContainerType>
    RdSet(const ContainerType &ct) {
        for (const Type &t: ct) {
            // ignore repeated elements
            if (m_map.find(t) != m_map.end())
                continue;

            m_vec.push_back(t);
            m_map[t] = m_vec.size() - 1;
        }
    }

    template <typename IteratorType>
    RdSet(const IteratorType &itBeg, const IteratorType &itEnd) {
        for (IteratorType it = itBeg; it != itEnd; ++it) {
            // ignore repeated elements
            if (m_map.find(*it) != m_map.end())
                continue;

            m_vec.push_back(*it);
            m_map[*it] = m_vec.size() - 1;
        }
    }


    using const_iterator = typename M_VEC_T::const_iterator;

    const_iterator begin()  const { return m_vec.cbegin(); }
    const_iterator end()    const { return m_vec.cend();   }
    const_iterator cbegin() const { return m_vec.cbegin(); }
    const_iterator cend()   const { return m_vec.cend();   }

    const_iterator find(const Type &key) const {
        auto map_it = m_map.find(key);
        
        // not find
        if (map_it == m_map.end()) {
            return cend();
        }

        std::size_t pos = map_it -> second;
        assert(pos < m_vec.size());
        return cbegin() + pos;
    }

    std::size_t size() const {
        return m_vec.size();
    }

    bool empty() const {
        return m_vec.empty();
    }

    bool insert(const Type &t) {
        if (m_map.find(t) != m_map.end()) {
            return false;
        }

        m_vec.push_back(t);
        m_map[t] = m_vec.size() - 1;
        return true;
    }

    bool erase(const Type &t) {
        auto map_it0 = m_map.find(t);
        
        if (map_it0 == m_map.end()) {
            return false;
        }

        auto map_it1 = m_map.find(m_vec.back());

        std::size_t idx0 = map_it0 -> second;
        std::size_t idx1 = map_it1 -> second;

        // swap t and m_vec.back()
        map_it0 -> second = idx1;
        map_it1 -> second = idx0;
        std::swap(m_vec[idx0], m_vec[idx1]);
        
        // remove element m_vec.back()
        m_map.erase(map_it0);
        m_vec.pop_back();

        return true;
    }

    Type random_element() const {
        if (empty()) {
            throw std::out_of_range(
                "Attempt to get an element "
                "from an empty RdSet."
            );
        }

        std::size_t uniform_random_sizet(void);
        std::size_t rdNum = uniform_random_sizet();
        return m_vec[rdNum % m_vec.size()];
    }


    // The internal state of this RdSet is normal
    // mainly used in testing and debug
    bool check_good() const {
        if (m_vec.size() != m_map.size()) return false;

        for (std::size_t idx = 0; idx < m_vec.size(); ++idx) {
            auto it = m_map.find(m_vec[idx]);
            if (it == m_map.end()) return false;
            if (it -> second != idx) return false;
        }

        return true;
    }

    // elements in RdSet are equal to elements in Container
    // mainly used in testing and debug
    template <typename ContainerType>
    bool check_equal(const ContainerType &ct) {
        if (ct.size() != m_vec.size()) return false;
        if (ct.size() != m_map.size()) return false;

        for (const Type &t: ct) {
            auto map_it = m_map.find(t);
            if (map_it == m_map.end()) {
                return false;
            }
            
            std::size_t pos = map_it -> second;
            if (pos >= m_vec.size()) return false;
            if (!(m_vec[pos] == t))  return false;
        }

        return true;
    }
};

