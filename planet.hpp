#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tom.hpp"

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

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_angle;
    float m_angVelocity;

    sf::Color m_Cterrain;
    sf::Color m_Catmosphere;

    public:
    Planet(const std::shared_ptr<PlanetSettings> pSetts, std::string name, int radius, int mass, sf::Vector2f position, int TOSegments, float angVelocity, int atmHeight, float atmVanishFact, sf::Color Cterrain, sf::Color Catmosphere, int amplitude, int stmoothness, int peakFrequency, int peakVariation, int peakAmplitude);

    void tick();

    //void exertForce

    void draw(sf::RenderTarget& target, bool orbit = false);
};