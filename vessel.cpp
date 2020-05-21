#include "vessel.hpp"


Vessel::Vessel(std::shared_ptr<Actor> actor, std::string name, sf::Vector2f position, int type,
	       int allegiance, const std::shared_ptr<VesselSettings> vSetts,
	       ResourceHolder<sf::Texture, std::string>& textures):
    m_vSetts(vSetts),
    m_actor(actor),
    m_name(name),
    m_model(vSetts->types[type].model),
    m_allegiance(allegiance),
    m_position(position),
    m_angle(M_PI/2.f),
    m_velocity(0.f, 0.f),
    m_angularVelocity(0.f),
    m_force(0.f, 0.f),
    m_strain(0.f),
    m_fuel(vSetts->startingFuel),
    m_gold(0),
    m_baseMass(vSetts->types[type].baseMass),
    m_mass(m_baseMass + m_fuel * vSetts->fuelMass + m_gold * vSetts->goldMass),
    m_size(vSetts->types[type].size),
    m_fuelCapacity(vSetts->types[type].fuelCapacity),
    m_goldCapacity(vSetts->types[type].goldCapacity),
    m_miningSpeed(vSetts->types[type].miningSpeed),
    m_shootingFreq(vSetts->types[type].shootingFreq),
    m_rayResistance(vSetts->types[type].rayResistance),
    m_thrustPower(vSetts->types[type].thrustPower),
    m_thrustFuelUse(vSetts->types[type].thrustFuelUse),
    m_manuverability(vSetts->types[type].manuverability),
    m_turnFuelUse(vSetts->types[type].turnFuelUse),
    m_durability(vSetts->types[type].durability),
    m_flameHeight(0.f),
    m_lastSignal(0),
    m_lastRay(0),
    m_nextBarrel(0),
    m_hitFade(0),
    m_recievedFade(0),
    m_dead(false),
    m_deathTimer(vSetts->deathTimer)
{
    std::cout << getId() << std::endl;
    
    sf::Vector2f baseOrigin = sf::Vector2f(textures.get(m_model + "::primary").getSize())/2.f;
    sf::Vector2f baseScale(((float)m_size) / baseOrigin.x / 2.f, ((float)m_size) / baseOrigin.y / 2.f); 
	 
    /*m_dynColMask.insert(m_dynColMask.begin(), vSetts->types[type].collisionMask.begin(),
      vSetts->types[type].collisionMask.end());*/

    for(int i = 0; i < vSetts->types[type].collisionMask.size(); ++i)
    {
	m_staColMask.push_back(vSetts->types[type].collisionMask[i] * ((float)m_size));
	m_dynColMask.push_back(m_staColMask.back());
	m_colMaskRepr.emplace_back(m_dynColMask.back() + m_position,
				      vSetts->factions[allegiance].primaryColor);
    }
    m_colMaskRepr.emplace_back(m_dynColMask[0] + m_position, vSetts->factions[allegiance].primaryColor);
	 
    for(int i = 0; i < vSetts->types[type].barrels.size(); ++i)
    {
	m_barrels.push_back(vSetts->types[type].barrels[i] * ((float)m_size));
    }
	 
    /*m_barrels.insert(m_barrels.begin(), vSetts->types[type].barrels.begin(),
      vSetts->types[type].barrels.end());*/
	 
    m_primarySprite.setTexture(textures.get(m_model + "::primary"));
    m_primarySprite.setOrigin(baseOrigin);
    m_primarySprite.setColor(vSetts->factions[allegiance].primaryColor);
    m_primarySprite.setScale(baseScale);
    
    m_secondarySprite.setTexture(textures.get(m_model + "::secondary"));
    m_secondarySprite.setOrigin(baseOrigin);
    m_secondarySprite.setColor(vSetts->factions[allegiance].secondaryColor);
    m_secondarySprite.setScale(baseScale);
	 
    m_landSprite.setTexture(textures.get(m_model + "::land"));
    m_landSprite.setOrigin(baseOrigin);
    m_landSprite.setColor(vSetts->factions[allegiance].primaryColor);
    m_landSprite.setScale(baseScale);

    for(int i = 0; i < vSetts->types[type].TOFuelSprites; ++i)
    {
	m_fuelSprites.emplace_back(sf::Sprite());
	m_fuelSprites.back().setTexture(textures.get(m_model + "::fuel" + std::to_string(i)));
	m_fuelSprites.back().setOrigin(baseOrigin);
	m_fuelSprites.back().setColor(vSetts->factions[allegiance].primaryColor);
	m_fuelSprites.back().setScale(baseScale);
    }
	 
    for(int i = 0; i < vSetts->types[type].TOGoldSprites; ++i)
    {
	m_goldSprites.emplace_back(sf::Sprite());
	m_goldSprites.back().setTexture(textures.get(m_model + "::gold" + std::to_string(i)));
	m_goldSprites.back().setOrigin(baseOrigin);
	m_goldSprites.back().setColor(vSetts->factions[allegiance].primaryColor);
	m_goldSprites.back().setScale(baseScale);
    }
	 
    for(int i = 0; i < vSetts->TOInfoSprites; ++i)
    {
	m_infoSprites.emplace_back(sf::Sprite());
	m_infoSprites.back().setTexture(textures.get(m_model + "::info" + std::to_string(i)));
	m_infoSprites.back().setOrigin(baseOrigin);
	m_infoSprites.back().setColor(vSetts->factions[allegiance].primaryColor);
	m_infoSprites.back().setScale(baseScale);
    }
	 
    m_flameSprite.setTexture(textures.get("flame"));
    m_flameSprite.setOrigin(textures.get("flame").getSize().x / 2.f, 0.f);
    m_flameSprite.setColor(vSetts->flameColor);
    m_flameSprite.setScale(baseScale);

    m_flameSize = sf::Vector2f(((float)m_size) / textures.get("flame").getSize().x,
			       ((float)m_size) / textures.get("flame").getSize().x);

    m_velArrow = Arrow();
    m_velArrow.setColor(m_vSetts->velArrowColor);
    m_forArrow = Arrow();
    m_forArrow.setColor(m_vSetts->forArrowColor);
    m_path.push_back(sf::Vertex(m_position, sf::Color::White));
}

