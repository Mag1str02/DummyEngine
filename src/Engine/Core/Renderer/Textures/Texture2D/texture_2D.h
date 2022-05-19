#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Addition/types.h"

namespace DE {

class Texture2D {

public:
    void BindToUnit(TextureUnitId unit_id) const;

    Texture2D();
    Texture2D(const Texture2D& other);
    Texture2D(Texture2D&& other);
    Texture2D& operator=(const Texture2D& other);
    Texture2D& operator=(Texture2D&& other);
    ~Texture2D();

private:
    friend class Initializer;

    class Texture2DManager {
    private:
        std::unordered_map<Texture2DId, ReferenceCount> _reference_count;
        bool _initialized;

        Texture2DManager();

        Texture2DId ICreateTexture2D();
        Texture2DId ICreateInstance(Texture2DId texture_id);
        void IDestroyInstance(Texture2DId texture_id);
        void IDestroyTexture2D(Texture2DId texture_id);

        static Texture2DManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static Texture2DId CreateTexture2D();
        static Texture2DId CreateInstance(Texture2DId texture_id);
        static void DestroyInstance(Texture2DId texture_id);
        static void DestroyTexture2D(Texture2DId texture_id);
    };

    Texture2DId _texture_id;
};
}  // namespace DE