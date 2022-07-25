#pragma once

#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

namespace fs = std::filesystem;

const Path WORKING_DIR = (fs::current_path().filename() == "build" ? fs::canonical(fs::current_path() / "..") : fs::current_path());
const Path SOURCE_DIR = WORKING_DIR / "src";
const Path RES_DIR = WORKING_DIR / "res";
const Path LOG_DIR = RES_DIR / "Logs";
const Path SHADER_DIR = SOURCE_DIR / "Shaders";
const Path MODEL_DIR = RES_DIR / "Models";

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);
