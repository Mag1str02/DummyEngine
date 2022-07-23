#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace fs = std::filesystem;

const size_t MAX_COMPILE_ERROR_LEN = 512;
const size_t MAX_MESSAGE_TYPE_LENGTH = 7;

const size_t MAX_INPUT_FRAME_AMOUNT = 100;

const float DEFAULT_CAMERA_FOV = 60.0f;
const float DEFAULT_CAMERA_ASPECT = 16.0f / 9.0f;
const float DEFAULT_NEAR_PLANE = 0.0000001f;
const float DEFAULT_FAR_PLANE = 2000.0f;

const fs::path WORKING_DIR = (fs::current_path().filename() == "build" ? fs::canonical(fs::current_path() / "..") : fs::current_path());
const fs::path SOURCE_DIR = WORKING_DIR / "src";
const fs::path RES_DIR = WORKING_DIR / "res";
const fs::path LOG_DIR = RES_DIR / "Logs";
const fs::path SHADER_DIR = SOURCE_DIR / "Shaders";
const fs::path MODEL_DIR = RES_DIR / "Models";

const glm::vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const glm::vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const glm::vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const glm::vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const glm::vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const glm::vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

#define DE_ENABLED_FRAME_TIME_READER 0