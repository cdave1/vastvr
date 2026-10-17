#ifndef _VAST_DATA_CHILDREN_H_
#define _VAST_DATA_CHILDREN_H_

#include "core/component/Component.h"
#include <Vast/VastCore.h>

template<class T> class ComponentPool;

namespace hd {
    namespace data {
        class Children : public Component {
        private:

            hd::Vector<EntityIdentifier> m_children;

            hd::Vector<hd::Entity *> * m_childrenCache;


        protected:

            friend class ComponentPool<Children>;

            Children(unsigned int ID);


        public:

            void Reset();

            void SetChildren();

            hd::Vector<EntityIdentifier> GetChildren() const;
            
            hd::Vector<hd::Entity *> * GetCache() const;
            
        };
    }
}

#endif
