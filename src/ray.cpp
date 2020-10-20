#include "ray.hpp"


Ray::Ray(std::string sender, float angle, sf::Vector2f position, sf::Vector2f velocity,
	 std::shared_ptr<RaySettings>& rSetts):
    m_sender(sender),
    m_color(rSetts->color),
    m_position(position),
    m_angle(angle - M_PI/2.f),
    m_velocity(velocity + angleToVector2f(m_angle) * ((float)m_rSetts->speed)),
    m_rSetts(rSetts),
    m_length(rSetts->dimensions.y),
    m_ticksPassed(0),
    m_hit(false)
{
    m_rectangle = sf::RectangleShape(rSetts->dimensions);
    m_rectangle.setFillColor(m_color);
    m_rectangle.setOrigin(rSetts->dimensions.x / 2.f, rSetts->dimensions.x / 2.f);
    m_rectangle.setPosition(m_position);
    m_rectangle.setRotation(radToDeg(m_angle - M_PI/2.f));
}

bool Ray::tick()
{
    sf::Color temp = m_rSetts->color;

    ++m_ticksPassed;
    if(m_ticksPassed >= m_rSetts->life || m_hit) return false;

    m_position += angleToVector2f(m_angle) * ((float)m_rSetts->speed);
    m_rectangle.setPosition(m_position);
	 
    temp.a = std::max(0, std::min(255, (int)
				  std::round((m_rSetts->life - m_ticksPassed) * 510.f/m_rSetts->life)));
    m_rectangle.setFillColor(temp);

    return true;
}

bool Ray::hit(std::vector<sf::Vector2f> mask, sf::Vector2f position, int size)
{
    if(length(m_position - position) <= m_length + size)
    {
	for(int i = 0; i <= mask.size(); ++i)
	{
	    /*printVector(mask[i] + position                                       , true);
	      printVector(mask[modulo(i+1, mask.size())] + position                , true);
	      printVector(m_position                                               , true);
	      printVector(m_position + angleToVector2f(m_angle) * ((float)m_length), true);*/
		  
	    if(linesCollision(mask[i], mask[modulo(i+1, mask.size())],
			      m_position, m_position + angleToVector2f(m_angle) * ((float)m_length)))
	    {
		m_hit = true;
					 
		return true;
	    }
	}
    }
    return false;
}

void Ray::draw(sf::RenderTarget& window)
{
    window.draw(m_rectangle);
}
