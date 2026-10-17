#include "ShaderProgram.h"
#include "graphics/GL/GraphicsInterface.h"

bool hd::graphics::ShaderProgram::ContainsShaderSource(const std::string &path) const {
    return m_shaderSources.count(path) > 0;
}


uint32_t hd::graphics::ShaderProgram::GetProgramId() const {
    return m_programId;
}


std::string hd::graphics::ShaderProgram::GetPath() const {
    std::string out = "";
    for (const auto &pair : m_shaderSources) {
        out = out + pair.first;
    }
    return out;
}


hd::graphics::ShaderProgram::ShaderProgram() :
    m_programId(0),
    m_shaderSources(),
    m_uniformLocations(),
    m_attributeLocations(),
    m_uniformIndexSize(0),
    m_uniformIndex(0) {}


hd::graphics::ShaderProgram::ShaderProgram(uint32_t programId) :
    m_programId(programId),
    m_shaderSources(),
    m_uniformLocations(),
    m_attributeLocations(),
    m_uniformIndexSize(0),
    m_uniformIndex(0) {}


hd::graphics::ShaderProgram::~ShaderProgram() {
}


void hd::graphics::ShaderProgram::AddShaderSource(const std::string &path,
                                                  const std::string &source) {
    m_shaderSources[path] = source;
}


int32_t hd::graphics::ShaderProgram::GetUniformLocation(const char *name) const {
    if (m_programId == 0) {
        hd::log::e("Shader program not initialized, cannot return location of %s\n", name);
        return -1;
    }

    auto iter = m_uniformLocations.find(name);
    if (iter == m_uniformLocations.end()) {
        int32_t location = glGetUniformLocation(m_programId, name);
        m_uniformLocations[name] = location;
        return location;
    } else {
        return iter->second;
    }
}


int32_t hd::graphics::ShaderProgram::GetAttributeLocation(const char *name) const {
    if (m_programId == 0) {
        hd::log::e("Shader program not initialized, cannot return location of %s\n", name);
        return -1;
    }

    auto iter = m_attributeLocations.find(name);
    if (iter == m_attributeLocations.end()) {
        int32_t location = glGetAttribLocation(m_programId, name);
        m_attributeLocations[name] = location;
        return location;
    } else {
        return iter->second;
    }
}


bool hd::graphics::ShaderProgram::CheckUniform(const std::string &name, const int32_t location) const {
    if (location < 0) {
        //hd::log::e("Uniform location \"%s\" not in Shader: \"%s\"\n", name.c_str(), m_name.c_str());
        return false;
    } else {
        return true;
    }
}


bool hd::graphics::ShaderProgram::CheckAttribute(const std::string &name, const int32_t location) const {
    if (location < 0) {
        //hd::log::e("Attribute location \"%s\" in Shader: \"%s\"\n", name.c_str(), m_name.c_str());
        return false;
    } else {
        return true;
    }
}


void hd::graphics::ShaderProgram::SetUniformIndex(const std::vector<std::string> &names) {
    uint32_t uniformIndexSize = 0;
    for (std::string name : names) {
        int32_t location = GetUniformLocation(name.c_str());
        m_uniformLocationIndex[uniformIndexSize++] = location;
    }
    m_uniformIndexSize = uniformIndexSize;
}


int32_t hd::graphics::ShaderProgram::GetUniformLocationForIndex(const uint32_t index) {
    if (index >= m_uniformIndexSize) {
        return -1;
    } else {
        return m_uniformLocationIndex[index];
    }
}


const static bool DEBUG_SHADER_STATUS = false;
static GLint currentShaderProgram = -1;

void hd::graphics::ShaderProgram::Load() const {
    m_uniformIndex = 0;
    if (DEBUG_SHADER_STATUS) {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        hdAssert(currentProgram == 0);
    }

    // Only use the shader program if its not the same as what is currently bound.
    if (currentShaderProgram != GetProgramId()) {
        glUseProgram(0);
        glUseProgram(GetProgramId());
        currentShaderProgram = GetProgramId();
    }
}


void hd::graphics::ShaderProgram::SetUniform1i(const char *name, const int value) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform1i(location, value);
    }
}


void hd::graphics::ShaderProgram::SetUniform1f(const char *name, const float value) const {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform1f(location, value);
    }
}


void hd::graphics::ShaderProgram::SetUniform1fv(const char *name, const unsigned count, const float *val) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform1fv(location, count, (const GLfloat *)val);
    }
}


void hd::graphics::ShaderProgram::SetUniform2f(const char *name, const float v1, const float v2) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform2f(location, v1, v2);
    }
}


void hd::graphics::ShaderProgram::SetUniform3f(const char *name, const Vec3 &val) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform3f(location, val[0], val[1], val[2]);
    }
}


void hd::graphics::ShaderProgram::SetUniform3fv(const char *name, const unsigned count, const Vec3 *val) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform3fv(location, count, (const GLfloat *)val);
    }
}


