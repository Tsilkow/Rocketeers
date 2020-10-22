#include "pv-interaction.hpp"


void resolvePlanetCollision(Planet& planet, Vessel& vessel)
{
    sf::Vector2f connector = vessel.getPosition() - planet.getPosition();
}
