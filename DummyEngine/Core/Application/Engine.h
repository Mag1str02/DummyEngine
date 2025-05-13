#pragma once

namespace DummyEngine {

    using FSetupApplication = void (*)();

    int Main(FSetupApplication setup);
}  // namespace DummyEngine