#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <deque>
#include <vector>
#include <functional>

namespace asilla {
  template <typename T>
  class queue {
    public:
      /** 
        * Wrap existing operator and method of unordered_map 
        */
      T& operator[](int index) { return queue_[index]; }
      const T& at(int index) const { return queue_.at(index); }
      T& at(int index) { return queue_.at(index); }
      
      void push_back(const T&value) { return queue_.push_back(value); }
      
      T& front() { return queue_.front(); }
      T& back() { return queue_.back(); }
      T pop_front() { 
        T value = queue_.front();
        queue_.pop_front(); // this is void function
        return value; //we might need consider to this. or void function, or return reference to avoid copy data
      }

      int size() const { return queue_.size(); }

      void clear() { queue_.clear(); }

      typename std::deque<T>::iterator begin() { return queue_.begin(); }
      typename std::deque<T>::const_iterator begin() const { return queue_.begin(); }
      typename std::deque<T>::iterator end() { return queue_.end(); }
      typename std::deque<T>::const_iterator end() const { return queue_.end(); }
     
      /** 
        * Addtional utility method 
        */
      void map(const std::function<void(T&)> &callback) {
        for (auto &value : queue_) callback(value);
      }
  private:
    std::deque<T> queue_;
  };
}

#endif
