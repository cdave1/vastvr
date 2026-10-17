#include "Shader.h"
#include "graphics/GL/GraphicsInterface.h"
#include "graphics/shader/ShaderLoader.h"
#include "graphics/shader/ShaderProgram.h"

using namespace hd;
using namespace hd::graphics;

const char * hd::graphics::DEPTHPASS_FRAGMENT_SHADER = "DepthPassFragmentShader";
const char * hd::graphics::DEPTHPASS_VERTEX_SHADER = "DepthPassVertexShader";

const char * hd::graphics::TRANSFORM_ATTRIBUTE_NAME = "iTransform";
const char * hd::graphics::OPACITY_VALUE_UNIFORM = "Opacity";

const char * hd::graphics::WORLD_MATRIX_UNIFORM = "WorldMatrix";
const char * hd::graphics::CAMERA_POSITION_UNIFORM = "CameraWorldPosition";
const char * hd::graphics::VIEW_MATRIX_UNIFORM = "ViewMatrix";
const char * hd::graphics::PROJECTION_MATRIX_UNIFORM = "ProjectionMatrix";
const char * hd::graphics::VIEW_PROJECTION_MATRIX_UNIFORM = "ViewProjectionMatrix";
const char * hd::graphics::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM = "WorldViewProjectionMatrix";

const char * hd::graphics::LIGHT_POSITION_UNIFORM = "LightPosition";
const char * hd::graphics::LIGHT_AMBIENT_COLOR_UNIFORM = "LightAmbientColor";
const char * hd::graphics::LIGHT_DIFFUSE_COLOR_UNIFORM = "LightDiffuseColor";
const char * hd::graphics::LIGHT_SPECULAR_COLOR_UNIFORM = "LightSpecularColor";

const char * hd::graphics::MATERIAL_AMBIENT_COLOR_UNIFORM = "iMaterialAmbientColor";
const char * hd::graphics::MATERIAL_DIFFUSE_COLOR_UNIFORM = "iMaterialDiffuseColor";
const char * hd::graphics::MATERIAL_SPECULAR_COLOR_UNIFORM = "iMaterialSpecularColor";
const char * hd::graphics::MATERIAL_SHININESS_UNIFORM = "iMaterialShininess";



Shader::Shader(unsigned int ID) : Component(ID),
m_shaderProgram(nullptr),
    m_preRenderCallback(nullptr),
	m_postRenderCallback(nullptr),
    m_name(),
    m_textureCount(0)
{}


void Shader::Reset() {
	m_shaderProgram = nullptr;
}


uint32_t Shader::GetProgram() const {
    return m_shaderProgram->GetProgramId();
}


bool Shader::LoadShader(const std::string &vertexShaderPath,
                        const std::string &fragmentShaderPath) {
    hd::graphics::ShaderLoader *shaderLoader = hd::graphics::ShaderLoader::Instance();
    hd::graphics::ShaderProgram *program = shaderLoader->Get(vertexShaderPath, fragmentShaderPath);
    m_shaderProgram = program;
    m_name = vertexShaderPath + ", " + fragmentShaderPath;
    return m_shaderProgram != NULL;
}


void Shader::SetShaderProgram(hd::graphics::ShaderProgram *program) {
    m_shaderProgram = program;
    m_name = m_shaderProgram->GetPath();
}


void Shader::Begin() {
    if (m_shaderProgram) {
        m_shaderProgram->Load();
    }
    if (m_preRenderCallback) {
        m_preRenderCallback(this);
    }
}


void Shader::End() {
    if (m_postRenderCallback) {
        m_postRenderCallback(this);
    }
    
//    glUseProgram(0);
    
    unsigned int count = 0;
    while (count++ < m_textureCount) {
        hd::gl::PopActiveTexture();
        hd::gl::BindTexture(NULL);
    }
    m_textureCount = 0;
}


void Shader::SetUniform1i(const char *name, const int value) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1i(name, value);
}


void Shader::SetUniform1f(const char *name, const float value) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1f(name, value);
}


void Shader::SetUniform1fv(const char *name, const unsigned count, const float *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1fv(name, count, val);
}


