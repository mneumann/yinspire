/* 
 * Implements/wraps a simple dynamic array.
 */

#ifndef __YINSPIRE__ARRAY__
#define __YINSPIRE__ARRAY__

#include <vector>

template <class T>
class Array
{
  public:

    inline int
      size() const
      {
        return this->array.size();
      } 

    inline void
      push(T& i)
      {
        this->array.push_back(i);
      }

    inline T
      pop()
      {
        T& res = this->array.back();
        this->array.pop();
        return res;
      }

    inline T&
      operator[](int i)
      {
        return this->array[i];
      }

    inline void
      clear()
      {
        this->array.clear();
      }

  private:

    std::vector<T> array;
};

#endif
