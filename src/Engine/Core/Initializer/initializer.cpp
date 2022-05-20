#include "initializer.h"

#include "../ECS/Component/component_manager.hpp"
#include "../ECS/Entity/entity_manager.hpp"
#include "../ECS/System/system_manager.hpp"
#include "../Renderer/Buffers/EBO/ebo.h"
#include "../Renderer/Buffers/VAO/vao.h"
#include "../Renderer/Buffers/VBO/vbo.h"
#include "../Renderer/Shader/shader.h"
#include "../Renderer/ShaderProgram/shader_program.h"


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

    Shader::ShaderManager::Initialize();
    ShaderProgram::ShaderProgramManager::Initialize();

    VertexBuffer::VertexBufferManager::Get().Initialize();
    VertexArray::VertexArrayManager::Get().Initialize();
    ElementBuffer::ElementBufferManager::Get().Initialize();
}
void Initializer::Terminate() {
    EntityManager::Get().Terminate();
    ComponentManager::Get().Terminate();
    SystemManager::Terminate();

    Shader::ShaderManager::Terminate();
    ShaderProgram::ShaderProgramManager::Terminate();

    VertexBuffer::VertexBufferManager::Get().Terminate();
    VertexArray::VertexArrayManager::Get().Terminate();
    ElementBuffer::ElementBufferManager::Get().Terminate();
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
