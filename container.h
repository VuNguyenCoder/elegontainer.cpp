#ifndef __CONTAINER_H__
#define __CONTAINER_H__

namespace asilla {
    template <typename T, typename C> 
    class container {
    public:
        // Access method
        T& operator[](int index) { return container_[index]; }
        const T& at(int index) const { return container_.at(index); }
        T& at(int index) { return container_.at(index); }
        T& front() { return container_.front(); }
        T& back() { return container_.back(); }
        
        int size() const { return container_.size(); }
        
        // Iterator method
        typename C::iterator begin() { return container_.begin(); }
        typename C::const_iterator begin() const { return container_.begin(); }
        typename C::iterator end() { return container_.end(); }
        typename C::const_iterator end() const { return container_.end(); }
        typename C::iterator erase(typename C::iterator pos) { return container_.erase(pos); }
        typename C::const_iterator erase(typename C::const_iterator pos) { return container_.erase(pos); }
        
        // CRUD method 
        void push_back(const T& value) { return container_.push_back(value); }
        void clear() { container_.clear(); }
        
    protected: 
        C container_;
    };
}

#endif
