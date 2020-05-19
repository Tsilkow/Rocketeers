#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Arrow
{
    private:
    sf::Vector2f m_origin;
    float m_length;
    float m_angle;
    sf::Color m_color;
    int m_width;
    std::vector<sf::Vertex> m_representation;

    void calculate();

    public:
    Arrow(sf::Vector2f origin = {0.f, 0.f}, float length = 1.f, float angle = M_PI,
	  sf::Color color = sf::Color::White, int width = 10);

    bool setLength(float length);

    bool setOrigin(sf::Vector2f origin);

    bool setAngle(float angle);

    bool setDirection(sf::Vector2f from, sf::Vector2f to);

    bool setColor(sf::Color color);

    void draw(sf::RenderTarget& target);
};
