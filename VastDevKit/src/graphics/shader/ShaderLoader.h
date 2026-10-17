#ifndef _VAST_GRAPHICS_SHADER_LOADER_H_
#define _VAST_GRAPHICS_SHADER_LOADER_H_

#include <string>

#include "core/collections/Collections.h"

namespace hd {
    namespace graphics {
        class ShaderProgram;
        
        class ShaderLoader {
        private:
            
            hd::Vector<ShaderProgram *> *m_shaderPrograms;

            std::string m_shaderLocation;

            
        public:
            
            static ShaderLoader * Instance();

            hd::graphics::ShaderProgram * Get(const std::string &vertexShaderName,
                                              const std::string &vertexShader,
                                              const std::string &fragmentShaderName,
                                              const std::string &fragmentShader);

            hd::graphics::ShaderProgram * Get(const std::string &vertexShaderFile,
                                              const std::string &fragmentShaderFile);

            void SetShaderLocation(const std::string location) { m_shaderLocation = location; }
            
        protected:
            
            ShaderLoader();
            
            ~ShaderLoader();
            
            
        private:

            std::string GetShaderFileContents(const std::string &path);
            
            uint32_t LoadShaderData(const std::string &file,
                                    const std::string &data,
                                    const unsigned int type);
            
            bool Link(const uint32_t programId) const;
            
        };
    }
}


#endif
