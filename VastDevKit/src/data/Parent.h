#ifndef _VAST_DATA_PARENT_H_
#define _VAST_DATA_PARENT_H_

#include "core/component/Component.h"
#include <Vast/VastCore.h>

template<class T> class ComponentPool;

namespace hd {
    namespace data {
        class Parent : public Component {
        private:

            EntityIdentifier m_id;

            hd::Entity * m_entity;



        protected:

            friend class ComponentPool<Parent>;

            Parent(unsigned int ID);
            
            
        public:
            
            void Reset();

            void SetId(EntityIdentifier id);

            void SetEntity(hd::Entity *entity);

            EntityIdentifier GetId() const;

            hd::Entity * GetEntity() const;
            
        };
    }
}

#endif
