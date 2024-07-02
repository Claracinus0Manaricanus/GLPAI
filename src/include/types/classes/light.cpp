#include "light.hpp"

// constructors
PointLight::PointLight() {
  this->position = {0, 0, 0};
  this->color = {1, 1, 1};
  this->strength = 1;
}

PointLight::PointLight(PointLightData& data) {
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
