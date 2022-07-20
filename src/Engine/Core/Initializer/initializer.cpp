#include "initializer.h"

#include "../ECS/Component/component_manager.hpp"
#include "../ECS/Entity/entity_manager.hpp"
#include "../ECS/System/system_manager.hpp"

#include "../Rendering/RendererOpenGL/Buffers/EBO/gl_ebo.h"
#include "../Rendering/RendererOpenGL/Buffers/VAO/gl_vao.h"
#include "../Rendering/RendererOpenGL/Buffers/VBO/gl_vbo.h"
#include "../Rendering/RendererOpenGL/Shader/gl_shader.h"
#include "../Rendering/RendererOpenGL/ShaderProgram/gl_shader_program.h"

namespace DE {

using GLRenderer::GLElementBuffer;
using GLRenderer::GLShader;
using GLRenderer::GLShaderProgram;
using GLRenderer::GLVertexArray;
using GLRenderer::GLVertexBuffer;


Initializer::Initializer() {
    _max_entity_amount = 1000;
}
Initializer& Initializer::Get() {
    static Initializer initializer;
    return initializer;
}

void Initializer::SetHint(int64_t hint_id, int64_t hint_value) {
    switch (hint_id) {
        case H_MAX_ENTITY_AMOUNT:
            _max_entity_amount = hint_value;
            break;
        default:
            break;
    }
}

void Initializer::Initialize() {
    EntityManager::Get().Initialize();
    ComponentManager::Get();

    GLShader::ShaderManager::Initialize();
    GLShaderProgram::ShaderProgramManager::Initialize();

    GLVertexBuffer::VertexBufferManager::Get().Initialize();
    GLVertexArray::VertexArrayManager::Get().Initialize();
    GLElementBuffer::ElementBufferManager::Get().Initialize();
}
void Initializer::Terminate() {
    EntityManager::Get().Terminate();
    ComponentManager::Get().Terminate();
    SystemManager::Terminate();

    GLShader::ShaderManager::Terminate();
    GLShaderProgram::ShaderProgramManager::Terminate();

    GLVertexBuffer::VertexBufferManager::Get().Terminate();
    GLVertexArray::VertexArrayManager::Get().Terminate();
    GLElementBuffer::ElementBufferManager::Get().Terminate();
}

void deInitialize() {
    Initializer::Get().Initialize();
}
void deTerminate() {
    Initializer::Get().Terminate();
}

void deHint(int64_t hint_id, int64_t hint_value) {
    Initializer::Get().SetHint(hint_id, hint_value);
}
}  // namespace DE
