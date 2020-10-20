#include "arrow.hpp"


Arrow::Arrow(sf::Vector2f origin, float length, float angle, sf::Color color, int width):
    m_origin(origin),
    m_length(length),
    m_angle(angle),
    m_color(color),
    m_width(width)
{
    calculate();
}

void Arrow::calculate()
{
    m_representation.clear();
    
    sf::Vector2f end = sf::Vector2f(m_length, 0.f);

    m_representation.push_back(sf::Vertex(sf::Vector2f(m_length, 0.f), m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(m_length - m_width*1.5f,  m_width*1.5f),
					  m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(m_length - m_width*1.5f,  m_width*0.5f),
					  m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(0.f                    ,  m_width*0.5f),
					  m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(0.f                    , -m_width*0.5f),
					  m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(m_length - m_width*1.5f, -m_width*0.5f),
					  m_color));
    m_representation.push_back(sf::Vertex(sf::Vector2f(m_length - m_width*1.5f, -m_width*1.5f),
					  m_color));

    for(int i = 0; i < m_representation.size(); ++i)
    {
	m_representation[i].position = m_origin + rotate(m_representation[i].position, m_angle);
    }
}

bool Arrow::setLength(float length)
{
    if(m_length != length)
    {
	m_length = length;
	calculate();
	return true;
    }
    return false;
}

bool Arrow::setOrigin(sf::Vector2f origin)
{
    if(m_origin != origin)
    {
	m_origin = origin;
	calculate();
	return true;
    }
    return false;
}

bool Arrow::setAngle(float angle)
{
    if(m_angle != angle)
    {
	m_angle = angle;
	calculate();
	return true;
    }
    return false;
}

bool Arrow::setDirection(sf::Vector2f from, sf::Vector2f to)
{
    m_origin = from;
    m_length = length(to - from);
    m_angle = atan2(to.x - from.x, -(to.y - from.y)) - M_PI/2.f;
    calculate();
	
    return true;
}

bool Arrow::setColor(sf::Color color)
{
    if(m_color != color)
    {
	m_color = color;
	calculate();
	return true;
    }
    return false;
}

void Arrow::draw(sf::RenderTarget& target)
{
    target.draw(&m_representation[0], m_representation.size(), sf::TriangleFan);
}
