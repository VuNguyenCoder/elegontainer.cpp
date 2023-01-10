#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "container.h"

#include <vector>
#include <string>
#include <sstream>
#include <functional>

namespace asilla {
    template <typename T> 
    class vector : public container<T, std::vector<T>> {
    public:
        vector() {}
        vector(const std::initializer_list<T>& list) { container_= list; }
        vector(const std::vector<T>& v) { container_ = v; }

        void resize(int const size) { container_.resize(size); }
        void append(const T& value) { container_.append(value); }

        vector& for_each(const std::function<void(T& value, int index)>& callback) {
            for (int i = 0; i < container_.size(); i++) {
                callback(container_[i], i);
            }
            return *this;
        }
        
        template <typename U = T>
        vector<U> map(const std::function<U(const T& value, int index)>& callback) const {
            vector<U> v;
            for (int i = 0; i < container_.size(); i++) {
                v.push_back(callback(container_.at(i), i));
            }
            return v;
        }
        vector<T> map(const std::function<T(const T& value, int index)>& callback) const {
            return map<>(callback);
        }

        vector<T> filter(const std::function<bool(const T& value, int index)> &callback) const {      
            vector<T> v = *this;
            int i = 0;
            for (auto it = v.begin(); it != v.end(); i++ ) {
                if (!callback(*it, i)) 
                    v.erase(it);
                else 
                    ++it;
            }
            return v;
        }

        template <typename U = T>
        const U reduce(const std::function<U(U& total_value, const T& value, int index)>& callback,
                        const U& initial_value) const {
            U total_value = initial_value;
            for (int i = 0; i < container_.size(); i++) {
                total_value = callback(total_value, container_.at(i), i);
            }
            return total_value;
        }
        T reduce(const std::function<T(T& total_value, const T& value, int index)>& callback,
                        const T& initial_value) const {
            return reduce<>(callback, initial_value);
        }

        std::string to_string(const std::string& delimiter = ", ", 
                                    const std::string& start_with = "{",
                                    const std::string& end_with = "}") const {
            std::string body = reduce<std::string>([&](std::string& total_value, const T& value, int index) {
                std::stringstream ss;
                ss << value;
                return total_value + ss.str() + ", ";
            }, "");
            if(body.length() > 2)
                body = body.replace(body.length()-2, 2, "");
            return start_with + body + end_with;
        } 

        friend std::ostream& operator<<(std::ostream& out, const vector& v) {
            out << v.to_string();
            return out;
        }

        friend bool operator==(const vector<T>& v1, const vector<T>& v2) {
            if(v1.size() != v2.size()) return false;
            for(int i = 0; i < v1.size(); i++) {
                if(!(v1.at(i) == v2.at(i))) return false;
            }
            return true;
        }
      
    private:
        using container<T, std::vector<T>>::container_;
    };
}

#endif
