#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace fs = std::filesystem;

const size_t MAX_COMPILE_ERROR_LEN = 512;
const size_t MAX_MESSAGE_TYPE_LENGTH = 7;

const float DEFAULT_CAMERA_FOV = 60.0f;
const float DEFAULT_CAMERA_ASPECT = 16.0f / 9.0f;
const float DEFAULT_NEAR_PLANE = 0.1f;
const float DEFAULT_FAR_PLANE = 2000.0f;

const fs::path WORKING_DIR = fs::canonical(fs::current_path() / "..");
const fs::path SOURCE_DIR = WORKING_DIR / "src";
const fs::path RES_DIR = WORKING_DIR / "res";
const fs::path LOG_DIR = RES_DIR / "Logs";
const fs::path SHADER_DIR = SOURCE_DIR / "Shaders";