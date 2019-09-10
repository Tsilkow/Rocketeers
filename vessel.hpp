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
#include "resources.hpp"


float length(sf::Vector2f distance);

float modulo(float a, float b);

sf::Vector2f angleToVector2f(float angle);

float degToRad(float deg);

struct SignalSettings
{
	 int life;
	 int speed;
	 sf::Color color;
	 int maxFreq;
};

class Signal
{
	 std::shared_ptr<SignalSettings> m_sSetts;
	 std::string m_sender;
	 std::string m_data;
	 sf::Vector2f m_center;
	 int m_ticksPassed;
	 sf::CircleShape m_circle;

	 public:
	 Signal(std::string sender, std::string data, sf::Vector2f center, std::shared_ptr<SignalSettings>& sSetts);

	 bool tick();

	 bool withinLastTick(sf::Vector2f start, sf::Vector2f end);
    // checks whether since the last tick, object in a position from range has recieved this signal
	 
	 void draw(sf::RenderTarget& window);


	 const std::string& getSender() {return m_sender; }
	 const std::string& getData() {return m_data; }
};

struct RaySettings
{
	 int TOTicks;
	 int speed;
	 sf::Color color;
	 float length;
	 int maxFirerate;
};

class Ray
{
	 std::shared_ptr<RaySettings> m_rSetts;
	 std::string m_sender;
	 float m_length;
	 float m_angle;
	 sf::Vector2f m_position;
	 sf::RectangleShape m_rectangle;

	 public:
	 Ray(std::string sender, float length, float angle, sf::Vector2f position,
		  std::shared_ptr<RaySettings>& rSetts);

	 bool tick();

	 bool hit(std::vector<sf::Vector2f> mask, sf::Vector2f position);

	 void draw(sf::RenderTarget& window);


	 const std::string& getSender() {return m_sender; }
};

struct VesselType
{
	 std::string model;
	 int TOFuelSprites;
	 int TOGoldSprites;
	 std::vector<sf::Vector2f> collisionMask;
	 int size;
	 int baseMass;
	 int fuelCapacity;
	 int goldCapacity;
	 int miningSpeed;
	 int shootingFreq;
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
	 sf::Color flameColor;
	 float flameRaise;
	 int TOInfoSprites;
	 int fuelMass;
	 int goldMass;
	 int startingFuel;
};

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
	 int m_shootingFreq;
	 float m_thrustPower;
	 int m_thrustFuelUse;
	 float m_manuverability;
	 int m_turnFuelUse;
	 float m_durability;
	 Output m_status;
	 float m_flameHeight;
	 int m_lastSignal;
	 sf::Vector2f m_flameSize;
	 std::vector<sf::Vector2f> m_collisionMask;
	 sf::Sprite m_baseSprite;
	 sf::Sprite m_landSprite;
	 sf::Sprite m_flameSprite;
	 std::vector<sf::Sprite> m_fuelSprites;
	 std::vector<sf::Sprite> m_goldSprites;
	 std::vector<sf::Sprite> m_infoSprites;

	 public:
	 Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type,
			  int allegiance, const std::shared_ptr<VesselSettings> vSetts,
			  ResourceHolder<sf::Texture, std::string>& textures);

	 bool applyForce(sf::Vector2f toAdd);

	 int changeFuel(int change);

	 int changeGold(int change);
	 
	 bool thrust();

	 bool turn(bool clockwise);
	 
	 bool tick(int time, bool& createSignal, Signal& tempSignal, std::shared_ptr<SignalSettings>& sSetts,
				  bool& createRay);
	 
	 void recieve(Signal recieved);
	 
	 void centerView(sf::View& targetView);

	 void draw(sf::RenderTarget& target);


	 const std::string getId() {return m_name + " " + m_model; }
	 const std::string getName() {return m_name; }
	 const std::string getModel() {return m_model; }
	 const std::string getActorName() {return m_actor->getName(); }
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
