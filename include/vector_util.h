#ifndef __VECTOR_UTIL_INCLUDE
#define __VECTOR_UTIL_INCLUDE

#include <vector>

template <typename T> class create_vector {
  private:
    std::vector<T> m_vec;
  public:
    create_vector(const T& val) {
      m_vec.push_back(val);
    }
    create_vector<T>& operator()(const T& val) {
      m_vec.push_back(val);
      return *this;
    }
    operator std::vector<T>() {
      return m_vec;
    }
};

#endif // __VECTOR_UTIL_INCLUDE

