#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "actor.hpp"


struct VesselType
{
	 std::string model;
	 sf::Texture baseTexture;
	 sf::Texture landTexture;
	 std::vector<sf::Texture> fuelTextures;
	 std::vector<sf::Texture> infoTextures;
	 std::vector<sf::Vector2f> collisionMask;
	 int size;
	 int baseMass;
	 int fuelCapacity;
	 int goldCapacity;
	 int miningSpeed;
	 int shootingSpeed;
	 float thrustPower;
	 int thrustFuelUse;
	 float manuverability;
	 int turnFuelUse;
	 int durability;
};

struct VesselSettings
{
	 std::vector<VesselType> types;
	 std::vector<sf::Color> allegianceColors;
	 sf::Texture flameTexture;
	 sf::Color flameColor;
	 int fuelMass;
	 int goldMass;
	 int startingFuel;
};

float length(sf::Vector2f distance);

class Vessel
{
	 std::shared_ptr<VesselSettings> m_vSetts;
	 std::shared_ptr<Actor> m_actor;
	 std::string m_name;
	 std::string m_model;
	 int m_allegiance;
	 sf::Vector2f m_position;
	 float m_angle;
	 sf::Vector2f m_velocity;
	 float m_angularVelocity;
	 sf::Vector2f m_force;
	 float m_strain;
	 int m_fuel;
	 int m_gold;
	 int m_baseMass;
	 int m_mass;
	 int m_size;
	 int m_fuelCapacity;
	 int m_goldCapacity;
	 int m_miningSpeed;
	 int m_shootingSpeed;
	 float m_thrustPower;
	 int m_thrustFuelUse;
	 float m_manuverability;
	 int m_turnFuelUse;
	 float m_durability;
	 Output m_status;
	 std::vector<sf::Vector2f> m_collisionMask;
	 sf::Texture m_baseTexture;
	 sf::Texture m_landTexture;
	 sf::Texture m_flameTexture;
	 std::vector<sf::Texture> m_fuelTextures;
	 std::vector<sf::Texture> m_infoTextures;
	 sf::Sprite m_baseSprite;
	 sf::Sprite m_landSprite;
	 sf::Sprite m_flameSprite;
	 std::vector<sf::Sprite> m_fuelSprites;
	 std::vector<sf::Sprite> m_infoSprites;

	 public:
	 Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type, int allegiance,
			  const std::shared_ptr<VesselSettings>& vSetts);

	 bool applyForce(sf::Vector2f toAdd);

	 int changeFuel(int change);

	 int changeGold(int change);
	 
	 bool thrust();

	 bool turn(bool clockwise);
	 
	 bool tick();

	 void draw(sf::RenderTarget& target);


	 const std::string getName() {return m_name; }
	 const std::string getModel() {return m_model; }
	 const int getAllegiance() {return m_allegiance; }
	 const sf::Vector2f getPosition() {return m_position; }
	 const float getAngle() {return m_angle; }
	 const sf::Vector2f getVelocity() {return m_velocity; }
	 const float getAngularVelocity() {return m_angularVelocity; }
	 const int getFuel() {return m_fuel; }
	 const int getGold() {return m_gold; }
	 const int getMass() {return m_mass; }
	 const std::vector<sf::Vector2f> getMask() {return m_collisionMask; }
};
