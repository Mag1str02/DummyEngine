#include "SceneConstants.hpp"

const DE::Vec3& DE::Physics::SceneConstants::getGravity() const {
    return _gravity;
}
void DE::Physics::SceneConstants::setGravity(const DE::Vec3& gravity) {
    SceneConstants::_gravity = gravity;
}
