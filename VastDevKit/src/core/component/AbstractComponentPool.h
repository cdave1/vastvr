#ifndef _VAST_ABSTRACT_COMPONENT_POOL_H_
#define _VAST_ABSTRACT_COMPONENT_POOL_H_

namespace hd {
    class Component;
    
    class AbstractComponentPool {
    public:
        
        AbstractComponentPool() {}
        
        virtual bool Free(Component *component) = 0;
        
        virtual void FreeAll() = 0;
        
    };
}

#endif