void Vessel::applyForce(sf::Vector2f toAdd)
{
    m_force += toAdd;
}

void Vessel::applyStrain(float toAdd)
{
    m_strain += toAdd;
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
	applyForce(angleToVector2f(m_angle - M_PI/2.f)
		   * ((float)toUse * m_thrustPower)/((float)m_thrustFuelUse));
	return true;
    }
	 
    return false;
}

bool Vessel::turn(bool clockwise)
{
    int toUse = changeFuel(m_turnFuelUse);

    if(fabs(m_manuverability * ((float)toUse)/m_turnFuelUse) > fabs(m_angularVelocity) &&
       fabs(m_angularVelocity) > 0.f)
    {
	m_angularVelocity = 0.f;
	return true;
    }
    else if(toUse != 0.f)
    {
	m_angularVelocity += boolToSign(clockwise) * m_manuverability * ((float)toUse)/m_turnFuelUse;
	return true;
    }
	 
    return false;
}

bool Vessel::tick(int time, bool& createSignal, Signal& tempSignal, std::shared_ptr<SignalSettings>& sSetts,
		  bool& createRay, Ray& tempRay, std::shared_ptr<RaySettings>& rSetts)
{
    if(m_strain > m_durability || m_rayResistance <= 0)
    {
	m_dead = true;
	m_allegiance = 0;
    }
	 
    m_velocity += m_force/((float)m_mass);
    m_position += m_velocity;

    m_path.push_back(sf::Vertex(m_position, sf::Color::White));
    if(m_path.size() > m_vSetts->pathLimit) m_path.erase(m_path.begin());
    m_velArrow.setDirection(m_position, 60.f*m_velocity + m_position);
    m_forArrow.setDirection(m_position, m_force + m_position);
    
    m_force = sf::Vector2f(0.f, 0.f);
    m_strain = 0;
    m_angle += m_angularVelocity;

    if(!m_dead)
    {
	m_flameHeight = std::max(m_flameHeight - 2*m_vSetts->flameRaise, 0.f);

	m_hitFade      = std::max(0, m_hitFade      - 1);
	m_recievedFade = std::max(0, m_recievedFade - 1);

	if(m_primarySprite.getColor() != m_vSetts->factions[m_allegiance].primaryColor ||
	   m_hitFade > 0 || m_recievedFade > 0)
	{
	    m_primarySprite.setColor(
		   colorStep(m_primarySprite.getColor(),
			     m_vSetts->factions[m_allegiance].primaryColor,
			     std::max(m_hitFade, m_recievedFade)));
	    m_secondarySprite.setColor(
		   colorStep(m_secondarySprite.getColor(),
			     m_vSetts->factions[m_allegiance].secondaryColor,
			     std::max(m_hitFade, m_recievedFade)));
	}
    }
    else
    {
	m_flameHeight = 0.f;
	m_hitFade = 0;
	m_recievedFade = 0;

	m_primarySprite.setColor(m_vSetts->factions[0].primaryColor);
    }

    m_primarySprite.setPosition(m_position);
    m_primarySprite.setRotation(radToDeg(m_angle));
    
    m_secondarySprite.setPosition(m_position);
    m_secondarySprite.setRotation(radToDeg(m_angle));
	 
    m_landSprite.setPosition(m_position);
    m_landSprite.setRotation(radToDeg(m_angle));

    m_flameSprite.setPosition(m_position + angleToVector2f(m_angle + M_PI/2.f) * (float)m_size / 2.f);
    m_flameSprite.setRotation(radToDeg(m_angle));
    m_flameSprite.setScale(m_flameSize.x, m_flameSize.y * m_flameHeight);

    for(int i = 0; i < m_fuelSprites.size(); ++i)
    {
	m_fuelSprites[i].setPosition(m_position);
	m_fuelSprites[i].setRotation(radToDeg(m_angle));
    }
    
    for(int i = 0; i < m_goldSprites.size(); ++i)
    {
	m_goldSprites[i].setPosition(m_position);
	m_goldSprites[i].setRotation(radToDeg(m_angle));
	}
    
    for(int i = 0; i < m_infoSprites.size(); ++i)
    {
	m_infoSprites[i].setPosition(m_position);
	m_infoSprites[i].setRotation(radToDeg(m_angle));
    }

    for(int i = 0; i <= m_dynColMask.size(); ++i)
    {
        if(i < m_staColMask.size())
	    m_dynColMask[i] = rotate(m_staColMask[i], m_angle) + m_position;
	m_colMaskRepr[i].position = m_dynColMask[modulo(i, m_dynColMask.size())];
    }

    m_sphereMask.setPosition(m_position.x - m_size/2.f, m_position.y - m_size/2.f);

    Input in = {0};
    if(!m_dead) m_status = m_actor->act(in);
    else m_status = {false, false, false, false, false, false, false};

    if(m_status.thrust) thrust();
    if(m_status.turnLeft) turn(false);
    if(m_status.turnRight) turn(true);
    if(m_status.signal && sSetts->maxFreq != 0 && time - m_lastSignal > sSetts->maxFreq)
    {
	m_lastSignal = time;
	createSignal = true;
	tempSignal = Signal(getId(), std::to_string(time), m_position, sSetts);
    }
    if(m_status.shoot && m_shootingFreq != 0 && time - m_lastRay > m_shootingFreq)
    {
	m_lastRay = time;
	createRay = true;
	//std::cout << m_barrels[m_nextBarrel].x << " " << m_barrels[m_nextBarrel].y << std::endl;
	tempRay = Ray(getId(), m_angle, m_position +
		      rotate(m_barrels[m_nextBarrel], m_angle), m_velocity, rSetts);
	m_nextBarrel = modulo(m_nextBarrel + 1, m_barrels.size());
    }

    if(!m_status.turnLeft && !m_status.turnRight)
    {
	if(m_angularVelocity > 0.f) turn(false);
	else if(m_angularVelocity < 0.f) turn(true);
    }

    return !m_dead;
}

