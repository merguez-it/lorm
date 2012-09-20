#include <iostream>
#include <vector>
#include <string>

template <class T> class group : public std::vector<T> {
  public:
    group() : std::vector<T>() {}
    group(int size) : std::vector<T>(size) {}
    template <class I>group(I first, I last) : std::vector<T>(first, last) {}
    
    void remove() {
      typename std::vector<T>::iterator it;
      for(it = this->begin(); it != this->end(); it++) {
        (*it).remove();
      }
    }
};
