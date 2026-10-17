#ifndef _VAST_GRAPHICS_LIGHT_H_
#define _VAST_GRAPHICS_LIGHT_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

//
// Light components do not store position information.  Assume the position is
// 0,0,0.
//
namespace hd {
    namespace graphics {
        class Light : public Component {
            
        private:
            Vec3 m_position;

            Vec3 m_spotDirection;
            
            float m_spotExponent;
            
            float m_spotCutoff; // in radians
            
            float m_constantAttenuation;
            
            float m_linearAttenuation;
            
            float m_quadraticAttenuation;
            
            bool m_isDirectional;
            
            Color4f m_ambient;
            
            Color4f m_diffuse;
            
            Color4f m_specular;
            
            
        protected:
            
            friend class ComponentPool<Light>;
            
            Light(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            // Getters
            const Vec3 & GetPosition() const;

            const Vec3 & GetSpotDirection() const;
            
            float GetSpotExponent() const;
            
            float GetSpotCutoff() const;
            
            float GetConstantAttenuation() const;
            
            float GetLinearAttenuation() const;
            
            float GetQuadraticAttenuation() const;
            
            bool IsDirectional() const;
            
            const Color4f & GetAmbient() const;
            
            const Color4f & GetDiffuse() const;
            
            const Color4f & GetSpecular() const;
            
            // Setters
            void SetPosition(const Vec3 &position);

            void SetSpotDirection(const Vec3 &direction);
            
            void SetSpotExponent(const float exponent);
            
            void SetSpotCutoff(const float cutoff);
            
            void SetConstantAttenuation(const float constantAttenuation);
            
            void SetLinearAttenuation(const float linearAttenuation);
            
            void SetQuadraticAttenuation(const float quadraticAttenuation);
            
            void SetIsDirectional(bool isDirectional);
            
            void SetAmbient(const Color4f &ambient);
            
            void SetDiffuse(const Color4f &diffuse);
            
            void SetSpecular(const Color4f &specular);
            
        };
    }
}

#endif