#include "vessel.hpp"



float length(sf::Vector2f distance)
{
	 return sqrt(distance.x*distance.x + distance.y*distance.y);
}

float modulo(float a, float b)
{
	 while(a <  0.f) a += b;
	 while(a >= b  ) a -= b;

	 return a;
}

sf::Vector2f angleToVector2f(float angle)
{
	 return sf::Vector2f(cos(angle), sin(angle));
}

float degToRad(float deg)
{
	 return deg * M_PI/180;
}

Signal::Signal(std::string sender, std::string data, sf::Vector2f center,
					std::shared_ptr<SignalSettings>& sSetts):
	 m_sender(sender),
	 m_data(data),
	 m_center(center),
	 m_sSetts(sSetts)
{
	 m_ticksPassed = 0;
	 m_circle.setPosition(m_center);
	 m_circle.setRadius(0);
	 m_circle.setOutlineColor(m_sSetts->color);
	 m_circle.setFillColor(sf::Color(255, 255, 255, 0));
	 m_circle.setOutlineThickness(5);
	 m_circle.setPointCount(64);
}

bool Signal::tick()
{
	 sf::Color temp = m_sSetts->color;
	 
	 ++m_ticksPassed;
	 if(m_ticksPassed >= m_sSetts->life) return false;

	 m_circle.setRadius(m_ticksPassed * m_sSetts->speed);
	 m_circle.setPosition(m_center -
								 sf::Vector2f(m_ticksPassed * m_sSetts->speed, m_ticksPassed * m_sSetts->speed));

	 temp.a = std::max(0, std::min(255, (int)
									  std::round((m_sSetts->life - m_ticksPassed) * 510.f/m_sSetts->life)));
	 m_circle.setOutlineColor(temp);
	 
	 return true;
}

bool Signal::withinLastTick(sf::Vector2f start, sf::Vector2f end)
{
	 float ls = length(m_center - start);
	 float le = length(m_center - end);
	 float now = m_ticksPassed * m_sSetts->speed;
	 float past = (m_ticksPassed-1) * m_sSetts->speed;

	 

	 if((ls > past && ls <= now) || (le > past && le <= now) ||
		 (ls >= now && le < past) || (le >= now && ls < past))
	 {
		  std::cout << "------" << std::endl;
		  std::cout << ls << " " << le << " " << now << " " << past << std::endl;
	 
		  return true;
	 }

	 return false;
}

void Signal::draw(sf::RenderTarget& window)
{
	 window.draw(m_circle);
}