void Vessel::recieve(Signal recieved)
{
    if(!m_dead)
    {
	m_actor->recieve(recieved.getSender(), recieved.getData());
    
	m_recievedFade = m_vSetts->actionShadeFade;
	m_primarySprite.setColor(recieved.getColor());
	m_secondarySprite.setColor(recieved.getColor());
    }
}

void Vessel::getHit(Ray hitting)
{
    if(!m_dead)
    {
	std::cout << getId() + ": I GOT HIT - " << m_rayResistance << " hits left" << std::endl;
	--m_rayResistance;
    
	m_hitFade = m_vSetts->actionShadeFade;
	m_primarySprite.setColor(hitting.getColor());
	m_secondarySprite.setColor(hitting.getColor());
    }
}

void Vessel::centerView(sf::View& targetView)
{
    targetView.setCenter(m_position);
    targetView.setRotation(radToDeg(m_angle));
}

void Vessel::draw(sf::RenderTarget& target, bool debug)
{
    int fuelLevel = std::round(((float)m_fuel)/m_fuelCapacity * m_fuelSprites.size());
    int goldLevel;
    if(m_goldCapacity != 0) goldLevel= std::round(((float)m_gold)/m_goldCapacity * m_goldSprites.size());
    else goldLevel = 0;
    
    if(!m_dead) target.draw(m_secondarySprite);
    target.draw(m_primarySprite);
    // target.draw(m_landSprite);

    if(!m_dead)
    {
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
    }

    if(debug)
    {
	target.draw(&m_colMaskRepr[0], m_colMaskRepr.size(), sf::LineStrip);

	m_velArrow.draw(target);
	
	m_forArrow.draw(target);

	target.draw(&m_path[0], m_path.size(), sf::LineStrip);
    }
}

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

