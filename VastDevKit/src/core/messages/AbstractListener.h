#ifndef _VAST_ABSTRACT_LISTENER_H_
#define _VAST_ABSTRACT_LISTENER_H_

namespace hd {
    class MessageData {
    public:
        void *messenger;
        void *sender;
        void *payload;
    };
}

namespace hd {
    class AbstractListener {
    public:
        
        AbstractListener() {}
        
        ~AbstractListener() {}
        
        virtual void Call(hd::MessageData data) = 0;
        
        virtual bool ContainsListener(const void *listener) const = 0;
        
        virtual const char * GetName() const = 0;
        
    };
}


#endif