#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "vessel.hpp"
#include "planet.hpp"


bool init(std::string filename, ResourceHolder<sf::Texture, std::string>& textures, VesselSettings& vSetts,
			 SignalSettings& sSetts, RaySettings& rSetts/*, ViewSettings& viSetts*/);

bool setup(std::string filename, std::vector<Vessel>& vessels, std::vector<Planet>& planets,
	   std::shared_ptr<Actor>& player, std::shared_ptr<Actor>& enemy,
	   ResourceHolder<sf::Texture, std::string>& textures, std::shared_ptr<VesselSettings>& shr_vSetts,
	   PlanetSettings& pSetts);
