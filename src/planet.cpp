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
	sf::Vector2f direction(sin(2 * M_PI * modulo(i, TOSegments)/TOSegments),
			       cos(2 * M_PI * modulo(i, TOSegments)/TOSegments));
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
}

void Planet::tick()
{
    sf::Transform R;
    m_position += m_velocity;
    m_angle += m_angVelocity;

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

sf::Vector2f Planet::exertForce(sf::Vector2f objectPosition, int objectMass)
{
    sf::Vector2f result = (m_position - objectPosition) *
	m_pSetts->gravConst * (float)m_mass * (float)objectMass /
	raiseToPower(length(m_position - objectPosition), 3);

    /*std::cout << "{" << m_pSetts->gravConst * (float)m_mass * (float)objectMass << " " << length(m_position - objectPosition) << " ";
    printVector(result, false);
    std::cout << "}" << std::endl;*/

    return result;
}

void Planet::draw(sf::RenderTarget& target, bool orbit)
{
    target.draw(&m_dynAtmosphere[0], m_dynAtmosphere.size(), sf::TriangleStrip);
    target.draw(&m_dynSurface[0], m_dynSurface.size(), sf::TriangleFan);
}
