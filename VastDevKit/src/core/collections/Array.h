#ifndef _VAST_COLLECTIONS_ARRAY_H_
#define _VAST_COLLECTIONS_ARRAY_H_

#include "core/collections/CollectionSettings.h"

// Template array.

namespace hd {
    template<typename T>
    class Array {
    private:
        T *m_items;

        uint32_t m_capacity;

    public:
        Array();

        Array(uint32_t capacity);

        ~Array();

        int Add(const T &item, const uint32_t index);

        bool Remove(const uint32_t index);

        void Clear();

        T At(const uint32_t index) const;

        T * Item(const uint32_t index) const;

        T * GetItems() const;

        uint32_t Capacity() const;

        bool IsEmpty() const;

        bool IsFull() const;

        bool Contains(const T &item) const;

        void Resize(const uint32_t index);

        void PrintDebugInfo();
        /*
        void Map(void (T::*func)(void) const);

        void Map(void (T::*func)(void));*/
    };


    template<typename T>
    Array<T>::Array() : m_items(nullptr), m_capacity(1) {
        m_items = (T *)calloc(1, sizeof(T) * m_capacity);
        hdAssert(m_items);
    }


    template<typename T>
    Array<T>::Array(uint32_t capacity) : m_items(nullptr), m_capacity(capacity) {
        m_items = (T *)calloc(1, sizeof(T) * m_capacity);
        hdAssert(m_items);
    }


    template<typename T>
    Array<T>::~Array() {
        hdFreeSafely(m_items);
    }


    template<typename T>
    inline int Array<T>::Add(const T &item, const uint32_t index) {
        if (index >= m_capacity) {
            return _VAST_COLLECTIONS_ERROR_INVALID_INDEX;
        }

        m_items[index] = item;
        return index;
    }


    template<typename T>
    inline bool Array<T>::Remove(const uint32_t index) {
        if (index >= m_capacity) {
            return false;
        }

        m_items[index] = NULL;
        return true;
    }


    template<typename T>
    inline void Array<T>::Clear() {
        memset(m_items, 0, sizeof(T) * m_capacity);
    }


    template<typename T>
    inline T Array<T>::At(const uint32_t index) const {
        hdAssert(m_items);
        if (index >= m_capacity) {
            return nullptr;
        }
        return m_items[index];
    }


    template<typename T>
    inline T * Array<T>::Item(const uint32_t index) const {
        hdAssert(m_items);
        if (index >= m_capacity) {
            return nullptr;
        }
        return &m_items[index];
    }


    template<typename T>
    inline T * Array<T>::GetItems() const {
        return m_items;
    }


    template<typename T>
    inline uint32_t Array<T>::Capacity() const {
        return m_capacity;
    }


    template<typename T>
    inline bool Array<T>::IsEmpty() const {
        int i = 0;
        while (i < m_capacity) {
            if (m_items[i++]) {
                return false;
            }
        }
        return true;
    }


    template<typename T>
    inline bool Array<T>::IsFull() const {
        int i = 0;
        while (i < m_capacity) {
            if (!m_items[i++]) {
                return false;
            }
        }
        return true;
    }


    template<typename T>
    inline bool Array<T>::Contains(const T &item) const {
        if (item == nullptr) {
            return false;
        }

        if (m_capacity == 0) {
            return false;
        }

        unsigned i = 0;
        while (i < m_capacity) {
            if (item == m_items[i++]) {
                return true;
            }
        }
        return false;
    }


    template<typename T>
    inline void Array<T>::Resize(const uint32_t newsize) {
        m_capacity = newsize;
        m_items = (T **)realloc(m_items, sizeof(T) * m_capacity);
        hdAssert(m_items);
    }


    template<typename T>
    inline void Array<T>::PrintDebugInfo() {
        hd::log::i("Array Info:\n\tCapacity: %d\n\tAllocated: %zd\n\tItems:\n", m_capacity, malloc_size(m_items));
        int i = 0;
        while (i < m_capacity) {
            if (m_items[i]) {
                hd::log::i("\t\tArray contains item at %d: %p\n", i, &m_items[i]);
            }
            i++;
        }
    }


    /*
    template<typename T>
    inline void Array<T>::Map(void (T::*func)(void) const) {
        for (unsigned i = 0; i < m_capacity; ++i) {
            if (m_items[i]) {
                (m_items[i]->*func)();
            }
        }
    }


    // Lazy man's overloading
    template<typename T>
    inline void Array<T>::Map(void (T::*func)(void))
    {
        for (unsigned i = 0; i < m_capacity; ++i)
        {
            if (m_items[i])
            {
                (m_items[i]->*func)();
            }
        }
    }*/
}

#endif
