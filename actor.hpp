#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>



struct Input
{
	 int a;
};

struct Output
{
	 bool thrust;
	 bool turnLeft;
	 bool turnRight;
	 bool signal;
	 bool load;
	 bool unload;
	 bool shoot;
};	 

class Actor
{
	 std::string m_name;

	 public:
	 Actor(std::string name);

	 virtual void recieve(std::string sender, std::string data) = 0;

	 virtual Output act(Input in) = 0;

	 
	 const std::string getName() {return m_name; }
};

class HumanActor: public Actor
{
	 public:
	 HumanActor(std::string name): Actor(name) {; };

	 void recieve(std::string sender, std::string data);

	 Output act(Input in);
};
	 
class AIActor: public Actor
{
	 public:
	 AIActor(std::string name): Actor(name) {; };

	 void recieve(std::string sender, std::string data);

	 Output act(Input in);
};
