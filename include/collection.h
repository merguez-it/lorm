#ifndef _COLLECTION_T_
#define _COLLECTION_T_

#include <iostream>
#include <vector>
#include <string>

template <class T> class collection : public std::vector<T> {
  public:
	collection() : std::vector<T>() {}
    collection(int size) : std::vector<T>(size) {}
    template <class I>collection(I first, I last) : std::vector<T>(first, last) {}
    
    void remove() {
      typename std::vector<T>::iterator it;
      for(it = this->begin(); it != this->end(); it++) {
        (*it).remove();
      }
      this->clear();
    }

    size_t count() {
      return this->size();
    }
	
	T& operator[](size_t n) {
		T& ret=std::vector<T>::operator[](n);
		if (!(ret.is_loaded())) {
			ret=T::search_by_id(ret.id);
		}
		return ret;
	}
};

#endif //_COLLECTION_T_
