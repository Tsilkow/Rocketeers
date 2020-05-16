#include "actor.hpp"


Actor::Actor(std::string name):
    m_name(name)
{
    ;
}

void HumanActor::recieve(std::string sender, std::string data)
{
    std::cout << "= = = = = = = = = = = =" << std::endl;
    std::cout << "FROM = " << sender << std::endl;
    std::cout << "TEXT = " << std::endl;
    std::cout << data << std::endl;
    std::cout << "= = = = = = = = = = = =" << std::endl;
}

Output HumanActor::act(Input in)
{
    Output result = {false, false, false, false, false, false, false};

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W       )) result.thrust = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A       )) result.turnLeft = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D       )) result.turnRight = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift  )) result.signal = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q       )) result.load = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E       )) result.unload = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space   )) result.shoot = true;

    return result;
}

void AIActor::recieve(std::string sender, std::string data)
{
    std::cout << "recieved" << std::endl;
}

Output AIActor::act(Input in)
{
    Output result = {false, false, false, false, false, false, false};

    return result;
}