bool resolveCollision(Vessel& A, Vessel& B)
{
    sf::Vector2f connector = B.getPosition() - A.getPosition();
	    
    if(length(connector) < (A.getSize()/2 + B.getSize()/2))
    {
	bool collision = false;
	for(int i = 0; i < A.getMask().size(); ++i)
	{
	    for(int j = 0; j < B.getMask().size(); ++j)
	    {
		if(linesCollision(A.getMask()[       i                       ],
				  A.getMask()[modulo(i+1, A.getMask().size())],
				  B.getMask()[       j                       ],
				  B.getMask()[modulo(j+1, B.getMask().size())]))
		{
		    //std::cout << i << " " << j << std::endl;
		    collision = true;
		}
		if(collision) break;
	    }
	    if(collision) break;
	}

        if(collision)
	{
	    float alongDir = dotProduct(B.getVelocity() - A.getVelocity(), connector/length(connector));
	    
	    if(alongDir > 0) return false;
	    
	    sf::Vector2f impulse(0.f, 0.f);
	    float bounce = std::min(A.getBounce(), B.getBounce());
	    
	    impulse = -alongDir * connector/length(connector)/(1.f/A.getMass() + 1.f/B.getMass());

	    A.applyForce (-impulse * (1 + bounce));
	    A.applyStrain(length(impulse) * (1 - bounce));
	    B.applyForce( impulse * (1 + bounce));
	    A.applyStrain(length(impulse) * (1 - bounce));

	    return true;
	}
    }
    return false;
}
