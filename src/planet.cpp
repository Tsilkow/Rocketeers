#include "planet.hpp"


void connectPeaks(std::vector<int>& result, int begin, int end, int amplitude, int loop)
{
    if(end - begin < 2) return;
    int middle = (begin + end)/2;
    ++loop;
	 
    result[middle] = (result[begin] + result[end])/2 + randomI(-amplitude/loop, amplitude/loop);
    connectPeaks(result, begin, middle, amplitude, loop);
    connectPeaks(result, middle,   end, amplitude, loop);
}

std::vector<int> generateHeights(int base, int TOSegments, int amplitude, float smoothness, int peakFrequency, int peakVariation, int peakAmplitude)
{
    std::vector<int> result(TOSegments);
    std::vector<int> peaks;
    int peakIndex = 0;

    peaks.emplace_back(0);
    result[peaks.back()] = base;
    for(int i = 1; i < TOSegments/peakFrequency; ++i)
    {
	peaks.emplace_back(peakFrequency * i + randomI(-peakVariation, peakVariation));
	result[peaks.back()] = base + randomI(-peakAmplitude, peakAmplitude);
    }
    peaks.emplace_back(TOSegments);
    result.emplace_back(base);

    for(int i = 1; i < peaks.size(); ++i)
    {
	connectPeaks(result, peaks[i-1], peaks[i], amplitude, 0);
    }

    for(int i = 1; i < TOSegments; ++i)
    {
	result[i] -= (result[i] - (result[i-1] + result[modulo(i+1, TOSegments)])/2) * smoothness;
    }

    return result;
}

Planet::Planet(const std::shared_ptr<PlanetSettings> pSetts, std::string name, int radius, int mass, sf::Vector2f position, float angVelocity, int TOSegments, int amplitude, int smoothness, int peakFrequency, int peakVariation, int peakAmplitude, sf::Color Cterrain, int atmHeight, float atmVanishFact, sf::Color Catmosphere):
    m_pSetts(pSetts),
    m_name(name),
    m_radius(radius),
    m_mass(mass),
    m_position(position),
    m_velocity(0.f, 0.f),
    m_angle(0.f),
    m_angVelocity(angVelocity),
    m_atmHeight(atmHeight),
    m_atmVanishFact(atmVanishFact),
    m_Cterrain(Cterrain),
    m_Catmosphere(Catmosphere)
{
    m_segmentSize = 2 * m_radius * sin(M_PI/((float)TOSegments));
    std::cout << M_PI/((float)TOSegments) << std::endl;
    m_heights = generateHeights(m_radius, TOSegments, amplitude, smoothness, peakFrequency, peakVariation, peakAmplitude);

    int atmosphereStart = m_radius - peakAmplitude;
    sf::Color CatmosphereEdge(m_Catmosphere.r, m_Catmosphere.g, m_Catmosphere.b, 0);

    m_statSurface.emplace_back(0.f, 0.f);
    m_dynSurface.emplace_back(m_statSurface.back(), m_Cterrain);
    for(int i = 0; i <= TOSegments; ++i)
    {
	sf::Vector2f direction = angleToVector2f(2 * M_PI * modulo(i, TOSegments)/TOSegments);
	// surface is constructed using triangles connected at a common vertice in the middle of the planet (sf::TriangleFan)
	m_statSurface.emplace_back(direction * (float)m_heights[modulo(i, TOSegments)]);
	m_dynSurface.emplace_back(m_statSurface.back(), m_Cterrain);
	
	// atmosphere is constructed using connected triangles (sf::TriangleStrip), which form a quad closer to the actual surface | result is a nice ring a
	m_statAtmosphere.emplace_back(direction * (float)atmosphereStart);
	m_dynAtmosphere.emplace_back(m_statAtmosphere.back(), m_Catmosphere);
	m_statAtmosphere.emplace_back(direction * (float)(m_radius + m_atmHeight));
	m_dynAtmosphere.emplace_back(m_statAtmosphere.back(), CatmosphereEdge);

	//printVector(direction * (float)atmosphereStart, true);
	//printVector(direction * (float)(m_radius + m_atmHeight), true);
	//std::cout << "(" << atmosphereStart << " " << m_radius + m_atmHeight << ")" << std::endl;
    }

    m_heightHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
    m_heightHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
    
    m_surfaceHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
    m_surfaceHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
    m_surfaceHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
    m_surfaceHighlight.emplace_back(m_position, sf::Color(255, 0, 0));
}