void Shader::SetUniform2f(const char *name, const float v1, const float v2) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform2f(name, v1, v2);
}


void Shader::SetUniform3f(const char *name, const Vec3 &val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform3f(name, val);
}


void Shader::SetUniform3fv(const char *name, const unsigned count, const Vec3 *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform3fv(name, count, val);
}


void Shader::SetUniform4f(const char *name,
                          const float v1, const float v2,
                          const float v3, const float v4) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(name, v1, v2, v3, v4);
}


void Shader::SetUniform4f(const char *name,
                          const Vec4 &val) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(name, val);
}


void Shader::SetUniform4f(const char *name, const Color4f &color) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(name, color);
}


void Shader::SetUniformMatrix(const char *name,
                              const Matrix &matrix) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniformMatrix(name, matrix);
}


void Shader::BindTexture(const char *name, const Texture *texture) {
    hdAssert(m_shaderProgram);
    // TODO: return bool from this function.
    m_shaderProgram->BindTexture(name, texture);
    {
        m_textureCount++;
    }
}


void Shader::BindAttributeLocation(const char *name) const {
    m_shaderProgram->BindAttributeLocation(name);
}


int32_t Shader::GetAttributeLocation(const char *name) const {
    return m_shaderProgram->GetAttributeLocation(name);
}


int32_t Shader::GetUniformLocation(const char *name) const {
    return m_shaderProgram->GetUniformLocation(name);
}


#pragma mark - Uniforms with direct positions

void Shader::SetUniform1i(const int32_t location, const int value) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1i(location, value);
}


void Shader::SetUniform1f(const int32_t location, const float value) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1f(location, value);
}


void Shader::SetUniform1fv(const int32_t location, const unsigned count, const float *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform1fv(location, count, val);
}


void Shader::SetUniform2f(const int32_t location, const float v1, const float v2) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform2f(location, v1, v2);
}


void Shader::SetUniform3f(const int32_t location, const Vec3 &val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform3f(location, val);
}


void Shader::SetUniform3fv(const int32_t location, const unsigned count, const Vec3 *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform3fv(location, count, val);
}


void Shader::SetUniform4f(const int32_t location,
                          const float v1, const float v2,
                          const float v3, const float v4) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(location, v1, v2, v3, v4);
}


void Shader::SetUniform4f(const int32_t location,
                          const Vec4 &val) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(location, val);
}


void Shader::SetUniform4f(const int32_t location, const Color4f &color) const {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetUniform4f(location, color);
}


void Shader::SetUniformMatrix(const int32_t location,
                              const Matrix &matrix) const {
    m_shaderProgram->SetUniformMatrix(location, matrix);
}


void Shader::BindTexture(const int32_t location, const Texture *texture) {
    m_shaderProgram->BindTexture(location, texture);
    m_textureCount++;
}



void Shader::SetIndexUniform1i(const int value) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform1i(value);
}


void Shader::SetIndexUniform1f(const float value) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform1f(value);
}


void Shader::SetIndexUniform1fv(const unsigned count, const float *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform1fv(count, val);
}


void Shader::SetIndexUniform2f(const float v1, const float v2) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform2f(v1, v2);
}


void Shader::SetIndexUniform3f(const Vec3 &val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform3f(val);
}


void Shader::SetIndexUniform3fv(const unsigned count, const Vec3 *val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform3fv(count, val);
}


void Shader::SetIndexUniform4f(const float v1, const float v2,
                               const float v3, const float v4) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform4f(v1, v2, v3, v4);
}


void Shader::SetIndexUniform4f(const Vec4 &val) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniform4f(val);
}


void Shader::SetIndexUniform4f(const Color4f &color) {
    m_shaderProgram->SetIndexUniform4f(color);
}


void Shader::SetIndexUniformMatrix(const Matrix &matrix) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->SetIndexUniformMatrix(matrix);
}


void Shader::BindIndexTexture(const Texture *texture) {
    hdAssert(m_shaderProgram);
    m_shaderProgram->BindIndexTexture(texture);
}
