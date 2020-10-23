#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "commons.hpp"
#include "vessel.hpp"
#include "planet.hpp"


bool resolvePlanetCollision(Planet& planet, Vessel& vessel);
