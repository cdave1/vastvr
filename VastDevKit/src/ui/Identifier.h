#ifndef _VAST_UI_IDENTIFIER_H_
#define _VAST_UI_IDENTIFIER_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

namespace hd {
    namespace ui {
        class Identifier : public Component {
        private:
            
            std::string m_identifier;
            
            
        protected:
            
            friend class ComponentPool<Identifier>;
            
            Identifier(unsigned int ID);
            
            
        public:
            
            const std::string & GetId() const;
            
            void SetId(const std::string &identifier);
            
            
        };
    }
}

#endif