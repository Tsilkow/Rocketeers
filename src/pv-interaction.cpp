#include "pv-interaction.hpp"


bool resolvePlanetCollision(Planet& planet, Vessel& vessel)
{
    sf::Vector2f connector = vessel.getPosition() - planet.getPosition();

    float planetHeight = planet.getHeightAt(atan2(connector.y, connector.x));
    std::vector<sf::Vector2f> planetSurface = planet.getSurfaceAt(atan2(connector.y, connector.x));

    if(length(connector) < (planetHeight + vessel.getSize()/2))
    {
	bool collision = false;
	for(int i = 0; i < planetSurface.size() - 1; ++i)
	{
	    for(int j = 0; j < vessel.getMask().size(); ++j)
	    {
		if(linesCollision(planetSurface[i],
				  planetSurface[i+1],
				  vessel.getMask()[       j                       ],
				  vessel.getMask()[modulo(j+1, vessel.getMask().size())]))
		{
		    //std::cout << i << " " << j << std::endl;
		    collision = true;
		}
		if(collision) break;
	    }
	    if(collision) break;
	}

        if(collision)
	{
	    float alongDir = dotProduct(vessel.getVelocity() - planet.getVelocity(),
					connector/length(connector));
	    
	    if(alongDir > 0) return false;
	    
	    sf::Vector2f impulse(0.f, 0.f);
	    float bounce = vessel.getBounce();
	    
	    impulse = -alongDir * connector/length(connector)/(1.f/planet.getMass() + 1.f/vessel.getMass());

	    vessel.applyForce( impulse * (1 + bounce));
	    vessel.applyStrain(length(impulse) * (1 - bounce));

	    return true;
	}
    }
    return false;
}