Vessel::Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type,
					int allegiance, const std::shared_ptr<VesselSettings> vSetts,
					ResourceHolder<sf::Texture, std::string>& textures):
	 m_vSetts(vSetts),
	 m_actor(actor),
	 m_name(name),
	 m_model(vSetts->types[type].model),
	 m_allegiance(allegiance),
	 m_position(position),
	 m_angle(0.f),
	 m_velocity(0.f, 0.f),
	 m_angularVelocity(0.f),
	 m_force(0.f, 0.f),
	 m_strain(0.f),
	 m_fuel(vSetts->startingFuel),
	 m_gold(0),
	 m_baseMass(vSetts->types[type].baseMass),
	 m_mass(m_baseMass + m_fuel * vSetts->fuelMass),
	 m_size(vSetts->types[type].size),
	 m_fuelCapacity(vSetts->types[type].fuelCapacity),
	 m_goldCapacity(vSetts->types[type].goldCapacity),
	 m_miningSpeed(vSetts->types[type].miningSpeed),
	 m_shootingFreq(vSetts->types[type].shootingFreq),
	 m_thrustPower(vSetts->types[type].thrustPower),
	 m_thrustFuelUse(vSetts->types[type].thrustFuelUse),
	 m_manuverability(vSetts->types[type].manuverability),
	 m_turnFuelUse(vSetts->types[type].turnFuelUse),
	 m_durability(vSetts->types[type].durability),
	 m_flameHeight(0.f),
	 m_lastSignal(0)
{
	 sf::Vector2f baseOrigin = sf::Vector2f(textures.get(m_model + "::base").getSize()) / 2.f;
	 sf::Vector2f baseScale(((float)m_size) / baseOrigin.x, ((float)m_size) / baseOrigin.y); 
	 
	 m_collisionMask.insert(m_collisionMask.begin(), vSetts->types[type].collisionMask.begin(),
									vSetts->types[type].collisionMask.end());
	 
	 m_baseSprite.setTexture(textures.get(m_model + "::base"));
	 m_baseSprite.setOrigin(baseOrigin);
	 m_baseSprite.setColor(vSetts->allegianceColors[allegiance]);
	 m_baseSprite.setScale(baseScale);
	 
	 m_landSprite.setTexture(textures.get(m_model + "::land"));
	 m_landSprite.setOrigin(baseOrigin);
	 m_landSprite.setColor(vSetts->allegianceColors[allegiance]);
	 m_landSprite.setScale(baseScale);

	 for(int i = 0; i < vSetts->types[type].TOFuelSprites; ++i)
	 {
		  m_fuelSprites.emplace_back(sf::Sprite());
		  m_fuelSprites.back().setTexture(textures.get(m_model + "::fuel" + std::to_string(i)));
		  m_fuelSprites.back().setOrigin(baseOrigin);
		  m_fuelSprites.back().setColor(vSetts->allegianceColors[allegiance]);
		  m_fuelSprites.back().setScale(baseScale);
	 }
	 
	 for(int i = 0; i < vSetts->types[type].TOGoldSprites; ++i)
	 {
		  m_goldSprites.emplace_back(sf::Sprite());
		  m_goldSprites.back().setTexture(textures.get(m_model + "::gold" + std::to_string(i)));
		  m_goldSprites.back().setOrigin(baseOrigin);
		  m_goldSprites.back().setColor(vSetts->allegianceColors[allegiance]);
		  m_goldSprites.back().setScale(baseScale);
	 }
	 
	 for(int i = 0; i < vSetts->TOInfoSprites; ++i)
	 {
		  m_infoSprites.emplace_back(sf::Sprite());
		  m_infoSprites.back().setTexture(textures.get(m_model + "::info" + std::to_string(i)));
		  m_infoSprites.back().setOrigin(baseOrigin);
		  m_infoSprites.back().setColor(vSetts->allegianceColors[allegiance]);
		  m_infoSprites.back().setScale(baseScale);
	 }
	 //std::cout << "SEND ME A KISS BY WIRE" << std::endl;
	 
	 m_flameSprite.setTexture(textures.get("flame"));
	 m_flameSprite.setOrigin(textures.get("flame").getSize().x / 2.f, 0.f);
	 m_flameSprite.setColor(vSetts->flameColor);
	 m_flameSprite.setScale(baseScale);

	 m_flameSize = sf::Vector2f(((float)m_size) / textures.get("flame").getSize().x,
										 ((float)m_size) / textures.get("flame").getSize().x);
}

bool Vessel::applyForce(sf::Vector2f toAdd)
{
	 m_force += toAdd;
	 m_strain += length(toAdd);
}

int Vessel::changeFuel(int change)
{
	 change = std::min(m_fuelCapacity - m_fuel, std::max(-m_fuel, change));
	 m_fuel += change;
	 m_mass += change * m_vSetts->fuelMass;

	 return change;
}

int Vessel::changeGold(int change)
{
	 change = std::min(m_goldCapacity - m_gold, std::max(-m_gold, change));
	 m_gold += change;
	 m_mass += change * m_vSetts->goldMass;

	 return change;
}

bool Vessel::thrust()
{
	 int toUse = changeFuel(m_thrustFuelUse);
	 if(toUse != 0.f)
	 {
		  m_flameHeight = std::min(m_flameHeight, 1.f) + 3*m_vSetts->flameRaise;
		  applyForce(angleToVector2f(m_angle) * ((float)toUse * m_thrustPower)/((float)m_thrustFuelUse));
		  return true;
	 }
	 
	 return false;
}

bool Vessel::turn(bool clockwise)
{
	 int toUse = changeFuel(m_turnFuelUse);
	 if(toUse != 0.f)
	 {
		  m_angularVelocity += (2*clockwise-1) * m_manuverability * ((float)toUse)/m_turnFuelUse;
		  return true;
	 }
	 
	 return false;
}

