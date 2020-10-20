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
    std::string m_sender; // id of entity that sent this
    std::string m_data; // data being transmitted
    sf::Color m_color;
    sf::Vector2f m_center; // origin
    int m_ticksPassed; // tick passed since it was sent
    sf::CircleShape m_circle; // graphical representation 

   	public:
    Signal(std::string sender, std::string data, sf::Vector2f center, std::shared_ptr<SignalSettings>& sSetts);

    bool tick();

    bool withinLastTick(sf::Vector2f start, sf::Vector2f end);
    // checks whether since the last tick, object in a position from range has recieved this signal
	 
    void draw(sf::RenderTarget& window);


    const std::string& getSender() {return m_sender; }
    const std::string& getData() {return m_data; }
    const sf::Color getColor() {return m_color; }
};
