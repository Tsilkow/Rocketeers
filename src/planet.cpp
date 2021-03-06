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

    m_dynSurface.emplace_back(sf::Vector2f(0.f, 0.f), m_Cterrain);
    m_dynAtmosphere.emplace_back(sf::Vector2f(0.f, 0.f), m_Catmosphere);
    for(int i = 0; i <= TOSegments; ++i)
    {
	m_statSurface.emplace_back(sin(2 * M_PI * modulo(i, TOSegments)/TOSegments) *
				   m_heights[modulo(i, TOSegments)],
				   cos(2 * M_PI * modulo(i, TOSegments)/TOSegments) *
				   m_heights[modulo(i, TOSegments)]);
	m_dynSurface.emplace_back(m_statSurface.back(), m_Cterrain);
	
	m_statAtmosphere.emplace_back(sin(2 * M_PI * modulo(i, TOSegments)/TOSegments) * m_atmHeight,
				      cos(2 * M_PI * modulo(i, TOSegments)/TOSegments) * m_atmHeight);
	m_dynAtmosphere.emplace_back(m_statAtmosphere.back(), sf::Color(0, 0, 0, 0));
    }
}

void Planet::tick()
{
    sf::Transform R;
    sf::Vector2f oldPos = m_position;
    m_position += m_velocity;
    m_angle += m_angVelocity;

    R.rotate(radToDeg(m_angVelocity));

    for(int i = 0; i < m_dynSurface.size(); ++i)
    {
	m_dynSurface[i].position = R.transformPoint(m_dynSurface[i].position - oldPos) + m_position;
    }
    
    for(int i = 0; i < m_dynAtmosphere.size(); ++i)
    {
	m_dynAtmosphere[i].position = R.transformPoint(m_dynAtmosphere[i].position - oldPos) + m_position;
    }
}

void Planet::draw(sf::RenderTarget& target, bool orbit)
{
    target.draw(&m_dynAtmosphere[0], m_dynAtmosphere.size(), sf::TriangleFan);
    target.draw(&m_dynSurface[0], m_dynSurface.size(), sf::TriangleFan);
}
