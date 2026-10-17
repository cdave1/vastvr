#ifndef _VAST_GRAPHICS_MESH_MATERIAL_H_
#define _VAST_GRAPHICS_MESH_MATERIAL_H_

#include <string>
#include <Vast/VastMath.h>

namespace hd {
    class Texture;
    
    namespace graphics {
        class MeshMaterial {
        private:
            
            std::string m_name;
            
            Color4f m_ambient;
            
            Color4f m_diffuse;
            
            Color4f m_specular;
            
            Color4f m_emission;
            
            float m_shininess;
            
            Texture *m_colorTexture;
            
            Texture *m_normalTexture;
            
            Texture *m_alphaMaskTexture;
            
            Texture *m_bumpTexture;
            
            
        public:
            
            const std::string & GetName() const;
            
            hd::Texture * GetColorTexture() const;
            
            hd::Texture * GetNormalTexture() const;
            
            hd::Texture * GetAlphaMaskTexture() const;
            
            hd::Texture * GetBumpTexture() const;
            
            const Color4f & GetAmbient() const;
            
            const Color4f & GetDiffuse() const;
            
            const Color4f & GetSpecular() const;
            
            const Color4f & GetEmission() const;
            
            float GetShininess() const;
            
            
        private:
            friend class Mesh;

            friend class MeshLoader;
            
            MeshMaterial();
            
            ~MeshMaterial();
            
            void SetName(const std::string &name);
            
            void SetColorTexture(Texture *texture);
            
            void SetNormalTexture(Texture *texture);
            
            void SetAlphaMaskTexture(Texture *texture);
            
            void SetBumpTexture(Texture *texture);
            
            void SetColor(const Color4f &color);
            
            void SetAmbient(const Color4f &ambient);
            
            void SetDiffuse(const Color4f &diffuse);
            
            void SetSpecular(const Color4f &specular);
            
            void SetEmission(const Color4f &emission);
            
            void SetShininess(const float shininess);
            
        };
    }
}

#endif