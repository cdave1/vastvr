#include "ShaderLoader.h"
#include <Vast/VastCore.h>
#include "ShaderProgram.h"
#include "graphics/GL/GraphicsInterface.h"
#include "core/filesystem/FileSystem.h"

static const uint32_t MISSING_SHADER_ID = 0;

hd::graphics::ShaderLoader *s_instance = NULL;

hd::graphics::ShaderLoader * hd::graphics::ShaderLoader::Instance() {
    if (!s_instance) {
        s_instance = new hd::graphics::ShaderLoader();
    }
    return s_instance;
}


hd::graphics::ShaderProgram * hd::graphics::ShaderLoader::Get(const std::string &vertexShaderName,
                                                              const std::string &vertexShader,
                                                              const std::string &fragmentShaderName,
                                                              const std::string &fragmentShader) {
    if (vertexShaderName.empty()
        || vertexShader.empty()) {
        return NULL;
    }

    for (unsigned i = 0; i < m_shaderPrograms->Size(); i++) {
        hd::graphics::ShaderProgram *program = m_shaderPrograms->At(i);

        if (program->ContainsShaderSource(vertexShaderName) &&
            program->ContainsShaderSource(fragmentShaderName)) {
            return program;
        }
    }

    uint32_t programId = 0;
    uint32_t vertexShaderId = LoadShaderData(vertexShaderName, vertexShader, GL_VERTEX_SHADER);
    if (vertexShaderId == MISSING_SHADER_ID) {
        return NULL;
    } else {
        programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glDeleteShader(vertexShaderId);
    }

    if (fragmentShaderName.length() > 0 && fragmentShader.length() > 0) {
        uint32_t fragmentShaderId = LoadShaderData(fragmentShaderName, fragmentShader, GL_FRAGMENT_SHADER);
        if (fragmentShaderId == MISSING_SHADER_ID) {
            return NULL;
        } else {
            glAttachShader(programId, fragmentShaderId);
            glDeleteShader(fragmentShaderId);
        }
    }

    if (Link(programId)) {
        hd::graphics::ShaderProgram *program = new hd::graphics::ShaderProgram(programId);
        m_shaderPrograms->Add(program);
        program->AddShaderSource(vertexShaderName, "");
        program->AddShaderSource(fragmentShaderName, "");
        return program;
    } else {
        return NULL;
    }
}


hd::graphics::ShaderProgram * hd::graphics::ShaderLoader::Get(const std::string &vertexShaderFile,
                                                              const std::string &fragmentShaderFile) {
    if (vertexShaderFile.empty() || fragmentShaderFile.empty()) {
        return NULL;
    }
    
    for (unsigned i = 0; i < m_shaderPrograms->Size(); i++) {
        hd::graphics::ShaderProgram *program = m_shaderPrograms->At(i);
            
        if (program->ContainsShaderSource(vertexShaderFile) &&
            program->ContainsShaderSource(fragmentShaderFile)) {
            return program;
        }
    }

    std::string vertexShaderData = GetShaderFileContents(vertexShaderFile);
    if (vertexShaderData.length() == 0) {
        return NULL;
    }

    std::string fragmentShaderData = GetShaderFileContents(fragmentShaderFile);
    if (fragmentShaderData.length() == 0) {
        return NULL;
    }

    uint32_t vertexShaderId = LoadShaderData(vertexShaderFile, vertexShaderData, GL_VERTEX_SHADER);
    if (vertexShaderId == MISSING_SHADER_ID) {
        return NULL;
    }
    
    uint32_t fragmentShaderId = LoadShaderData(fragmentShaderFile, fragmentShaderData, GL_FRAGMENT_SHADER);
    if (fragmentShaderId == MISSING_SHADER_ID) {
        return NULL;
    }
    
    uint32_t programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    
    if (Link(programId)) {
        hd::graphics::ShaderProgram *program = new hd::graphics::ShaderProgram(programId);
        m_shaderPrograms->Add(program);
        program->AddShaderSource(vertexShaderFile, "");
        program->AddShaderSource(fragmentShaderFile, "");
        return program;
    } else {
        return NULL;
    }
}


hd::graphics::ShaderLoader::ShaderLoader() : m_shaderLocation() {
    m_shaderPrograms = new hd::Vector<ShaderProgram *>();
}


hd::graphics::ShaderLoader::~ShaderLoader() {
}


std::string hd::graphics::ShaderLoader::GetShaderFileContents(const std::string &file) {
    if (file.empty()) {
        return NULL;
    }

    std::string path = m_shaderLocation + file;
    hd::File *shaderFile = NULL;
    if (NULL == (shaderFile = hd::FileSystem::OpenFile(path))) {
        hd::log::i("[ShaderProgram]: Could not open (%s) for reading. Couldn't find file\n", path.c_str());
        return "";
    }

    char *buffer = NULL;
    int len = hd::FileSystem::GetFileSize(shaderFile);
    if (NULL == (buffer = (char *)calloc(1, sizeof(char) * (len + 1)))) {
        hd::log::e("CALLOC!\n");
        hd::FileSystem::CloseFile(shaderFile);
        return "";
    }

	hd::FileSystem::ReadFile(buffer, len, 1, shaderFile);
    std::string data;
    data.append(buffer);
    free(buffer);
    
    hd::FileSystem::CloseFile(shaderFile);
    return data;
}


uint32_t hd::graphics::ShaderLoader::LoadShaderData(const std::string &path,
                                                    const std::string &data,
                                                    const unsigned int type) {
    GLuint shaderId;
    GLint status;
    const GLchar *source = (GLchar *)data.c_str();
    
    shaderId = glCreateShader((GLenum)type);
    if (glIsShader(shaderId) == GL_FALSE) {
        hd::log::e("Shader creation failed at glCreateShader: %s %s\n", path.c_str());
        return 0;
    }
    
    glShaderSource(shaderId, 1, &source, NULL);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char *log;
        GLint length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            log = (char *)calloc(1, length);
            glGetShaderInfoLog(shaderId, length, NULL, log);
            hd::log::e("Shader error: %s %s\n", path.c_str(), log);
            hdFreeSafely(log);
        }
    } else {
		hd::log::e("Shader loaded ok: %s\n", path.c_str());
	}
    
    if (status == GL_FALSE ) {
        glDeleteShader(shaderId);
        return 0;
    } else {
        return shaderId;
    }
}


bool hd::graphics::ShaderLoader::Link(const uint32_t programId) const {
    GLint status;
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    
    GLint length;
    char *log;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        log = (char *)calloc(1, length);
        glGetProgramInfoLog(programId, length, &status, log);
        hd::log::e("%s\n", log);
        hdFreeSafely(log);
    }
    
    if (status == GL_FALSE) {
        glDeleteProgram(programId);
        return false;
    } else {
        return true;
    }
}

