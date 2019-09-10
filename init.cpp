#include "init.hpp"

using namespace nlohmann;

bool init(std::string filename, ResourceHolder<sf::Texture, std::string>& textures,
			 VesselSettings& vSetts, SignalSettings& sSetts/*, ViewSettings& viSetts*/)
{
	 std::fstream mainFile(filename, std::fstream::in);
	 std::fstream vesselFile;
	 if(!mainFile.good())
	 {
		  std::cout << "!ERROR! Settings file cannot be opened! Aborting ..." << std::endl;
		  return false;
	 }
	 
	 json mainParse = json::parse(mainFile);
	 json vesselParse;
	 //auto vesselParse = mainParse["vessel"];
	 VesselType tempVessel;

	 std::cout << "Parsed settings file: " << std::endl << std::setw(4) << mainParse << std::endl;

	 // Loading vessel settings

	 vSetts.flameColor = sf::Color(mainParse["vessel"]["aiFlame-color"][0].get<int>(),
											 mainParse["vessel"]["aiFlame-color"][1].get<int>(),
											 mainParse["vessel"]["aiFlame-color"][2].get<int>());
	 vSetts.flameRaise = mainParse["vessel"]["fFlame-raise"].get<float>();
	 
	 vSetts.TOInfoSprites = mainParse["vessel"]["iTotal-info-sprites"].get<int>();
	 vSetts.fuelMass = mainParse["vessel"]["iFuel-mass"].get<int>();
	 vSetts.goldMass = mainParse["vessel"]["iGold-mass"].get<int>();
	 vSetts.startingFuel = mainParse["vessel"]["iStarting-fuel"].get<int>();

	 for(int i = 0; i < mainParse["vessel"]["aaiColors"].size(); ++i)
	 {
		  vSetts.allegianceColors.push_back(sf::Color(mainParse["vessel"]["aaiColors"][i][0].get<int>(),
																	 mainParse["vessel"]["aaiColors"][i][1].get<int>(),
																	 mainParse["vessel"]["aaiColors"][i][2].get<int>()));
	 }
	 
	 textures.load("flame", mainParse["vessel"]["sFlame-texture"].get<std::string>());

	 // Loading vessel types

	 for(int i = 0; i < mainParse["vessel"]["asTypes"].size(); ++i)
	 {
		  vesselFile.open("data/defs/" + mainParse["vessel"]["asTypes"][i].get<std::string>() + ".json",
								std::fstream::in);

		  if(!vesselFile.good())
		  {
				std::cout << "!ERROR! \"" << mainParse["vessel"]["asTypes"][i].get<std::string>()
							 << "\" vessel file cannot be opened! Aborting ..." << std::endl;
				return false;
		  }
		  
		  vesselParse = json::parse(vesselFile);


		  tempVessel.model         = vesselParse["sModel"].get<std::string>();
		  tempVessel.TOFuelSprites = vesselParse["iTotal-fuel-sprites"].get<int>();
		  tempVessel.TOGoldSprites = vesselParse["iTotal-gold-sprites"].get<int>();

		  tempVessel.size           = vesselParse["iSize"].get<int>();
		  tempVessel.baseMass       = vesselParse["iBase-mass"].get<int>();
		  tempVessel.fuelCapacity   = vesselParse["iFuel-capacity"].get<int>();
		  tempVessel.goldCapacity   = vesselParse["iGold-capacity"].get<int>();
		  tempVessel.miningSpeed    = vesselParse["iMining-speed"].get<int>();
		  tempVessel.shootingFreq   = vesselParse["iShooting-frequency"].get<int>();
		  tempVessel.thrustPower    = vesselParse["fThrust-power"].get<float>();
		  tempVessel.thrustFuelUse  = vesselParse["iThrust-fuel-use"].get<int>();
		  tempVessel.manuverability = vesselParse["fManuverability"].get<float>();
		  tempVessel.turnFuelUse    = vesselParse["iTurn-fuel-use"].get<int>();
		  tempVessel.durability     = vesselParse["iDurability"].get<int>();

		  for(int j = 0; j < vesselParse["aafCollision-mask"].size(); ++j)
		  {
				tempVessel.collisionMask.push_back(
					 sf::Vector2f(vesselParse["aafCollision-mask"][j][0].get<float>(),
									  vesselParse["aafCollision-mask"][j][1].get<float>()));
		  }

		  textures.load(
				tempVessel.model + "::base", vesselParse["sTexture"].get<std::string>() + "base.png");
		  textures.load(
				tempVessel.model + "::land", vesselParse["sTexture"].get<std::string>() + "land.png");
		  
		  for(int j = 0; j < tempVessel.TOFuelSprites; ++j)
		  {
				textures.load(
					 tempVessel.model + "::fuel" + std::to_string(j),
					 vesselParse["sTexture"].get<std::string>() + "fuel" + std::to_string(j) + ".png");
		  }
		  
		  for(int j = 0; j < tempVessel.TOGoldSprites; ++j)
		  {
				textures.load(
					 tempVessel.model + "::gold" + std::to_string(j),
					 vesselParse["sTexture"].get<std::string>() + "gold" + std::to_string(j) + ".png");
		  }
		  
		  for(int j = 0; j < vSetts.TOInfoSprites; ++j)
		  {
				textures.load(
					 tempVessel.model + "::info" + std::to_string(j),
					 vesselParse["sTexture"].get<std::string>() + "info" + std::to_string(j) + ".png");
		  }

		  vesselFile.close();

		  vSetts.types.push_back(tempVessel);
	 }

	 // Loading signal settings

	 sSetts.life    = mainParse["signal"]["iLife"].get<int>();
	 sSetts.speed   = mainParse["signal"]["iSpeed"].get<int>();
	 sSetts.color = sf::Color(mainParse["signal"]["aiColor"][0].get<int>(),
									  mainParse["signal"]["aiColor"][1].get<int>(),
									  mainParse["signal"]["aiColor"][2].get<int>());
	 sSetts.maxFreq = mainParse["signal"]["iMax-frequency"].get<int>();

	 return true;
}
