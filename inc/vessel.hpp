#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "commons.hpp"
#include "arrow.hpp"
#include "actor.hpp"
#include "signal.hpp"
#include "ray.hpp"
#include "resources.hpp"


struct VesselType
{
    std::string model;
    int TOFuelSprites;
    int TOGoldSprites;
    std::vector<sf::Vector2f> collisionMask;
    std::vector<sf::Vector2f> barrels;
    int size;
    int baseMass;
    int fuelCapacity;
    int goldCapacity;
    int miningSpeed;
    int shootingFreq;
    int rayResistance;
    float thrustPower;
    int thrustFuelUse;
    float manuverability;
    int turnFuelUse;
    int durability;
};

struct Faction
{
    std::string name;
    std::string id;
    sf::Color primaryColor;
    sf::Color secondaryColor;
};

struct VesselSettings
{
    std::vector<VesselType> types;
    std::vector<Faction> factions;
    //std::vector< std::pair<sf::Color, sf::Color> > allegianceColors;
    sf::Color flameColor;
    sf::Color velArrowColor;
    sf::Color forArrowColor;
    float flameRaise;
    float actionShadeFade;
    int TOInfoSprites;
    int fuelMass;
    int goldMass;
    int startingFuel;
    float bouncyness;
    int deathTimer;
    int pathLimit;
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
    float m_angVelocity;
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
    int m_rayResistance;
    float m_thrustPower;
    int m_thrustFuelUse;
    float m_manuverability;
    int m_turnFuelUse;
    float m_durability;
    Output m_status;
    float m_flameHeight;
    int m_lastSignal;
    int m_lastRay;
    int m_nextBarrel;
    int m_hitFade;
    int m_recievedFade;
    bool m_dead;
    int m_deathTimer;
    sf::Vector2f m_flameSize;
    std::vector<sf::Vector2f> m_staColMask; // static collision mask (without position factoring)
    std::vector<sf::Vector2f> m_dynColMask; // dynamic collision mask (with position factoring)
    std::vector<sf::Vector2f> m_barrels;
    sf::CircleShape m_sphereMask;
    sf::Sprite m_primarySprite;
    sf::Sprite m_secondarySprite;
    sf::Sprite m_landSprite;
    sf::Sprite m_flameSprite;
    std::vector<sf::Vertex> m_colMaskRepr; // collsion mask representation
    std::vector<sf::Sprite> m_fuelSprites;
    std::vector<sf::Sprite> m_goldSprites;
    std::vector<sf::Sprite> m_infoSprites;
    std::vector<sf::Vertex> m_path;
    Arrow m_velArrow;
    Arrow m_forArrow;

    int changeFuel(int change);

    int changeGold(int change);
	 
    bool thrust();

    bool turn(bool clockwise);

    public:
    Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type,
	   int allegiance, const std::shared_ptr<VesselSettings> vSetts,
	   ResourceHolder<sf::Texture, std::string>& textures);

    void applyForce(sf::Vector2f toAdd);

    void applyRotation(float rotation);
    
    void applyStrain(float toAdd);
    
    void applyStrain(int amount);
	 
    bool tick(int time, bool& createSignal, Signal& tempSignal, std::shared_ptr<SignalSettings>& sSetts,
	      bool& createRay, Ray& tempRay, std::shared_ptr<RaySettings>& rSetts);
	 
    void recieve(Signal recieved);
	 
    void getHit(Ray hitting);
	 
    void centerView(sf::View& targetView);

    void draw(sf::RenderTarget& target, bool debug = false);


    const std::string getId() {return m_vSetts->factions[m_allegiance].id + " " + m_name + " " + m_model; }
    const std::string getName() {return m_vSetts->factions[m_allegiance].id + " " + m_name; }
    const std::string getModel() {return m_model; }
    const std::string getActorName() {return m_actor->getName(); }
    const int getAllegiance() {return m_allegiance; }
    const sf::Vector2f getPosition() {return m_position; }
    const int getSize() {return m_size; }
    const float getAngle() {return m_angle; }
    const sf::Vector2f getVelocity() {return m_velocity; }
    const float getAngVelocity() {return m_angVelocity; }
    const int getFuel() {return m_fuel; }
    const int getGold() {return m_gold; }
    const int getMass() {return m_mass; }
    const std::vector<sf::Vector2f> getMask() {return m_dynColMask; }
    const float getBounce() {return m_vSetts->bouncyness; }
};

bool resolveVesselCollision(Vessel& A, Vessel& B);
