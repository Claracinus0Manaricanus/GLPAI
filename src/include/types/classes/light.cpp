#include "light.hpp"

// PointLight //

// constructors
PointLight::PointLight() {
  this->position = {0, 0, 0};
  this->color = {1, 1, 1};
  this->strength = 1;
}

PointLight::PointLight(PointLightData data) {
  this->position = data.position;
  this->color = data.color;
  this->strength = data.strength;
}

// setters
void PointLight::move(Vec3 movement) { this->position += movement; }

void PointLight::setPosition(Vec3 position) { this->position = position; }

void PointLight::setColor(Vec3 color) { this->color = color; }

void PointLight::setStrength(float strength) { this->strength = strength; }

// getters
Vec3& PointLight::getPosition() { return position; }

Vec3& PointLight::getColor() { return color; }

float PointLight::getStrength() { return strength; }

// Directi Light //

// constructors
DirectLight::DirectLight() {
  direction = (Vec3){1, 1, 1};
  color = (Vec3){1, 1, 1};
  strength = 1;
}

DirectLight::DirectLight(DirectLightData data) {
  direction = data.direction;
  color = data.color;
  strength = data.strength;
}

// setters
void DirectLight::setDirection(Vec3 direction) { this->direction = direction; }

void DirectLight::setColor(Vec3 color) { this->color = color; }

void DirectLight::setStrength(float strength) { this->strength = strength; }

// getters
Vec3& DirectLight::getDirection() { return direction; }

Vec3& DirectLight::getColor() { return color; }

float DirectLight::getStrength() { return strength; }
