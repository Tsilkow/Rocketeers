#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "commons.hpp"
#include "arrow.hpp"


struct PlanetSettings
{
    float gravConst;
    float airFriConst;
};

void connectPeaks(std::vector<int>& result, int begin, int end, int amplitude, int loop);

std::vector<int> generateHeights(int base, int TOSegments, int amplitude, float smoothness, int peakFrequency, int peakVariation, int peakAmplitude);

class Planet
{
    std::string m_name;

    std::shared_ptr<PlanetSettings> m_pSetts;
    int m_radius;
    int m_mass;
    float m_segmentSize;
    int m_atmHeight;
    float m_atmVanishFact;
    std::vector<int> m_heights;
    std::vector<sf::Vector2f> m_statSurface;
    std::vector<sf::Vector2f> m_statAtmosphere;
    std::vector<sf::Vertex> m_dynSurface;
    std::vector<sf::Vertex> m_dynAtmosphere;
    std::vector<sf::Vertex> m_surfaceHighlight;
    std::vector<sf::Vertex> m_heightHighlight;
    Arrow m_velocityArrow;
    Arrow m_surfaceVector;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_angle;
    float m_angVelocity;

    sf::Color m_Cterrain;
    sf::Color m_Catmosphere;

    // first number is the index of the segment, the second one is a number on range 0.0 to 1.0 of how far on this segment the precise point is
    std::pair<int, float> getSegmentAt(float angle);

    public:
    Planet(const std::shared_ptr<PlanetSettings> pSetts, std::string name, int radius, int mass, sf::Vector2f position, float angVelocity, int TOSegments, int amplitude, int smoothness, int peakFrequency, int peakVariation, int peakAmplitude, sf::Color Cterrain, int atmHeight, float atmVanishFact, sf::Color Catmosphere);

    void tick();

    sf::Vector2f exertForce(sf::Vector2f objectPosition, int objectMass);

    void draw(sf::RenderTarget& target, bool orbit = false);
    
    float getHeightAt(float angle);
    
    sf::Vector2f getSurfacePoint(float angle);
	
    std::vector<sf::Vector2f> getSurfaceAt(float angle);

    sf::Vector2f getVelocityAt(float angle);

    sf::Vector2f getSurfaceVector(float angle);
    
    const sf::Vector2f& getPosition() {return m_position; }
    const sf::Vector2f& getVelocity() {return m_velocity; }
    const int& getMass() {return m_mass; }
    
};
