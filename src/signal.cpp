#include "signal.hpp"


Signal::Signal(std::string sender, std::string data, sf::Vector2f center,
	       std::shared_ptr<SignalSettings>& sSetts):
    m_sender(sender),
    m_data(data),
    m_color(sSetts->color),
    m_center(center),
    m_sSetts(sSetts)
{
    m_ticksPassed = 0;
    m_circle.setPosition(m_center);
    m_circle.setRadius(0);
    m_circle.setOutlineColor(m_color);
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
    float ls = length(start - m_center);
    float le = length(end - m_center);
    float now = m_ticksPassed * m_sSetts->speed;
    float past = (m_ticksPassed-1) * m_sSetts->speed;

    /*std::cout << length(start) << " | " << ls << std::endl;
    std::cout << length(end  ) << " | " << le << std::endl;*/
	 

    /*if((ls > past && ls <= now) || (le > past && le <= now) ||
       (ls >= now && le < past) || (le >= now && ls < past))*/
    if((ls <= past && le >= now) || (ls >= now && le <= now) ||
       (ls >= past && le <= now) || (ls >= past && le <= past))
    { 
	return true;
    }

    return false;
}

void Signal::draw(sf::RenderTarget& window)
{
    window.draw(m_circle);
}