bool Vessel::tick(int time, bool& createSignal, Signal& tempSignal, std::shared_ptr<SignalSettings>& sSetts,
						bool& createRay)
{
	 if(m_strain > m_durability) return false;

	 //std::cout << length(m_velocity) << std::endl;
	 
	 m_velocity += m_force/((float)m_mass);
	 m_position += m_velocity;
	 m_force = sf::Vector2f(0.f, 0.f);
	 m_strain = 0;
	 m_angle += m_angularVelocity;
	 
	 m_flameHeight = std::max(m_flameHeight - 2*m_vSetts->flameRaise, 0.f);

	 m_baseSprite.setPosition(m_position);
	 m_baseSprite.setRotation(m_angle * 180.f/M_PI + 90.f);
	 
	 m_landSprite.setPosition(m_position);
	 m_landSprite.setRotation(m_angle * 180.f/M_PI + 90.f);

	 m_flameSprite.setPosition(m_position + angleToVector2f(m_angle + M_PI) * (float)m_size);
	 m_flameSprite.setRotation(m_angle * 180.f/M_PI + 90.f);
	 m_flameSprite.setScale(m_flameSize.x, m_flameSize.y * m_flameHeight);
	 
	 for(int i = 0; i < m_fuelSprites.size(); ++i)
	 {
		  m_fuelSprites[i].setPosition(m_position);
		  m_fuelSprites[i].setRotation(m_angle * 180.f/M_PI + 90.f);
	 }
	 
	 for(int i = 0; i < m_goldSprites.size(); ++i)
	 {
		  m_goldSprites[i].setPosition(m_position);
		  m_goldSprites[i].setRotation(m_angle * 180.f/M_PI + 90.f);
	 }
	 
	 for(int i = 0; i < m_infoSprites.size(); ++i)
	 {
		  m_infoSprites[i].setPosition(m_position);
		  m_infoSprites[i].setRotation(m_angle * 180.f/M_PI + 90.f);
	 }

	 Input in = {0};
	 m_status = m_actor->act(in);

	 if(m_status.thrust) thrust();
	 if(m_status.turnLeft) turn(false);
	 if(m_status.turnRight) turn(true);
	 if(m_status.signal && time - m_lastSignal > sSetts->maxFreq)
	 {
		  m_lastSignal = time;
		  createSignal = true;
		  tempSignal = Signal(getId(), std::to_string(time), m_position, sSetts);
	 }

	 return true;
}

void Vessel::recieve(Signal recieved)
{
	 m_actor->recieve(recieved.getSender(), recieved.getData());
}

void Vessel::centerView(sf::View& targetView)
{
	 targetView.setCenter(m_position);
}

void Vessel::draw(sf::RenderTarget& target)
{
	 int fuelLevel = std::round(((float)m_fuel)/m_fuelCapacity * m_fuelSprites.size());
	 int goldLevel = std::round(((float)m_gold)/m_goldCapacity * m_goldSprites.size());
	 //std::cout << fuelLevel << " " << goldLevel << std::endl;
	 
	 target.draw(m_baseSprite);

	 if(m_status.thrust)    target.draw(m_infoSprites[0]);
	 if(m_status.turnLeft)  target.draw(m_infoSprites[1]);
	 if(m_status.turnRight) target.draw(m_infoSprites[2]);
	 if(m_status.signal)    target.draw(m_infoSprites[3]);
	 if(m_status.load)      target.draw(m_infoSprites[4]);
	 if(m_status.unload)    target.draw(m_infoSprites[5]);
	 if(m_status.shoot)     target.draw(m_infoSprites[6]);

	 for(int i = fuelLevel; i >= 1; --i)
	 {
		  target.draw(m_fuelSprites[i-1]);
	 }

	 for(int i = goldLevel; i >= 1; --i)
	 {
		  target.draw(m_goldSprites[i-1]);
	 }

	 target.draw(m_flameSprite);

	 //target.draw(m_landSprite);
}
