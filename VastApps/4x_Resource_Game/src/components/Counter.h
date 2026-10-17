#ifndef _VAST_BUILDER_COUNTER_H_
#define _VAST_BUILDER_COUNTER_H_

#include "core/component/Component.h"
#include "core/component/ComponentPool.h"

namespace hd {
    template<class T> class ComponentPool;
}

///
/// App specific counter for storing counts.
///
class Counter : public hd::Component {
private:

    uint32_t m_value;


protected:

    friend class hd::ComponentPool<Counter>;

    Counter(unsigned int ID);


public:

    uint32_t GetValue() const;

    void Increment();

    void Decrement();

};

#endif