/*
 * Implementation of a Pairing Heap
 *
 * Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
 *
 * Requirements for "ACC": 
 *
 *   struct ACC {
 *     inline static bool less(const T*, const T*); 
 *     inline static T*& next(T*);
 *     inline static T*& previous(T*);
 *     inline static T*& child(T*);
 *   }
 *
 */

#ifndef __YINSPIRE__PAIRING_HEAP__
#define __YINSPIRE__PAIRING_HEAP__

template <class T, class ACC=T>
class PairingHeap
{
  public:

    PairingHeap()
    {
      this->root = NULL;
      this->size_ = 0;
    }

    bool
      empty() const
      {
        return (this->root == NULL);
      }

    unsigned int
      size() const
      {
        return this->size_;
      }

    T*
      top() const
      {
        return this->root;
      }

    void
      pop()
      {
        T* current;
        T* last;
        T* r1;
        T* r2;

        --this->size_;

        if (ACC::child(this->root) != NULL)
        {
          // remove parent pointer from left-most child node
          ACC::previous(ACC::child(this->root)) = NULL;

          current = ACC::child(this->root);
          last = NULL;

          // left-to-right pass
          while (current != NULL)
          {
            r1 = current;
            r2 = ACC::next(current);

            if (r2 == NULL)
            {
              // no need to "meld", because we're at the end
              ACC::previous(r1) = last;
              last = current;
              current = NULL;
            }
            else
            {
              current = ACC::next(r2);

              ACC::next(r1) = NULL;
              ACC::previous(r1) = NULL;
              ACC::next(r2) = NULL;
              ACC::previous(r2) = NULL;

              r1 = meld(r1, r2);
              ACC::previous(r1) = last;
              last = r1;
            }
          }

          this->root = last;
          current = ACC::previous(last);

          // make it a clean root-node
          ACC::next(this->root) = NULL;
          ACC::previous(this->root) = NULL;	

          // right-to-left pass 
          while (current != NULL)
          {
            r2 = current;

            current = ACC::previous(r2);
            ACC::previous(r2) = NULL; // make it a clean root-node

            this->root = meld(this->root, r2);
          }
        }
        else
        {
          this->root = NULL;
        }
      }

    /*
     * Insert a new element into the Pairing Heap.
     */
    void
      push(T* node)
      {
        ACC::next(node) = NULL;
        ACC::previous(node) = NULL; // == NULL means it's a root node
        ACC::child(node) = NULL;

        ++this->size_;

        this->root = empty() ? node : meld(this->root, node);
      }

  private:

    /*
     * Meld two Pairing Heaps
     */
    T*
      meld(T* root1, T* root2)
      {
        if (ACC::less(root2, root1))
        {
          T* tmp = root1;
          root1 = root2;
          root2 = tmp;
        }

        // root2 becomes the leftmost node of root1
        ACC::previous(root2) = root2;  // "parent" pointer for leftmost child
        ACC::next(root2) = ACC::child(root1);

        if (ACC::child(root1) != NULL)
        {
          // assign double linked-list
          ACC::previous(ACC::child(root1)) = root2;
        }
        ACC::child(root1) = root2;
        return root1;
      }

  private:

    T* root;
    unsigned int size_;

};

#endif
