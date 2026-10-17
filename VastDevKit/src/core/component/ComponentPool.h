#ifndef _VAST_COMPONENT_POOL_H_
#define _VAST_COMPONENT_POOL_H_

#include "core/collections/Vector.h"
#include "core/component/Component.h"
#include "core/component/AbstractComponentPool.h"

///
/// Pull a component off the "available" pool and put into the
/// "used" elements pool.  When the component is
/// destroyed, it must be returned to the available pool.
/// Create a component is as simple as pulling a component from the top
/// of the "available" pool and returning the reference, while inserting
/// into the used" pool.
///
namespace hd {
    template<class T>
    class ComponentPool : public AbstractComponentPool {
    private:

        hd::Vector<T *> * m_items;

        hd::Vector<T *> * m_freeItems;

        uint32_t m_capacity;

        T * m_nullComponent;

        bool m_debugMessages;


    public:

        ComponentPool(const unsigned int capacity);

        ~ComponentPool() {
            hdDeleteSafely(m_items);
            hdDeleteSafely(m_freeItems);
        }

        T * Get();

        bool Free(Component *component);

        void FreeAll();

        bool Available();

        bool IsNull(T * component);

        void PrintDebugInfo();

        void EnableDebugMessages();

        void DisableDebugMessages();

    };
}


template<class T>
hd::ComponentPool<T>::ComponentPool(const unsigned int capacity) :
  AbstractComponentPool(),
  m_items(nullptr),
  m_freeItems(nullptr),
  m_capacity(capacity),
  m_debugMessages(false) {

      m_items = new hd::Vector<T *>(capacity);
      m_freeItems = new hd::Vector<T *>(capacity);

      T *components;
      if (NULL == (components = (T *)calloc(1, sizeof(T) * m_capacity))) {
          return;
      }

      for (uint32_t i = 0; i < m_capacity; ++i) {
          T *component = new (&components[i]) T(i);
          component->SetComponentPool(this);
          m_items->Add(component);
          m_freeItems->Add(component);
      }

      m_nullComponent = new T(NullComponentIdentifier);
      m_nullComponent->SetComponentPool(this);

      PrintDebugInfo();
}


template<class T>
bool hd::ComponentPool<T>::Available() {
    return !m_freeItems->empty();
}


template<class T>
bool hd::ComponentPool<T>::IsNull(T * component) {
    return component == m_nullComponent;
}


template<class T>
T * hd::ComponentPool<T>::Get() {
    hdAssert(m_freeItems->Size() <= m_items->Size());

    if (m_freeItems->IsEmpty()) {
        T *components;
        uint32_t resize = hd::max(uint32_t(m_capacity), uint32_t(8));
        hd::log::w("The component pool ran out of space! Allocating more space for %d components...\n", resize);

        if (NULL == (components = (T *)calloc(1, sizeof(T) * resize))) {
            hd::log::w("Null component was returned.  All components in this pool are now in use\n");
            m_nullComponent->Reset();
            return m_nullComponent;
        }

        m_capacity += resize;
        for (uint32_t i = 0; i < resize; ++i) {
            T *component = new (&components[i]) T(i);
            component->SetComponentPool(this);
            m_items->Add(component);
            m_freeItems->Add(component);
        }

        hdAssert(m_items->Size() == m_capacity);
        hdAssert(m_freeItems->Size() == resize);
    }

    T *freeItem = m_freeItems->Top();
    m_freeItems->Pop();

    hdAssert(freeItem->IsFree());

    freeItem->SetBusy();

    return freeItem;
}


template<class T>
bool hd::ComponentPool<T>::Free(Component *usedComponent) {
    T *component = static_cast<T *>(usedComponent);

    hdAssert(m_freeItems->Size() <= m_items->Size());

    ///
    /// HACK HACK HACK
    ///
    /// m_nullComponent check does not seem to work in all cases!!!  Check component ID as
    /// a temporary fix, because I can't figure out why the frame rate slows to a crawl once we
    /// begin returning m_nullComponent.
    ///
    /// UPDATE: I can now! It's because certain components have functions that accumulate data; the
    /// VertexBuffer component is one of these.  We return a single null component, but there are
    /// no further checks on that component.  The VertexBuffer component has an AddMesh function
    /// that accumulates vertex data.  Multiple entities contain a reference to this null
    /// component and thus the component can rapidly acquire a huge amount of vertex data, slowing
    /// the frame rate when it comes to render the data.
    ///
    /// There are several concurrent solutions to this:
    /// - Components are not allowed to have side effects -- memory is allocated in static or global
    ///   functions and then assigned to the entity.  Components can store a reference to the memory
    ///   but nothing else.  TextureManager is a good example of this.
    /// - (DONE) Component pools grow on Get() so that we effectively never run out of components.
    /// - We can also return a new heap allocated component when we run out of pool space.
    ///   This presents problems when it comes to serialization, however.
    ///
    if (component == nullptr || component == m_nullComponent || usedComponent->IsNullComponent()) {
        hd::log::i("Can't free a null component.\n");
        return false;
    }

    if (m_freeItems->Size() == m_items->Size()) {
        return false;
    }

    if (component < m_items->At(0) ||
        component > m_items->At(m_items->Size() - 1)) {
        return false;
    }

    hdAssert(component->IsFree());
    component->Reset();

    m_freeItems->Add(component);

    return true;
}


template<class T>
void hd::ComponentPool<T>::FreeAll() {
    m_freeItems->Clear();

    for (uint32_t i = 0; i < m_items->Size(); ++i) {
        T *component = m_items->At(i);
        component->Reset();
        component->Free();
        m_freeItems->Add(component);
    }
}


template<class T>
void hd::ComponentPool<T>::PrintDebugInfo() {
    hd::log::i("%d items on the available pool.  Total memory: %d bytes\n",
               m_items->GetSize(), int(m_capacity * sizeof(T)) + int(m_capacity * sizeof(T *)));
}


template<class T>
void hd::ComponentPool<T>::EnableDebugMessages() {
    m_debugMessages = true;
}


template<class T>
void hd::ComponentPool<T>::DisableDebugMessages() {
    m_debugMessages = false;
}

#endif
