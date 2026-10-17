#ifndef _VAST_POINTER_LIST_H_
#define _VAST_POINTER_LIST_H_

#include <Vast/VastMath.h>
#include "core/collections/CollectionSettings.h"

// A serializable list of pointers to other objects using
// templates rather than (void *). Will give the code a bit
// more semantic meaning.

namespace hd {
    template<typename T>
    class Vector {
    private:
        T *m_items;

        uint32_t m_itemCount;

        uint32_t m_capacity;

        int AddAtIndex(const T item, const uint32_t index);

        void Resize(const uint32_t index);

        bool Replace(const T &item, const uint32_t index);

        T m_end;

        T m_null;

    public:

        Vector();

        Vector(uint32_t capacity);

        ~Vector();

        int Add(const T item);

        void Enqueue(const T item);

        bool Remove(const T &item);

        bool RemoveAtIndex(const uint32_t index);

        bool Clear();

        T * First() const;

        T * Last() const;

        T * IteratorBegin() const;

        T * IteratorEnd() const;

        T Top() const;

        T Pop();

        T At(const uint32_t index) const;

        T * Item(const uint32_t index) const;

        T * GetItems() const;

        uint32_t GetSize() const;

        uint32_t Size() const;

        uint32_t Capacity() const;

        bool IsEmpty() const;

        bool IsFull() const;

		T operator [] (uint32_t i);

		T operator [] (uint32_t i) const;

        bool Contains(const T &item) const;

        /*
        void Map(void (T::*func)(void) const);

        void Map(void (T::*func)(void));*/

        T End();
    };


    template<typename T>
    Vector<T>::Vector() : m_items(nullptr), m_capacity(0), m_itemCount(0) {
        m_items = (T *)calloc(1, sizeof(T) * 1);
        hdAssert(m_items);
    }


    template<typename T>
    Vector<T>::Vector(uint32_t capacity) : m_items(nullptr), m_capacity(capacity), m_itemCount(0) {
        m_items = (T *)calloc(1, sizeof(T) * m_capacity);
        hdAssert(m_items);
    }


    template<typename T>
    Vector<T>::~Vector() {
        hdAssert(m_items);
        hdFreeSafely(m_items);
    }


    template<typename T>
    inline int Vector<T>::Add(const T item) {
        hdAssert(m_itemCount <= m_capacity);
        return this->AddAtIndex(item, (const int)m_itemCount);
    }


    template<typename T>
    inline void Vector<T>::Enqueue(const T item) {
        hdAssert(m_itemCount <= m_capacity);
        int index = this->AddAtIndex(item, 0);
        hdAssert(index == 0);
    }


    template<typename T>
    inline bool Vector<T>::Remove(const T &item) {
        if (item == nullptr) {
            return false;
        }

        // find the index
        unsigned i = 0;
        while (i < m_itemCount) {
            if (item == m_items[i]) break;
            ++i;
        }

        if (i >= m_itemCount) {
            return false;
        }
        return this->RemoveAtIndex(i);
    }


    template<typename T>
    inline bool Vector<T>::RemoveAtIndex(const uint32_t index) {
        hdAssert(m_itemCount <= m_capacity);
        if (m_itemCount == 0) {
            return false;
        }

        if (index >= m_itemCount) {
            return false;
        }

        m_items[index] = m_null;

        // move everything down
        for (unsigned i = index; i < m_itemCount-1; ++i) {
            m_items[i] = m_items[i+1];
        }
        m_items[m_itemCount-1] = m_null;
        --m_itemCount;
        return true;
    }


    template<typename T>
    inline bool Vector<T>::Clear() {
        for (unsigned i = 0; i < m_itemCount; ++i) {
            m_items[i] = m_null;
        }
        m_itemCount = 0;
        return true;
    }


    template<typename T>
    T * Vector<T>::First() const {
        if (m_itemCount > 0) {
            return &m_items[0];
        } else {
            return nullptr;
        }
    }


    template<typename T>
    T * Vector<T>::Last() const {
        if (m_itemCount > 0) {
            return &m_items[m_itemCount - 1];
        } else {
            return nullptr;
        }
    }

    template<typename T>
    T * Vector<T>::IteratorBegin() const {
        if (m_itemCount > 0) {
            return &m_items[0];
        } else {
            return nullptr;
        }
    }


    template<typename T>
    T * Vector<T>::IteratorEnd() const {
        if (m_itemCount > 0) {
            return &m_items[m_itemCount];
        } else {
            return nullptr;
        }
    }


    template<typename T>
    inline T* Vector<T>::GetItems() const {
        return m_items;
    }


