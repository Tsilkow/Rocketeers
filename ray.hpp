#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tom.hpp"


struct RaySettings
{
    int life;
    int speed;
    sf::Color color;
    sf::Vector2f dimensions;
    int maxFirerate;
};

class Ray
{
    std::shared_ptr<RaySettings> m_rSetts;
    std::string m_sender;
    sf::Color m_color;
    sf::Vector2f m_position;
    float m_angle;
    sf::Vector2f m_velocity;
    int m_length;
    sf::RectangleShape m_rectangle;
    int m_ticksPassed;
    bool m_hit;

	public:
    Ray(std::string sender, float angle, sf::Vector2f position, sf::Vector2f velocity,
	std::shared_ptr<RaySettings>& rSetts);

    bool tick();

    bool hit(std::vector<sf::Vector2f> mask, sf::Vector2f position, int size);

    void draw(sf::RenderTarget& window);


    const std::string& getSender() {return m_sender; }
    const sf::Color getColor() {return m_color; }
};
