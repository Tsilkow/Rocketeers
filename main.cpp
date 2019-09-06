#include <iostream>
#include <vector>
#include <math.h>
#include <memory>

#include "vessel.hpp"

using namespace std;

int main()
{
	 srand(time(NULL));

	 sf::Vector2i resolution(1200, 900);

	 sf::Texture flameTexture;
	 if(!flameTexture.loadFromFile("data/flame.png")) return -1;

	 sf::Texture fighterTexture;
	 if(!fighterTexture.loadFromFile("data/Orel1337/base.png")) return -1;

	 sf::Texture fighterLandTex;
	 if(!fighterLandTex.loadFromFile("data/Orel1337/land.png")) return -1;
	 
	 std::vector<sf::Texture> fighterFuelTex(6);
	 for(int i = 0; i < 6; ++i)
	 {
		  if(!fighterFuelTex[i].loadFromFile("data/Orel1337/fuel"+to_string(i)+".png")) return -1;
	 }
	 
	 std::vector<sf::Texture> fighterInfoTex(7);
	 for(int i = 0; i < 7; ++i)
	 {
		  if(!fighterInfoTex[i].loadFromFile("data/Orel1337/light"+to_string(i)+".png")) return -1;
	 }
	 
	 std::vector<sf::Vector2f> fighterMask = {
		  sf::Vector2f( 0.23f,  0.38f),
		  sf::Vector2f( 0.20f,  0.27f),
		  sf::Vector2f( 0.35f,  0.27f),
		  sf::Vector2f( 0.45f,  0.05f),
		  sf::Vector2f( 0.15f,  0.05f),
		  sf::Vector2f( 0.10f, -0.10f),
		  sf::Vector2f( 0.10f, -0.25f),
		  sf::Vector2f( 0.00f, -0.50f),
		  sf::Vector2f(-0.10f, -0.25f),
		  sf::Vector2f(-0.10f, -0.10f),
		  sf::Vector2f(-0.15f,  0.05f),
		  sf::Vector2f(-0.45f,  0.05f),
		  sf::Vector2f(-0.35f,  0.27f),
		  sf::Vector2f(-0.20f,  0.27f),
		  sf::Vector2f(-0.23f,  0.38f)
	 };

	 VesselType fighter = {
		  "OREL1337",     // model
		  fighterTexture, // texture
		  fighterLandTex, // landTexture
		  fighterFuelTex, // fuelTexture
		  fighterInfoTex, // infoTexture
		  fighterMask,    // collisionMask
		  100,            // size
		  2000,           // baseMass
		  10000,          // fuelCapacity
		  500,            // goldCapacity
		  1,              // miningSpeed
		  30,             // shootingSpeed
		  200.f,          // thrustPower
		  -5,             // thrustFuelUse
		  0.001f,         // manuverability
		  -1,             // turnFuelUse
		  1000000         // durability
	 };

	 VesselSettings vSetts = {
		  {fighter},                 // types[0]
		  {sf::Color(255, 255, 255), // allegianceColors[0]
			sf::Color(67, 179, 174),  // allegianceColors[1]
			sf::Color(255, 128, 0)},  // allegianceColors[2]
		  flameTexture,              // flameTexture
		  sf::Color(255, 255, 0),    // flameColor
		  1,                         // fuelMass
		  10,                        // goldMass
		  10000                      // startingFuel
	 };

	 std::shared_ptr<Actor> player = make_shared<HumanActor>("John Decker");
	 std::shared_ptr<Actor> enemy = make_shared<AIActor>("A18-CT8");

	 std::vector<Vessel> vessels;
	 vessels.emplace_back(player, "D18",
	 							 sf::Vector2f(200.f, 200.f), 0, 1,
	 							 std::make_shared<VesselSettings>(vSetts));
	 //vessels.emplace_back(enemy, "E7",
	 //						 sf::Vector2f(-800.f, 200.f), 0, 2,
	 //						 std::make_shared<VesselSettings>(vSetts));

	 enum GameState{Simulation};

	 bool exit = false;
	 bool hasFocus = true;
	 GameState current = GameState::Simulation;

	 sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Rocketeers");
	 window.setFramerateLimit(60);
	 sf::Event event;

	 while(!exit)
	 {
		  window.clear();
		  switch(current)
		  {
				case GameState::Simulation:

					 while(window.pollEvent(event))
					 {
						  switch (event.type)
						  {
								case sf::Event::Closed:
									 exit = true;
									 break;
							 	case sf::Event::LostFocus:
									 hasFocus = false;
									 break;
								case sf::Event::GainedFocus:
									 hasFocus = true;
									 break;
								case sf::Event::KeyPressed:
									 if(hasFocus)
									 {
										  switch(event.key.code)
										  {
												case sf::Keyboard::Escape:
													 window.close();
													 exit = true;
													 break;
										  }
									 }
									 break;
						  }
					 }
					 
					 if(hasFocus)
					 {/*
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) vessels[0]
						  /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up   ))
								testMap.moveScreen(sf::Vector2f( 0.f, -1.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
								testMap.moveScreen(sf::Vector2f(+1.f,  0.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down ))
								testMap.moveScreen(sf::Vector2f( 0.f,  1.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left ))
						  testMap.moveScreen(sf::Vector2f(-1.f,  0.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
								testMap.zoomScreen(true);
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
								testMap.zoomScreen(false);*/
					 }

					 for(int i = 0; i < vessels.size(); ++i)
					 {
						  vessels[i].tick();
					 }

					 for(auto it = vessels.begin(); it != vessels.end(); ++it)
					 {
						  it->draw(window);
					 }
					 
					 break;		 
		  }
		  window.display();
	 }
	 
	 return 0;
}
