#include <iostream>
#include <vector>
#include <math.h>
#include <memory>

#include "vessel.hpp"
#include "resources.hpp"
#include "init.hpp"


using namespace std;

struct ViewSettings
{
	 sf::Vector2i resolution;
	 int fpsLimit;
	 float scrollSpeed;
	 float zoomSpeed;
	 float rotationSpeed;
};

void reposition(sf::Vector2i resolution, sf::Vector2i newResolution, sf::View& manualView,
					 sf::View& focusView, bool resetCenter = false)
{
	 if(resetCenter)
	 {
		  manualView.setCenter(sf::Vector2f(0, 0));
		  manualView.setSize(sf::Vector2f(newResolution));
		  manualView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
		  
		  focusView.setCenter(sf::Vector2f(0, 0));
		  focusView.setSize(sf::Vector2f(newResolution));
		  focusView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	 }
	 else
	 {
		  manualView.setSize(sf::Vector2f(newResolution.x * manualView.getSize().x / resolution.x,
													 newResolution.y * manualView.getSize().y / resolution.y));

		  focusView.setSize(sf::Vector2f(newResolution.x * focusView.getSize().x / resolution.x,
													newResolution.y * focusView.getSize().y / resolution.y));
	 }
}


int main()
{
	 srand(time(NULL));
	 
	 ResourceHolder<sf::Texture, std::string> textures;
	 //ViewSettings viSetts;
	 SignalSettings sSetts;
	 VesselSettings vSetts;

	 if(!init("data/settings.json", textures, vSetts, sSetts/*, viSetts*/)) return 1;
		 
	 std::shared_ptr<VesselSettings> shared_vSetts = std::make_shared<VesselSettings>(vSetts);
	 std::shared_ptr<SignalSettings> shared_sSetts = std::make_shared<SignalSettings>(sSetts);

	 ViewSettings viSetts = {
		  sf::Vector2i(1200, 900),
		  60,    // fpsLimit
		  20.f,  // scrollSpeed
		  0.01f, // zoomSpeed
		  1.f    // rotationSpeed
	 };

	 string observedId = "John Decker";
	 shared_ptr<Actor> player = make_shared<HumanActor>(observedId);
	 shared_ptr<Actor> enemy = make_shared<AIActor>("A18-CT8");

	 vector<Vessel> vessels;
	 vessels.emplace_back(player, "D18", sf::Vector2f( 200.f,  200.f), 0, 3, shared_vSetts, textures);
	 vessels.emplace_back(enemy , "E7" , sf::Vector2f( 200.f,  200.f), 0, 1, shared_vSetts, textures);
	 vessels.emplace_back(enemy , "F67", sf::Vector2f( 400.f,  200.f), 0, 2, shared_vSetts, textures);
	 vessels.emplace_back(enemy , "F67", sf::Vector2f( 600.f,  200.f), 0, 3, shared_vSetts, textures);
	 vessels.emplace_back(enemy , "F67", sf::Vector2f( 800.f,  200.f), 0, 4, shared_vSetts, textures);

	 vector<Signal> signals;
	 Signal tempSignal("0", "ERROR", sf::Vector2f(0.f, 0.f), shared_sSetts);

	 enum GameState{Simulation};

	 bool exit = false;
	 bool hasFocus = true;
	 GameState currentState = GameState::Simulation;
	 unsigned long long int tick = 0;

	 sf::RenderWindow window(sf::VideoMode(viSetts.resolution.x, viSetts.resolution.y), "Rocketeers");
	 window.setFramerateLimit(viSetts.fpsLimit);
	 sf::Event event;
	 sf::View manualView;
	 sf::View focusView;
	 sf::View planetView;
	 sf::View textView;

	 enum ViewMode{Manual, Focus};
	 ViewMode currentMode = ViewMode::Manual;

	 bool emitted = false;
	 bool shoot = false;

	 reposition(viSetts.resolution, viSetts.resolution, manualView, focusView, true);

	 while(!exit)
	 {
		  window.clear();
		  switch(currentState)
		  {
				case GameState::Simulation:

					 while(window.pollEvent(event))
					 {
						  switch (event.type)
						  {
								case sf::Event::Closed:
									 exit = true;
									 break;
								case sf::Event::Resized:
									 reposition(viSetts.resolution, sf::Vector2i(event.size.width,
																								event.size.height),
													manualView, focusView);
									 viSetts.resolution.x = event.size.width;
									 viSetts.resolution.y = event.size.height;
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
												case sf::Keyboard::BackSlash:
													 window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width,
																						  sf::VideoMode::getDesktopMode().height));
													 break;
												case sf::Keyboard::F1:
													 currentMode = ViewMode::Manual;
													 break;
												case sf::Keyboard::F2:
													 currentMode = ViewMode::Focus;
													 break;
										  }
									 }
									 break;
						  }
					 }
					 
					 if(hasFocus && currentMode == ViewMode::Manual)
					 {
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up   ))
								manualView.move(viSetts.scrollSpeed *
													 angleToVector2f(degToRad(manualView.getRotation()) + M_PI*3/2.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down ))
								manualView.move(viSetts.scrollSpeed *
													 angleToVector2f(degToRad(manualView.getRotation()) + M_PI*1/2.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
								manualView.move(viSetts.scrollSpeed *
													 angleToVector2f(degToRad(manualView.getRotation()) + M_PI*0/2.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left ))
								manualView.move(viSetts.scrollSpeed *
													 angleToVector2f(degToRad(manualView.getRotation()) + M_PI*2/2.f));
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
								manualView.zoom(1.f - viSetts.zoomSpeed);
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
								manualView.zoom(1.f + viSetts.zoomSpeed);
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply))
								manualView.rotate( viSetts.rotationSpeed);
						  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Divide))
								manualView.rotate(-viSetts.rotationSpeed);
					 }

					 switch(currentMode)
					 {
						  case ViewMode::Manual: window.setView(manualView); break;
						  case ViewMode::Focus:  window.setView(focusView);  break;
					 }


					 for(int i = 0; i < signals.size(); ++i)
					 {
						  if(!signals[i].tick())
						  {
								signals.erase(signals.begin() + i);
						  }
					 }

					 for(int i = 0; i < vessels.size(); ++i)
					 {
						  emitted = false;
						  shoot = false;
						  
						  vessels[i].tick(tick, emitted, tempSignal, shared_sSetts, shoot);
						  
						  if(emitted) signals.push_back(tempSignal);

						  for(int j = 0; j < signals.size(); ++j)
						  {
								if(signals[j].withinLastTick(vessels[i].getPosition() - vessels[i].getVelocity(),
																	  vessels[i].getPosition()))
								{
									 vessels[i].recieve(signals[j]);
								}
						  }
						  
						  if(observedId == vessels[i].getActorName()) vessels[i].centerView(focusView);
					 }
					 
					 for(auto it = signals.begin(); it != signals.end(); ++it)
					 {
						  it->draw(window);
					 }

					 for(auto it = vessels.begin(); it != vessels.end(); ++it)
					 {
						  it->draw(window);
					 }
					 
					 break;		 
		  }
		  window.display();
		  //std::cout << tick << std::endl;
		  ++tick;
	 }
	 
	 return 0;
}
