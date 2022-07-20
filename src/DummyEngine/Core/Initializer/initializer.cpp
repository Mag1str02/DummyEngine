#include "DummyEngine/Core/Initializer/initializer.h"
#include "DummyEngine/Core/ECS/component_manager.hpp"
#include "DummyEngine/Core/ECS/entity_manager.hpp"
#include "DummyEngine/Core/ECS/system_manager.hpp"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_ebo.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_vao.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_vbo.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_shader.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_shader_program.h"

namespace DE {

Initializer::Initializer() {
    _max_entity_amount = 1000;
}
Initializer& Initializer::Get() {
    static Initializer initializer;
    return initializer;
}

void Initializer::SetHint(int64_t hint_id, int64_t hint_value) {
    switch (hint_id) {
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