void hd::graphics::ShaderProgram::SetUniform4f(const char *name,
                          const float v1, const float v2,
                          const float v3, const float v4) const {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform4f(location, v1, v2, v3, v4);
    }
}


void hd::graphics::ShaderProgram::SetUniform4f(const char *name,
                          const Vec4 &val) const {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform4f(location, val[0], val[1], val[2], val[3]);
    }
}


void hd::graphics::ShaderProgram::SetUniform4f(const char *name, const Color4f &color) const {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform4f(location, color[0], color[1], color[2], color[3]);
    }
}


void hd::graphics::ShaderProgram::SetUniformMatrix(const char *name,
                              const Matrix &matrix) const {

    int32_t location = GetUniformLocation(name);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.f);
    }
}


void hd::graphics::ShaderProgram::BindTexture(const char *name, const Texture *texture) {
    int32_t location = GetUniformLocation(name);
    if (CheckUniform(name, location)) {
        glUniform1i(location, hd::gl::GetActiveTextureIndex());
        hd::gl::BindTexture(texture);
        hd::gl::PushActiveTexture();
    }
}


void hd::graphics::ShaderProgram::BindAttributeLocation(const char *name) const {
    int32_t attributeLocation = GetAttributeLocation(name);
    if (attributeLocation >= 0) {
        glBindAttribLocation(GetProgramId(), attributeLocation, name);
    }
}


#pragma mark - Uniforms with direct positions

void hd::graphics::ShaderProgram::SetUniform1i(const int32_t location, const int value) {
    glUniform1i(location, value);
}


void hd::graphics::ShaderProgram::SetUniform1f(const int32_t location, const float value) const {
    glUniform1f(location, value);
}


void hd::graphics::ShaderProgram::SetUniform1fv(const int32_t location, const unsigned count, const float *val) {
    glUniform1fv(location, count, (const GLfloat *)val);
}


void hd::graphics::ShaderProgram::SetUniform2f(const int32_t location, const float v1, const float v2) {
    glUniform2f(location, v1, v2);
}


void hd::graphics::ShaderProgram::SetUniform3f(const int32_t location, const Vec3 &val) {
    glUniform3f(location, val[0], val[1], val[2]);
}


void hd::graphics::ShaderProgram::SetUniform3fv(const int32_t location, const unsigned count, const Vec3 *val) {
    glUniform3fv(location, count, (const GLfloat *)val);
}


void hd::graphics::ShaderProgram::SetUniform4f(const int32_t location,
                          const float v1, const float v2,
                          const float v3, const float v4) const {
    glUniform4f(location, v1, v2, v3, v4);
}


void hd::graphics::ShaderProgram::SetUniform4f(const int32_t location,
                          const Vec4 &val) const {
    glUniform4f(location, val[0], val[1], val[2], val[3]);
}


void hd::graphics::ShaderProgram::SetUniform4f(const int32_t location, const Color4f &color) const {
    glUniform4f(location, color[0], color[1], color[2], color[3]);
}


void hd::graphics::ShaderProgram::SetUniformMatrix(const int32_t location,
                              const Matrix &matrix) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.f);
}


void hd::graphics::ShaderProgram::BindTexture(const int32_t location, const Texture *texture) {
    glUniform1i(location, hd::gl::GetActiveTextureIndex());
    hd::gl::BindTexture(texture);
    hd::gl::PushActiveTexture();
}


void hd::graphics::ShaderProgram::SetIndexUniform1i(const int value) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform1i(location, value);
}


void hd::graphics::ShaderProgram::SetIndexUniform1f(const float value) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform1f(location, value);
}


void hd::graphics::ShaderProgram::SetIndexUniform1fv(const unsigned count, const float *val) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform1fv(location, count, val);
}


void hd::graphics::ShaderProgram::SetIndexUniform2f(const float v1, const float v2) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform2f(location, v1, v2);
}


void hd::graphics::ShaderProgram::SetIndexUniform3f(const Vec3 &val) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform3f(location, val);
}


void hd::graphics::ShaderProgram::SetIndexUniform3fv(const unsigned count, const Vec3 *val) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform3fv(location, count, val);
}


void hd::graphics::ShaderProgram::SetIndexUniform4f(const float v1, const float v2,
                               const float v3, const float v4) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform4f(location, v1, v2, v3, v4);
}


void hd::graphics::ShaderProgram::SetIndexUniform4f(const Vec4 &val) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform4f(location, val);
}


void hd::graphics::ShaderProgram::SetIndexUniform4f(const Color4f &color) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniform4f(location, color);
}


void hd::graphics::ShaderProgram::SetIndexUniformMatrix(const Matrix &matrix) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    SetUniformMatrix(location, matrix);
}


void hd::graphics::ShaderProgram::BindIndexTexture(const Texture *texture) {
    int32_t location = GetUniformLocationForIndex(m_uniformIndex++);
    BindTexture(location, texture);
}
