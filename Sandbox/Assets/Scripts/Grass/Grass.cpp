#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

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

        grass_ = VertexArray::Create();
        auto bf =
            VertexBuffer::Create({BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float3}, y_amount * x_amount, &data[0]);
        grass_->AddVertexBuffer(bf);

        auto                    shaders = Config::Get().ShaderPath;
        std::vector<ShaderPart> parts   = {
            {  ShaderPartType::Vertex, shaders / "Grass/Grass.vs"},
            {ShaderPartType::Geometry, shaders / "Grass/Grass.gs"},
            {ShaderPartType::Fragment, shaders / "Grass/Grass.fs"}
        };

        shader_ = Shader::Create(parts);
    }
    virtual void OnUpdate(float dt) override { time_ += dt * speed_; }
    virtual void OnRender() override {
        shader_->Bind();
        shader_->SetFloat("u_Time", time_);
        if (camera_.Has<FPSCamera>()) {
            auto& cam = camera_.Get<FPSCamera>();
            shader_->SetMat4("u_Camera.view", cam.GetViewMatrix());
            shader_->SetMat4("u_Camera.projection", cam.GetProjectionMatrix());
        }
        Renderer::GetRenderAPI().DrawArrays(grass_, RenderPrimitive::Point);
    }

private:
    float            time_;
    float            speed_;
    Entity           camera_;
    Ref<VertexArray> grass_;
    Ref<Shader>      shader_;
};

SCRIPT_BASE(Grass, FIELD(camera_), FIELD(speed_))
