#ifndef _VAST_COLLECTIONS_TYPEDEF_ARRAY_H_
#define _VAST_COLLECTIONS_TYPEDEF_ARRAY_H_

#include "core/collections/CollectionSettings.h"

namespace hd
{
    template<typename T, int Max>
    class TypedefArray
	{
	private:
		T m_list[Max];
		
		unsigned int m_itemCount;
		
	public:
		TypedefArray()
		{
			memset(m_list, 0, sizeof(m_list));
			m_itemCount = 0;
		}
		
		int Add(const T& item);
		
		int AddAtIndex(const T& item, const unsigned int index);
		
		bool Replace(const T& item, const unsigned int index);
		
		bool Remove(const T& item);
		
		bool RemoveAtIndex(const unsigned int index);
		
		bool RemoveAll();
		
		T* GetItems();
		
		T GetItemAtIndex(const unsigned int index);
		
		const T Top() const;
		
		const unsigned int GetItemCount() const;
		
		bool IsEmpty() const;
		
		bool IsFull() const;
		
		T& operator [] (unsigned int i);
		
		T operator [] (unsigned int i) const;
		
		//void Map(void (*func)(T& item) const);
		
		bool Contains(const T& item) const;
		
		void Map(void (*func)(T& item));
	};
    
    
    template<typename T, int Max>
    inline int TypedefArray<T, Max>::Add(const T& item) {
        hdAssert(m_itemCount <= Max);
        
        if (m_itemCount == Max) {
            return _VAST_COLLECTIONS_ERROR_FULL;
        }
        
        return this->AddAtIndex(item, (const unsigned int)m_itemCount);
    }
    
    
    template<typename T, int Max>
    inline int TypedefArray<T, Max>::AddAtIndex(const T& item, const unsigned int index) {
        hdAssert(m_itemCount <= Max);
        
        if (m_itemCount == Max) {
            return _VAST_COLLECTIONS_ERROR_FULL;
        }
        
        if (index > m_itemCount) {
            return _VAST_COLLECTIONS_ERROR_INVALID_INDEX;
        }
        
        // Make room for the new item; ignore if index is the last item
        for (unsigned i = m_itemCount; i >= index && i < (Max - 1); --i) {
            m_list[i+1] = m_list[i];
        }
        
        m_list[index] = item;
        ++m_itemCount;
        
        return index;
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::Replace(const T& item, const unsigned int index) {
        hdAssert(m_itemCount <= Max);
        if (index >= m_itemCount) {
            return false;
        }
        
        m_list[index] = item;
        return true;
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::Remove(const T& item) {
        if (item == NULL) {
            return false;
        }
        
        // find the index
        unsigned i = 0;
        while (i < m_itemCount) {
            if (item == m_list[i]) break;
            ++i;
        }

        if (i >= m_itemCount) {
            return false;
        }

        return this->RemoveAtIndex(i);
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::RemoveAtIndex(const unsigned int index) {
        hdAssert(m_itemCount <= Max);
        if (m_itemCount == 0) {
            return false;
        }

        if (index >= m_itemCount) {
            return false;
        }
        
        // move everything down
        for (unsigned i = index; i < m_itemCount-1; ++i) {
            m_list[i] = m_list[i+1];
        }

        memset(m_list + (m_itemCount - 1), 0, sizeof(T));
        --m_itemCount;
        return true;
    }
	
	
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::RemoveAll() {
        m_itemCount = 0;
        memset(m_list, 0, sizeof(m_list));
        return true;
    }
    
    
    template<typename T, int Max>
    inline T* TypedefArray<T, Max>::GetItems() {
        return m_list;
    }
    
    
    template<typename T, int Max>
    T TypedefArray<T, Max>::GetItemAtIndex(const unsigned int index) {
        if (m_itemCount == 0) {
            return NULL;
        }

        if (index >= m_itemCount) {
            return NULL;
        }

        return m_list[index];
    }
    
    
    template<typename T, int Max>
    const T TypedefArray<T, Max>::Top() const {
        if (m_itemCount == 0) {
            return NULL;
        }

        return m_list[m_itemCount - 1];
    }
    
    
    template<typename T, int Max>
    inline const unsigned int TypedefArray<T, Max>::GetItemCount() const {
        return m_itemCount;
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::IsEmpty() const {
        return m_itemCount == 0;
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::IsFull() const {
        return m_itemCount == Max;
    }
    
    
    template<typename T, int Max>
    inline T& TypedefArray<T, Max>::operator [] (unsigned int i) {
        return m_list[i];
    }
    
    
    template<typename T, int Max>
    inline T TypedefArray<T, Max>::operator [] (unsigned int i) const {
        return m_list[i];
    }
    
    
    template<typename T, int Max>
    inline void TypedefArray<T, Max>::Map(void (*func)(T& item)) {
        for (unsigned i = 0; i < m_itemCount; ++i) {
            T item = m_list[i];
            if (item != NULL) {
                (*func)(item);
            }
        }
    }
    
    
    template<typename T, int Max>
    inline bool TypedefArray<T, Max>::Contains(const T& item) const {
        if (item == NULL) {
            return false;
        }

        if (m_itemCount == 0) {
            return false;
        }
        
        unsigned i = 0;
        bool b = false;
        while (i < m_itemCount) {
            if (item == m_list[i]) {
                b = true;
                break;
            }
            ++i;
        }
        return b;
    }
}

#endif