    template<typename T>
    inline T Vector<T>::Top() const
    {
        hdAssert(m_items);
        if (m_itemCount == 0) {
            return m_end;
        }
        else return m_items[m_itemCount - 1];
    }


    template<typename T>
    inline T Vector<T>::Pop() {
        if (m_itemCount == 0) {
            return m_end;
        }

        T *top = Item(m_itemCount - 1);
        if (top) {
            T popped = *top;
            RemoveAtIndex(m_itemCount - 1);
            return popped;
        }
        return m_end;
    }


    template<typename T>
    inline T Vector<T>::At(const uint32_t index) const {
        hdAssert(m_items);
        if (m_itemCount == 0) {
            return m_end;
        }
        if (index >= m_itemCount) {
            return m_end;
        }
        return m_items[index];
    }


    template<typename T>
    inline T * Vector<T>::Item(const uint32_t index) const {
        hdAssert(m_items);
        if (m_itemCount == 0) {
            return nullptr;
        }
        if (index >= m_itemCount) {
            return nullptr;
        }
        return &m_items[index];
    }


    template<typename T>
    inline uint32_t Vector<T>::GetSize() const {
        return m_itemCount;
    }


    template<typename T>
    inline uint32_t Vector<T>::Size() const {
        return m_itemCount;
    }


    template<typename T>
    inline uint32_t Vector<T>::Capacity() const {
        return m_capacity;
    }


    template<typename T>
    inline bool Vector<T>::IsEmpty() const {
        return m_itemCount == 0;
    }


    template<typename T>
    inline bool Vector<T>::IsFull() const {
        return m_itemCount == m_capacity;
    }


    template<typename T>
    inline T Vector<T>::operator [] (uint32_t i) {
        return At(i);
    }


    template<typename T>
    inline T Vector<T>::operator [] (uint32_t i) const {
        return At(i);
    }


    template<typename T>
    inline bool Vector<T>::Contains(const T &item) const {
        if (item == nullptr) {
            return false;
        }

        if (m_itemCount == 0) {
            return false;
        }

        unsigned i = 0;
        while (i < m_itemCount) {
            if (item == m_items[i]) {
                return true;
            }
            ++i;
        }
        return false;
    }

    /*
    template<typename T>
    inline void Vector<T>::Map(void (T::*func)(void) const)
    {
        for (unsigned i = 0; i < m_itemCount; ++i)
        {
            T *item = (T *)m_items[i];
            if (item != NULL)
            {
                (item->*func)();
            }
        }
    }


    // Lazy man's overloading
    template<typename T>
    inline void Vector<T>::Map(void (T::*func)(void))
    {
        for (unsigned i = 0; i < m_itemCount; ++i)
        {
            T *item = (T *)m_items[i];
            if (item != NULL)
            {
                (item->*func)();
            }
        }
     }*/

    template<typename T>
    inline T Vector<T>::End() {
        return m_end;
    }


#pragma mark - Resize

    template<typename T>
    inline void Vector<T>::Resize(const uint32_t newsize) {
        if (newsize == 0) {
            return;
        }

        uint32_t oldsize = m_capacity;

        m_capacity = newsize;
        m_items = (T *)realloc(m_items, sizeof(T) * m_capacity);

        for (uint32_t i = oldsize; i < m_capacity; ++i) {
            m_items[i] = m_null;
        }

        hdAssert(m_items);
    }


    template<typename T>
    inline int Vector<T>::AddAtIndex(const T item, const uint32_t index) {
        hdAssert(m_itemCount <= m_capacity);

        // Increase capacity by 50%
        if ((m_itemCount + 1) > m_capacity) {
            Resize(m_capacity >= 2 ? m_capacity + hd::min(int(0.5f * m_capacity), 2048) : m_capacity + 50);
        }
        hdAssert(m_itemCount < m_capacity);

        // Make room for the new item
        if (m_itemCount > 0) {
            for (uint32_t i = hd::max(int(m_itemCount) - 1, 0); i >= index && i <= (m_itemCount - 1); --i) {
                m_items[i+1] = m_items[i];
                m_items[i] = m_null;
            }
        }
        m_items[index] = item;
        ++m_itemCount;
        return index;
    }


    template<typename T>
    inline bool Vector<T>::Replace(const T &item, const uint32_t index) {
        if (item == nullptr) {
            return false;
        }
        hdAssert(m_itemCount <= m_capacity);
        if (index >= m_itemCount) {
            return false;
        }

        m_items[index] = (T *)item;
        return true;
    }
}

#endif
