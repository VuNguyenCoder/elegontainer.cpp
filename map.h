#ifndef __MAP_H__
#define __MAP_H__

#include "container.h"
#include "vector.h"

#include <unordered_map>
#include <string>
#include <sstream>
#include <functional>

namespace egt {
    template <typename K, typename V> 
    class map : public container<V, std::unordered_map<K, V>> {
    public:
        map() {}

        map(const std::initializer_list<std::pair<const K, V>>& list) { container_ = list; }
         
        typename std::unordered_map<K, V>::iterator find(const K& key) { return container_.find(key); }
        typename std::unordered_map<K, V>::const_iterator find(const K& key) const { return container_.find(key); }
        void erase(const K& key) { container_.erase(key); }
        
        map& for_each(const std::function<void(V& value)>& callback) {
            for (auto& item : container_) callback(item.second);
            return *this;
        }
      
        map& for_each(const std::function<void(V& value, const K& key)>& callback) {
            for (auto& item : container_) callback(item.second, item.first);
            return *this;
        }
      
        map& for_each(const std::function<void(V& value, const K& key, const map& m)>& callback) {
            for (auto& item : container_) callback(item.second, item.first, *this);
            return *this;
        }

        bool contains(const K& key) const { 
            return !(container_.find(key) == container_.end());
        }

        vector<K> keys() const {
            vector<K> v;
            for (auto& item : container_) v.push_back(item.first); 
            return v;
        }

        vector<V> values() const {
            vector<K> v;
            for (auto& item : container_) v.push_back(item.second); 
            return v;
        }

        map &remove_if(const std::function<bool(const V& value, const K& key, const map& m)>& callback) {
            for (auto it = container_.begin(); it != container_.end(); ) {
                K key = it->first;
                V value = it->second;
                if (callback(value, key, *this)) 
                    it = container_.erase(it);
                else 
                    ++it;
            }
            return *this;
        }

        template <typename U>
        U reduce(const std::function<U(U& total_value, const V& value, const K& key)>& callback, const U& initial_value) const {
            U total_value = initial_value;
            for (auto& item : container_) {
                auto& key = item.first;
                total_value = callback(total_value, container_.at(key), key);
            }
            return total_value;
        }
        V reduce(const std::function<V(V &total_value, const V& value, const K& key)> &callback,
                        const V& initial_value) const {
            return reduce<V>(callback);
        }
      
        std::string to_string(const std::string &delimiter = ", ", 
                                    const std::string &start_with = "{",
                                    const std::string &end_with = "}") const {
            std::string body = reduce<std::string>([&](auto &total_value, auto& value, auto& key) {
                std::stringstream ss;
                ss << "{" << key << ", " << value << "}";
                return total_value + ss.str() + ", ";
            }, "");
            if(body.length() > 2)
                body = body.replace(body.length()-2, 2, "");
            return start_with + body + end_with;
        }     

    private:
        using container<V, std::unordered_map<K,V>>::container_;
    };
    
    template <typename K, typename V> 
    inline bool operator==(const map<K, V>& m1, const map<K, V>& m2) {
        if(m1.keys() == m2.keys() && m1.values() == m2.values())
            return true;
        return false;
    }

    template <typename K, typename V> 
    inline std::ostream& operator<<(std::ostream& out, const map<K, V>& m) {
        out <<  m.to_string();
        return out;
    }
}

#endif
