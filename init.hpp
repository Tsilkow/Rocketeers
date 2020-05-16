#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "vessel.hpp"


bool init(std::string filename, ResourceHolder<sf::Texture, std::string>& textures, VesselSettings& vSetts,
			 SignalSettings& sSetts, RaySettings& rSetts/*, ViewSettings& viSetts*/);
