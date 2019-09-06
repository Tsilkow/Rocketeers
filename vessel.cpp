#include "vessel.hpp"



float length(sf::Vector2f distance)
{
	 return (distance.x*distance.x + distance.y*distance.y);
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

Vessel::Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type, int allegiance,
					const std::shared_ptr<VesselSettings>& vSetts):
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
	 m_shootingSpeed(vSetts->types[type].shootingSpeed),
	 m_thrustPower(vSetts->types[type].thrustPower),
	 m_thrustFuelUse(vSetts->types[type].thrustFuelUse),
	 m_manuverability(vSetts->types[type].manuverability),
	 m_turnFuelUse(vSetts->types[type].turnFuelUse),
	 m_durability(vSetts->types[type].durability),
	 m_baseTexture(vSetts->types[type].baseTexture),
	 m_landTexture(vSetts->types[type].landTexture),
	 m_flameTexture(vSetts->flameTexture)
{
	 m_collisionMask.insert(m_collisionMask.begin(), vSetts->types[type].collisionMask.begin(),
									vSetts->types[type].collisionMask.end());
	 m_fuelTextures.insert(m_fuelTextures.begin(), vSetts->types[type].fuelTextures.begin(),
								 vSetts->types[type].fuelTextures.end());
	 m_infoTextures.insert(m_infoTextures.begin(), vSetts->types[type].infoTextures.begin(),
								 vSetts->types[type].infoTextures.end());
	 
	 m_landSprite.setTexture(m_landTexture);
	 m_landSprite.setOrigin(m_baseTexture.getSize().x/2.f, m_baseTexture.getSize().y/2.f);
	 m_landSprite.setColor(vSetts->allegianceColors[allegiance]);
	 m_landSprite.setScale(((float)m_size)/m_baseTexture.getSize().x,
								  ((float)m_size)/m_baseTexture.getSize().y);

	 for(int i = 0; i < 6; ++i)
	 {
		  m_fuelSprites.emplace_back(sf::Sprite());
		  m_fuelSprites.back().setTexture(m_fuelTextures[i]);
		  m_fuelSprites.back().setOrigin(m_baseTexture.getSize().x/2.f, m_baseTexture.getSize().y/2.f);
		  m_fuelSprites.back().setColor(vSetts->allegianceColors[allegiance]);
		  m_fuelSprites.back().setScale(((float)m_size)/m_baseTexture.getSize().x,
												  ((float)m_size)/m_baseTexture.getSize().y);
	 }
	 
	 for(int i = 0; i < 7; ++i)
	 {
		  m_infoSprites.emplace_back(sf::Sprite());
		  m_infoSprites.back().setTexture(m_infoTextures[i]);
		  m_infoSprites.back().setOrigin(m_baseTexture.getSize().x/2.f, m_baseTexture.getSize().y/2.f);
		  m_infoSprites.back().setColor(vSetts->allegianceColors[allegiance]);
		  m_infoSprites.back().setScale(((float)m_size)/m_baseTexture.getSize().x,
												  ((float)m_size)/m_baseTexture.getSize().y);
	 }
	 
	 m_flameSprite.setTexture(m_flameTexture);
	 m_flameSprite.setOrigin(m_baseTexture.getSize().x/2.f, m_baseTexture.getSize().y/2.f);
	 m_flameSprite.setColor(vSetts->flameColor);
	 m_flameSprite.setScale(((float)m_size)/m_baseTexture.getSize().x,
									((float)m_size)/m_baseTexture.getSize().y);
	 //m_sprite.setPosition(m_position - sf::Vector2f(m_size, m_size)/2.f);
	 
	 m_baseSprite.setTexture(m_baseTexture);
	 m_baseSprite.setOrigin(m_baseTexture.getSize().x/2.f, m_baseTexture.getSize().y/2.f);
	 m_baseSprite.setColor(vSetts->allegianceColors[allegiance]);
	 m_baseSprite.setScale(((float)m_size)/m_baseTexture.getSize().x,
								  ((float)m_size)/m_baseTexture.getSize().y);
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
		  applyForce(angleToVector2f(m_angle) * ((float)toUse * m_thrustPower)/((float)m_thrustFuelUse));
		  return true;
	 }

	 std::cout << "No thrust, no sir" << std::endl;
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

bool Vessel::tick()
{
	 if(m_strain > m_durability) return false;

	 std::cout << length(m_velocity) << std::endl;
	 
	 m_velocity += m_force/((float)m_mass);
	 m_position += m_velocity;
	 m_force = sf::Vector2f(0.f, 0.f);
	 m_strain = 0;
	 m_angle += m_angularVelocity;

	 m_baseSprite.setPosition(m_position);
	 m_baseSprite.setRotation(m_angle * 180.f/M_PI + 90.f);
	 
	 m_landSprite.setPosition(m_position);
	 m_landSprite.setRotation(m_angle * 180.f/M_PI + 90.f);

	 m_flameSprite.setPosition(m_position);
	 m_flameSprite.setRotation(m_angle * 180.f/M_PI + 90.f);
	 
	 for(int i = 0; i < 6; ++i)
	 {
		  m_fuelSprites[i].setPosition(m_position);
		  m_fuelSprites[i].setRotation(m_angle * 180.f/M_PI + 90.f);
	 }
	 
	 for(int i = 0; i < 7; ++i)
	 {
		  m_infoSprites[i].setPosition(m_position);
		  m_infoSprites[i].setRotation(m_angle * 180.f/M_PI + 90.f);
	 }

	 Input in = {0};
	 m_status = m_actor->act(in);
	 
	 std::cout << m_status.thrust << m_status.turnLeft << m_status.turnRight << m_status.signal
				  << m_status.load << m_status.shoot << std::endl;

	 if(m_status.thrust) thrust();
	 if(m_status.turnLeft) turn(false);
	 if(m_status.turnRight) turn(true);

	 return true;
}

void Vessel::draw(sf::RenderTarget& target)
{
	 int fuelLevel = std::round(((float)m_fuel)/m_fuelCapacity * 6);
	 
	 target.draw(m_baseSprite);

	 if(m_status.thrust)    target.draw(m_infoSprites[0]);
	 if(m_status.turnLeft)  target.draw(m_infoSprites[1]);
	 if(m_status.turnRight) target.draw(m_infoSprites[2]);
	 if(m_status.signal)    target.draw(m_infoSprites[3]);
	 if(m_status.load)      target.draw(m_infoSprites[4]);
	 if(m_status.unload)    target.draw(m_infoSprites[5]);
	 if(m_status.shoot)     target.draw(m_infoSprites[6]);

	 switch(fuelLevel)
	 {
		  case 6: target.draw(m_fuelSprites[5]);
		  case 5: target.draw(m_fuelSprites[4]);
		  case 4: target.draw(m_fuelSprites[3]);
		  case 3: target.draw(m_fuelSprites[2]);
		  case 2: target.draw(m_fuelSprites[1]);
		  case 1: target.draw(m_fuelSprites[0]);
	 }

	 if(m_status.thrust) target.draw(m_flameSprite);

	 target.draw(m_landSprite);
}
