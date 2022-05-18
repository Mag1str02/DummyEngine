#pragma once

namespace DE {
class Initializable {
protected:
    bool _initialized;

public:
    virtual ~Initializable() = default;
    virtual void Initialize() = 0;
    virtual void Terminate() = 0;
};
}  // namespace DE