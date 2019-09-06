#include "actor.hpp"


Actor::Actor(std::string name):
	 m_name(name)
{
	 ;
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

Output AIActor::act(Input in)
{
	 Output result = {false, false, false, false, false, false, false};

	 return result;
}
