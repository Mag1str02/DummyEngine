#include "initializer.h"

#include "../ECS/Entity/entity_manager.hpp"
#include "../ECS/Component/component_manager.hpp"
#include "../ECS/System/system_manager.hpp"
#include "../Wrappings/Shader/shader.h"
#include "../Wrappings/ShaderProgram/shader_program.h"

namespace DE
{

    Initializer::Initializer()
    {
        _max_entity_amount = 1000;
    }
    Initializer &Initializer::Get()
    {
        static Initializer initializer;
        return initializer;
    }

    void Initializer::SetHint(int64_t hint_id, int64_t hint_value)
    {
        switch (hint_id)
        {
        case H_MAX_ENTITY_AMOUNT:
            _max_entity_amount = hint_value;
            break;
        default:
            break;
        }
    }

    void Initializer::Initialize()
    {
        EntityManager::Get().Initialize();
        Shader::ShaderManager::Get().Initialize();
        ShaderProgram::ShaderProgramManager::Get().Initialize();
        ComponentManager::Get();
    }
    void Initializer::Terminate()
    {
        EntityManager::Get().Terminate();
        Shader::ShaderManager::Get().Terminate();
        ShaderProgram::ShaderProgramManager::Get().Terminate();
        ComponentManager::Get().Terminate();
        SystemManager::Terminate();
    }

    void deInitialize()
    {
        Initializer::Get().Initialize();
    }
    void deTerminate()
    {
        Initializer::Get().Terminate();
    }

    void deHint(int64_t hint_id, int64_t hint_value)
    {
        Initializer::Get().SetHint(hint_id, hint_value);
    }
} // namespace DE
