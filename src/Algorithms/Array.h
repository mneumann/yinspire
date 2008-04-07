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

    inline bool
      empty() const
      {
        return this->array.empty();
      }

    inline void
      push(const T& i)
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
      remove(const T& e)
      {
        // FIXME!!!
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
