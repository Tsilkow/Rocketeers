#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "commons.hpp"


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
    std::vector<sf::Vertex> m_line;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_angle;
    float m_angVelocity;

    sf::Color m_Cterrain;
    sf::Color m_Catmosphere;

    public:
    Planet(const std::shared_ptr<PlanetSettings> pSetts, std::string name, int radius, int mass, sf::Vector2f position, float angVelocity, int TOSegments, int amplitude, int smoothness, int peakFrequency, int peakVariation, int peakAmplitude, sf::Color Cterrain, int atmHeight, float atmVanishFact, sf::Color Catmosphere);

    void tick();

    sf::Vector2f exertForce(sf::Vector2f objectPosition, int objectMass);

    void draw(sf::RenderTarget& target, bool orbit = false);

    std::vector<sf::Vector2f> getSurfaceAt(float angle);

    float getHeightAt(float angle);
    
    const sf::Vector2f& getPosition() {return m_position; }
    const sf::Vector2f& getVelocity() {return m_velocity; }
    
};
