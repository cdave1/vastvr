#ifndef _VAST_GRAPHICS_COMPONENT_SHADER_H_
#define _VAST_GRAPHICS_COMPONENT_SHADER_H_

#include <string>
#include <map>

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    class Texture;
    
    namespace graphics {
        extern const char * DEPTHPASS_FRAGMENT_SHADER;
        extern const char * DEPTHPASS_VERTEX_SHADER;

        extern const char * OPACITY_VALUE_UNIFORM;
        extern const char * WORLD_MATRIX_UNIFORM;
        extern const char * VIEW_MATRIX_UNIFORM;
        extern const char * CAMERA_POSITION_UNIFORM;
        extern const char * PROJECTION_MATRIX_UNIFORM;
        extern const char * VIEW_PROJECTION_MATRIX_UNIFORM;
        extern const char * WORLD_VIEW_PROJECTION_MATRIX_UNIFORM;

        extern const char * TRANSFORM_ATTRIBUTE_NAME;

        extern const char * LIGHT_POSITION_UNIFORM;
        extern const char * LIGHT_AMBIENT_COLOR_UNIFORM;
        extern const char * LIGHT_DIFFUSE_COLOR_UNIFORM;
        extern const char * LIGHT_SPECULAR_COLOR_UNIFORM;

        extern const char * MATERIAL_AMBIENT_COLOR_UNIFORM;
        extern const char * MATERIAL_DIFFUSE_COLOR_UNIFORM;
        extern const char * MATERIAL_SPECULAR_COLOR_UNIFORM;
        extern const char * MATERIAL_SHININESS_UNIFORM;
        
        
        class ShaderProgram;
        
        class Shader : public Component {
        private:
            
            ShaderProgram *m_shaderProgram;
            
            std::function<void(Shader *)> m_preRenderCallback;
            
            std::function<void(Shader *)> m_postRenderCallback;
            
            std::string m_name;
            
            unsigned int m_textureCount;
            
            
        protected:
            
            friend class ComponentPool<Shader>;
            
            Shader(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            uint32_t GetProgram() const;
            
            bool LoadShader(const std::string &vertexShaderPath,
                            const std::string &fragmentShaderPath);

            void SetShaderProgram(hd::graphics::ShaderProgram *program);
            
            void Begin();
            
            void End();
            
            template<class T>
            void SetPreRenderCallback(T *src, void (T::*preRenderCallback)(Shader *shader));
            
            void SetAttribute1f(const char *name, const float a);


#pragma mark - Uniforms with strings

            void SetUniform1i(const char *name, const int value);
            
            void SetUniform1f(const char *name, const float value) const;
            
            void SetUniform1fv(const char *name, const unsigned count, const float *val);
            
            void SetUniform2f(const char *name, const float v1, const float v2);
            
            void SetUniform3f(const char *name, const Vec3 &val);
            
            void SetUniform3fv(const char *name, const unsigned count, const Vec3 *val);
            
            void SetUniform4f(const char *name,
                              const float v1, const float v2,
                              const float v3, const float v4) const;
            
            void SetUniform4f(const char *name, const Vec4 &val) const;

            void SetUniform4f(const char *name, const Color4f &color) const;
            
            void SetUniformMatrix(const char *name,
                                  const Matrix &matrix) const;
            
            void BindTexture(const char *name, const Texture *texture);

            void BindAttributeLocation(const char *name) const;

            int32_t GetAttributeLocation(const char *name) const;

            int32_t GetUniformLocation(const char *name) const;


#pragma mark - Uniforms with direct positions

            void SetAttribute1f(const int32_t location, const float a);

            void SetUniform1i(const int32_t location, const int value);

            void SetUniform1f(const int32_t location, const float value) const;

            void SetUniform1fv(const int32_t location, const unsigned count, const float *val);

            void SetUniform2f(const int32_t location, const float v1, const float v2);

            void SetUniform3f(const int32_t location, const Vec3 &val);

            void SetUniform3fv(const int32_t location, const unsigned count, const Vec3 *val);

            void SetUniform4f(const int32_t location,
                              const float v1, const float v2,
                              const float v3, const float v4) const;

            void SetUniform4f(const int32_t location, const Vec4 &val) const;

            void SetUniform4f(const int32_t location, const Color4f &color) const;

            void SetUniformMatrix(const int32_t location,
                                  const Matrix &matrix) const;

            void BindTexture(const int32_t location, const Texture *texture);


#pragma mark - Uniforms with indexes

            void SetIndexUniform1i(const int value);

            void SetIndexUniform1f(const float value);

            void SetIndexUniform1fv(const unsigned count, const float *val);

            void SetIndexUniform2f(const float v1, const float v2);

            void SetIndexUniform3f(const Vec3 &val);

            void SetIndexUniform3fv(const unsigned count, const Vec3 *val);

            void SetIndexUniform4f(const float v1, const float v2,
                              const float v3, const float v4);

            void SetIndexUniform4f(const Vec4 &val);

            void SetIndexUniform4f(const Color4f &color);

            void SetIndexUniformMatrix(const Matrix &matrix);
            
            void BindIndexTexture(const Texture *texture);
            
        };
    }
}

template<class T>
void hd::graphics::Shader::SetPreRenderCallback(T *src, void (T::*preRenderCallback)(Shader *shader)) {
    m_preRenderCallback = std::bind(preRenderCallback, src, std::placeholders::_1);
}

#endif