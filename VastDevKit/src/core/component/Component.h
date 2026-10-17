#ifndef _VAST_COMPONENT_H_
#define _VAST_COMPONENT_H_

#include <limits.h>

typedef uint32_t ComponentIdentifier;

typedef uint32_t ComponentTypeValue;

/**
 * Abstract component
 */
namespace hd {
    extern const ComponentIdentifier NullComponentIdentifier;

    class AbstractComponentPool;
    
    class Component {
    private:
        
        ComponentIdentifier m_ID;
        
        AbstractComponentPool *m_componentPool;

        bool m_isFree;
        
        
    public:
        
        ComponentIdentifier GetID() const;
        
        void PrintDebugInfo() const;
        
        void const Free();
        
        bool IsFree() const;

        bool IsNullComponent() const;
        
        
    protected:
        
        friend class AbstractComponentPool;
        
        Component(ComponentIdentifier ID);
        
        ~Component();
        
        void Reset();
        
        void SetComponentPool(AbstractComponentPool *componentPool);
        
        void SetBusy();
        
    };

    
    class ComponentType {
        static ComponentTypeValue typeValueCounter;

    public:
        template<typename T>
        static inline ComponentTypeValue value() {
            static ComponentTypeValue v = typeValueCounter++;
            return v;
        }
    };
}

#endif
