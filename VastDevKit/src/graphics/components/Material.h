#ifndef _VAST_GRAPHICS_COMPONENT_MATERIAL_H_
#define _VAST_GRAPHICS_COMPONENT_MATERIAL_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    class Texture;
    
    namespace graphics {
        class Material : public Component {
            
        private:

            Texture *m_colorTexture;

            Texture *m_normalTexture;

            Texture *m_alphaMaskTexture;

            Texture *m_bumpTexture;
            
            Color4f m_ambient;
            
            Color4f m_diffuse;
            
            Color4f m_specular;
            
            Color4f m_emission;

            Color4f m_ambientAdjustment;
            
            float m_shininess;

            
        protected:
            
            friend class ComponentPool<Material>;
            
            Material(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetTexture(Texture *texture);
            
            const Texture * GetTexture() const;
            
            hd::Texture * GetColorTexture() const;
            
            hd::Texture * GetNormalTexture() const;
            
            hd::Texture * GetAlphaMaskTexture() const;
            
            hd::Texture * GetBumpTexture() const;
            
            void SetColorTexture(Texture *texture);
            
            void SetNormalTexture(Texture *texture);
            
            void SetAlphaMaskTexture(Texture *texture);
            
            void SetBumpTexture(Texture *texture);
            
            void SetColor(const Color4f &color);
            
            const Color4f & GetAmbient() const;
            
            void SetAmbient(const Color4f &ambient);
            
            const Color4f & GetDiffuse() const;
            
            void SetDiffuse(const Color4f &diffuse);
            
            const Color4f & GetSpecular() const;
            
            void SetSpecular(const Color4f &specular);
            
            const Color4f & GetEmission() const;
            
            void SetEmission(const Color4f &emission);

            const Color4f & GetAmbientAdjustment() const;

            void SetAmbientAdjustment(const Color4f &ambient);
            
            float GetShininess() const;
            
            void SetShininess(const float shininess);
            
            void Render() const;
            
        };
    }
}

#endif