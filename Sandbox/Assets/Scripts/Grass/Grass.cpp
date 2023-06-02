#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class Grass : public Script {
    SCRIPT(Grass)
public:
    virtual void OnAttach() override {
        uint32_t x_amount    = 500;
        uint32_t y_amount    = 500;
        auto     interpolate = [](float min, float max, int index, int max_index) { return min + (max - min) * (float(index) / (max_index - 1)); };

        std::vector<Vec3> data(x_amount * y_amount * 3);
        for (size_t i = 0; i < x_amount * y_amount; ++i) {
            int x           = i % x_amount;
            int y           = i / y_amount - 1;
            data[3 * i]     = Vec3(interpolate(-100, 100, x, x_amount), -99, interpolate(-100, 100, y, y_amount));
            data[3 * i + 1] = Vec3(0, 1, 0);
            data[3 * i + 2] = glm::rotate(Vec3(1.0, 0, 0), Random::Float(0, 3.1415926 * 2), Vec3(0, 1, 0));
        }

        grass = VertexArray::Create();
        auto bf =
            VertexBuffer::Create({BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float3}, y_amount * x_amount, &data[0]);
        grass->AddVertexBuffer(bf);

        auto                    shaders = Config::GetPath(DE_CFG_SHADER_PATH);
        std::vector<ShaderPart> parts   = {
            {  ShaderPartType::Vertex, shaders / "Grass/Grass.vs"},
            {ShaderPartType::Geometry, shaders / "Grass/Grass.gs"},
            {ShaderPartType::Fragment, shaders / "Grass/Grass.fs"}
        };

        shader = Shader::Create(parts);
    }
    virtual void OnUpdate(float dt) override { time += dt * Speed; }
    virtual void OnRender() override {
        shader->Bind();
        shader->SetFloat("u_Time", time);
        if (Camera.Has<FPSCamera>()) {
            auto& cam = Camera.Get<FPSCamera>();
            shader->SetMat4("u_Camera.view", cam.GetViewMatrix());
            shader->SetMat4("u_Camera.projection", cam.GetProjectionMatrix());
        }
        Renderer::GetRenderAPI().DrawArrays(grass, RenderPrimitive::Point);
    }

private:
    float            time;
    float            Speed;
    Entity           Camera;
    Ref<VertexArray> grass;
    Ref<Shader>      shader;
};

SCRIPT_BASE(Grass, FIELD(Camera), FIELD(Speed))
