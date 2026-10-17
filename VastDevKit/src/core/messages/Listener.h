/**
 * Encapsulates a function pointer and a target instance.
 */

#ifndef _VAST_LISTENER_H_
#define _VAST_LISTENER_H_

#include "AbstractListener.h"

namespace hd {
template<class T>
    class Listener : AbstractListener {
    public:
        
        Listener(const char * name) : AbstractListener() {
            m_action = NULL;
            m_staticAction = NULL;
            m_listener = NULL;
            memset(m_name, 0, sizeof(m_name));
            snprintf(m_name, sizeof(m_name), "%s", name);
        }
        
        const char * GetName() const;
        
        void Call(hd::MessageData data);
        
        void SetAction(void (T::*func)(hd::MessageData data));
        
        void SetStaticAction(void (*func)(hd::MessageData data));
        
        void SetListener(const T * listener);
        
        bool ContainsListener(const void * listener) const;
        
        
    private:
        
        void (T::*m_action)(hd::MessageData data);
        
        void (*m_staticAction)(hd::MessageData data);
        
        T * m_listener;
        
        char m_name[64];
        
    };
}


template<class T>
inline const char * hd::Listener<T>::GetName() const {
    return m_name;
}


template<class T>
inline void hd::Listener<T>::SetAction(void (T::*func)(hd::MessageData data)) {
    m_action = func;
}


template<class T>
inline void hd::Listener<T>::SetStaticAction(void (*func)(hd::MessageData data)) {
    m_staticAction = func;
}


template<class T>
inline void hd::Listener<T>::SetListener(const T *listener) {
    if((void *)listener == (void *)this) {
        hd::log::e("Cannot subscribe to yourself (%p)\n", listener);
        m_listener = NULL;
        return;
    }
    m_listener = (T*)listener;
}


template<class T>
void hd::Listener<T>::Call(hd::MessageData data) {
    if (m_listener) {
        (m_listener->*m_action)(data);
    } else {
        (*m_staticAction)(data);
    }
}


template<class T>
bool hd::Listener<T>::ContainsListener(const void *listener) const {
    return m_listener == listener;
}

#endif