void Planet::tick()
{
    sf::Transform R;
    m_position += m_velocity;
    //std::cout << "m_angle + m_angVelocity = " << m_angle << " + " << m_angVelocity << std::endl;
    m_angle = modulo(m_angle + m_angVelocity, 2*M_PI);
    //std::cout << "...  = " << m_angle << std::endl;

    R.rotate(radToDeg(m_angle));

    for(int i = 0; i < m_dynSurface.size(); ++i)
    {
	m_dynSurface[i].position = R.transformPoint(m_statSurface[i]) + m_position;
    }
    
    for(int i = 0; i < m_dynAtmosphere.size(); ++i)
    {
	m_dynAtmosphere[i].position = R.transformPoint(m_statAtmosphere[i]) + m_position;
    }

}

sf::Vector2f Planet::exertForce(sf::Vector2f objectPosition, sf::Vector2f objectVelocity,
				float objectSize, int objectMass)
{
    sf::Vector2f connector = objectPosition - m_position;
    float angle = atan2(connector.y, connector.x);
    sf::Vector2f relVelocity = objectVelocity - getVelocityAt(angle, length(connector));
	
    sf::Vector2f result(0.f, 0.f);

    // gravity
    result += -connector *
	m_pSetts->gravConst * (float)m_mass * (float)objectMass /
	raiseToPower(length(-connector), 3);
    
    //friction
    result += -0.5f * m_pSetts->airFriConst * objectSize *
	std::max(0.f, std::min(1.f, (m_radius + m_atmHeight - length(connector))/((float)m_atmHeight))) *
	(relVelocity/length(relVelocity)) * (float)pow(length(relVelocity), 2);

    /*std::cout << "{" << m_pSetts->gravConst * (float)m_mass * (float)objectMass << " " << length(m_position - objectPosition) << " ";
    printVector(result, false);
    std::cout << "}" << std::endl;*/

    return result;
}

float Planet::getAngleTo(sf::Vector2f target)
{
    sf::Vector2f connector = target - m_position;
    
    return atan2(connector.y, connector.x);
}

std::pair<int, float> Planet::getSegmentAt(float angle)
{
    float point = modulo(angle - m_angle, 2*M_PI)/(2*M_PI) * (m_dynSurface.size()-2);

    return std::pair<int, float>((int)std::floor(point), point - std::floor(point));
}

std::vector<sf::Vector2f> Planet::getSurfaceAt(float angle)
{
    std::vector<sf::Vector2f> result;

    int middle = getSegmentAt(angle).first;

    for(int i = -1; i <= 2; ++i)
    {
	result.push_back(m_dynSurface[1+modulo(middle+i, m_dynSurface.size()-2)].position);
	m_surfaceHighlight[i+1].position = result.back();
    }

    return result;
}

float Planet::getHeightAt(float angle)
{
    float result;
    std::pair<int, float> point = getSegmentAt(angle);

    result =
	m_heights[       point.first                         ] * (1.f - point.second) +
	m_heights[modulo(point.first + 1, m_heights.size()-1)] * (      point.second);

    m_heightHighlight[0].position = m_position;
    m_heightHighlight[1].position = m_position + result * angleToVector2f(angle);

    return result;
}

sf::Vector2f Planet::getSurfacePoint(float angle)
{
    return m_position + getHeightAt(angle) * angleToVector2f(angle);
}

sf::Vector2f Planet::getVelocityAt(float angle, float height)
{
    sf::Vector2f result = angleToVector2f(angle + M_PI/2.f) * m_angVelocity * height + m_velocity;
    sf::Vector2f start = m_position + height * angleToVector2f(angle);
    
    m_velocityArrow.setDirection(start, start + result*60.f);
    
    return result;
}

sf::Vector2f Planet::getSurfaceVector(float angle)
{
    sf::Vector2f surface = getSurfaceAt(angle)[2] - getSurfaceAt(angle)[1];
    sf::Vector2f start = getSurfacePoint(angle);
    sf::Vector2f result = angleToVector2f(modulo(atan2(surface.y, surface.x) - M_PI/2.f, 2*M_PI));
    
    m_surfaceVector.setDirection(start, start + result);
    
    return result;
}

void Planet::draw(sf::RenderTarget& target, bool orbit)
{
    target.draw(&m_dynAtmosphere[0], m_dynAtmosphere.size(), sf::TriangleStrip);
    target.draw(&m_dynSurface[0], m_dynSurface.size(), sf::TriangleFan);
    m_velocityArrow.draw(target);
    m_surfaceVector.draw(target);
    target.draw(&m_heightHighlight[0], m_heightHighlight.size(), sf::LineStrip);
    target.draw(&m_surfaceHighlight[0], m_surfaceHighlight.size(), sf::LineStrip);
}
