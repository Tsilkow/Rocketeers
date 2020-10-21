#include "init.hpp"


using namespace nlohmann;

bool init(std::string filename, ResourceHolder<sf::Texture, std::string>& textures,
	  VesselSettings& vSetts, SignalSettings& sSetts, RaySettings& rSetts/*, ViewSettings& viSetts*/)
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
    VesselType tempVessel;
    //auto vesselParse = mainParse["vessel"];

    std::cout << "Parsed settings file: " << std::endl << std::setw(4) << mainParse << std::endl;

    // Loading vessel settings

    try
    {
	vSetts.flameColor = sf::Color(mainParse["vessel"]["aiFlame-color"][0].get<int>(),
				      mainParse["vessel"]["aiFlame-color"][1].get<int>(),
				      mainParse["vessel"]["aiFlame-color"][2].get<int>());
	vSetts.velArrowColor = sf::Color(mainParse["vessel"]["aiVelocity-arrow-color"][0].get<int>(),
					 mainParse["vessel"]["aiVelocity-arrow-color"][1].get<int>(),
					 mainParse["vessel"]["aiVelocity-arrow-color"][2].get<int>());
	vSetts.forArrowColor = sf::Color(mainParse["vessel"]["aiForce-arrow-color"][0].get<int>(),
					 mainParse["vessel"]["aiForce-arrow-color"][1].get<int>(),
					 mainParse["vessel"]["aiForce-arrow-color"][2].get<int>());
	vSetts.flameRaise = mainParse["vessel"]["fFlame-raise"].get<float>();

	vSetts.actionShadeFade = mainParse["vessel"]["fActionShadeFade"].get<float>();
	vSetts.TOInfoSprites = mainParse["vessel"]["iTotal-info-sprites"].get<int>();
	vSetts.fuelMass = mainParse["vessel"]["iFuel-mass"].get<int>();
	vSetts.goldMass = mainParse["vessel"]["iGold-mass"].get<int>();
	vSetts.startingFuel = mainParse["vessel"]["iStarting-fuel"].get<int>();
	vSetts.bouncyness = mainParse["vessel"]["fBouncyness"].get<float>();
	vSetts.deathTimer = mainParse["vessel"]["iDeathTimer"].get<int>();
	vSetts.pathLimit = mainParse["vessel"]["iPathLimit"].get<int>();
	
	for(int i = 0; i < mainParse["vessel"]["aaFactions"].size(); ++i)
	{
	    vSetts.factions.push_back(
		   {mainParse["vessel"]["aaFactions"][i][0].get<std::string>(),
		    mainParse["vessel"]["aaFactions"][i][1].get<std::string>(),
		    sf::Color(mainParse["vessel"]["aaFactions"][i][2][0].get<int>(),
			      mainParse["vessel"]["aaFactions"][i][2][1].get<int>(),
			      mainParse["vessel"]["aaFactions"][i][2][2].get<int>()),
		    sf::Color(mainParse["vessel"]["aaFactions"][i][3][0].get<int>(),
			      mainParse["vessel"]["aaFactions"][i][3][1].get<int>(),
			      mainParse["vessel"]["aaFactions"][i][3][2].get<int>())});
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
	    tempVessel.rayResistance  = vesselParse["iRay-resistance"].get<int>();
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
		  
	    for(int j = 0; j < vesselParse["aafBarrels"].size(); ++j)
	    {
		tempVessel.barrels.push_back(
		       sf::Vector2f(vesselParse["aafBarrels"][j][0].get<float>(),
				    vesselParse["aafBarrels"][j][1].get<float>()));
	    }

	    textures.load(
		   tempVessel.model + "::primary", vesselParse["sTexture"].get<std::string>() + "primary.png");
	    textures.load(
		   tempVessel.model + "::secondary", vesselParse["sTexture"].get<std::string>() + "secondary.png");
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

	// Loading ray settings
	 
	rSetts.life        = mainParse["ray"]["iLife"].get<int>();
	rSetts.speed       = mainParse["ray"]["iSpeed"].get<int>();
	rSetts.color = sf::Color(mainParse["ray"]["aiColor"][0].get<int>(),
				 mainParse["ray"]["aiColor"][1].get<int>(),
				 mainParse["ray"]["aiColor"][2].get<int>());
	rSetts.dimensions = sf::Vector2f(mainParse["ray"]["afDimensions"][0].get<float>(),
					 mainParse["ray"]["afDimensions"][1].get<float>());
	rSetts.maxFirerate = mainParse["ray"]["iMax-firerate"].get<int>();

    
    }
    catch (json::exception& e)
    {
	std::cout << "INIT FILE ERROR = " << e.what() << std::endl;
    }
	 

    return true;
}

