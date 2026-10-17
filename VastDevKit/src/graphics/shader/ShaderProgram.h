#ifndef _VAST_GRAPHICS_SHADER_PROGRAM_H_
#define _VAST_GRAPHICS_SHADER_PROGRAM_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "core/collections/Collections.h"

namespace hd {
    class Texture;
    
    namespace graphics {
        class ShaderProgram {
        private:
            
            std::unordered_map<std::string, std::string> m_shaderSources;

            mutable std::unordered_map<const char *, int32_t> m_uniformLocations;

            mutable std::unordered_map<const char *, int32_t> m_attributeLocations;

            int32_t m_uniformLocationIndex[64];

            uint32_t m_uniformIndexSize;
            
            uint32_t m_programId;

            mutable uint32_t m_uniformIndex;
            
            
        public:
            
            std::string GetPath() const;
            
            uint32_t GetProgramId() const;
            
            bool ContainsShaderSource(const std::string &path) const;

            int32_t GetAttributeLocation(const char *name) const;

            int32_t GetUniformLocation(const char *name) const;

            bool CheckAttribute(const std::string &name, const int32_t location) const;

            bool CheckUniform(const std::string &name, const int32_t location) const;

            void SetUniformIndex(const std::vector<std::string> &names);

            int32_t GetUniformLocationForIndex(const uint32_t index);

            void Load() const;
            

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
            
            void BindIndexTexture(const hd::Texture *texture);
            
            
        private:
            
            friend class ShaderLoader;
            
            ShaderProgram();
            
            ShaderProgram(uint32_t programId);
            
            ~ShaderProgram();
            
            void AddShaderSource(const std::string &path,
                                 const std::string &source);
            
        };
    }
}


#endif