bool setup(std::string filename, std::vector<Vessel>& vessels, std::vector<Planet>& planets,
	   std::shared_ptr<Actor>& player, std::shared_ptr<Actor>& enemy,
	   ResourceHolder<sf::Texture, std::string>& textures, std::shared_ptr<VesselSettings>& shr_vSetts,
	   PlanetSettings& pSetts)
{
    std::fstream mainFile(filename, std::fstream::in);

    if(!mainFile.good())
    {
	std::cout << "!ERROR! Setup file cannot be opened! Aborting ..." << std::endl;
	return false;
    }
    
    json mainParse = json::parse(mainFile);
    json vesselParse;
    int playerControlled;
    int atmHeight = 0;
    float atmVanishFact = 0.f;
    sf::Color Catmosphere = sf::Color::Black;
    std::shared_ptr<Actor> controller;

    std::cout << "Parsed setup file: " << std::endl << std::setw(4) << mainParse << std::endl;

    try{
	pSetts.gravConst   = mainParse["fGravity"].get<float>();
	pSetts.airFriConst = mainParse["fAirFriction"].get<float>();
	playerControlled   = mainParse["iPlayerControlledFaction"].get<int>();

	for(int i = 0; i < mainParse["planets"].size(); ++i)
	{
	    if(mainParse["planets"][i]["atmosphere"]["present"].get<bool>() == true)
	    {
		atmHeight     = mainParse["planets"][i]["atmosphere"]["iHeight"].get<int>();
		atmVanishFact = mainParse["planets"][i]["atmosphere"]["fVanishingFactor"].get<float>();
		Catmosphere   = sf::Color(mainParse["planets"][i]["atmosphere"]["aiColor"][0].get<int>(),
					  mainParse["planets"][i]["atmosphere"]["aiColor"][1].get<int>(),
					  mainParse["planets"][i]["atmosphere"]["aiColor"][2].get<int>());
	    }
	    else
	    {		
		atmHeight = 0;
		atmVanishFact = 0.f;
		Catmosphere = sf::Color::Black;
	    }
		
	    planets.emplace_back(std::make_shared<PlanetSettings>(pSetts),
				 mainParse["planets"][i]["sName"].get<std::string>(),
				 mainParse["planets"][i]["iRadius"].get<int>(),
				 mainParse["planets"][i]["iMass"].get<int>(),
				 sf::Vector2f(mainParse["planets"][i]["aiPosition"][0].get<float>(),
					      mainParse["planets"][i]["aiPosition"][1].get<float>()),
				 mainParse["planets"][i]["fAngularVelocity"].get<float>(),
				 mainParse["planets"][i]["surface"]["iSegmentsTotal"].get<int>(),
				 mainParse["planets"][i]["surface"]["iGeneralAmplitude"].get<int>(),
				 mainParse["planets"][i]["surface"]["fSmoothness"].get<float>(),
				 mainParse["planets"][i]["surface"]["iPeakFrequency"].get<int>(),
				 mainParse["planets"][i]["surface"]["iPeakVariation"].get<int>(),
				 mainParse["planets"][i]["surface"]["iPeakAmplitude"].get<int>(),
				 sf::Color(mainParse["planets"][i]["surface"]["aiColor"][0].get<int>(),
					   mainParse["planets"][i]["surface"]["aiColor"][1].get<int>(),
					   mainParse["planets"][i]["surface"]["aiColor"][2].get<int>()),
				 atmHeight,
				 atmVanishFact,
				 Catmosphere);
	}

	for(int i = 0; i < mainParse["vessels"].size(); ++i)
	{
	    if(mainParse["vessels"][i]["iAllegiance"].get<int>() == playerControlled)
	    {
		vessels.emplace_back(player,
				     mainParse["vessels"][i]["sName"].get<std::string>(),
				     sf::Vector2f(mainParse["vessels"][i]["aiPosition"][0].get<int>(),
						  mainParse["vessels"][i]["aiPosition"][1].get<int>()),
				     mainParse["vessels"][i]["iType"].get<int>(),
				     mainParse["vessels"][i]["iAllegiance"].get<int>(),
				     shr_vSetts,
				     textures);
		std::cout << i << std::endl;
	    }
	    else
	    {
		vessels.emplace_back(enemy,
				     mainParse["vessels"][i]["sName"].get<std::string>(),
				     sf::Vector2f(mainParse["vessels"][i]["aiPosition"][0].get<int>(),
						  mainParse["vessels"][i]["aiPosition"][1].get<int>()),
				     mainParse["vessels"][i]["iType"].get<int>(),
				     mainParse["vessels"][i]["iAllegiance"].get<int>(),
				     shr_vSetts,
				     textures);
	    }
	    

	}
	    
    }
    catch (json::exception& e)
    {
	std::cout << "SETUP FILE ERROR = " << e.what() << std::endl;
    }

    return true;
}
