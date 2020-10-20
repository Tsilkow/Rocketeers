#include "commons.hpp"


int boolToSign(bool input)
{
    if(input) return 1;
    return -1;
}

std::string strPrecRound(double value, short precision)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << std::fixed << value;
    return ss.str();
}

float raiseToPower(float value, int power)
{
    if(power == 0) return 1;
    if(power == 1) return value;
    if(power % 2 == 0) return raiseToPower(value*value, power/2);
    else return value * raiseToPower(value, power-1);
}

int randomI(int min, int max)
{
    if(max == min) return min;
    return rand() % (max - min + 1) + min;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min/step), std::round(max/step)) * step;
}

bool randomB()
{
    return (rand()%2 == 0);
}

char randomSa()
{
    return Samo[randomI(0, Samo.size())];
}

char randomSp()
{
    return Spol[randomI(0, Spol.size())];
}

std::vector<int> range(int min, int max)
{
    std::vector<int> result;

    for(int i = min; i <= max; ++i)
    {
	result.emplace_back(i);
    }

    return result;
}

std::vector<int> choose(std::vector<int> from, int many)
{
    std::vector<int> result;
    int random;
	 
    for(int i = 0; i < many && from.size() > 0; ++i)
    {
	random = randomI(0, from.size()-1);
	result.emplace_back(from[random]);
	from.erase(from.begin() + random);
    }

    return result;
}

float modulo(float a, float b)
{
    while(a < 0)
    {
	a += b;
    }
    while(a >= b)
    {
	a -= b;
    }

    return a;
}

float length(sf::Vector2f distance)
{
    return sqrt(distance.x * distance.x + distance.y * distance.y);
}

float degToRad(float angle)
{
    return angle * M_PI/180;
}

float radToDeg(float angle)
{
    return angle * 180/M_PI;
}

sf::Vector2f rotate(sf::Vector2f vector, float angle)
{
    return sf::Vector2f(vector.x * cos(angle) - vector.y * sin(angle),
			vector.x * sin(angle) + vector.y * cos(angle));
}

sf::Vector2f angleToVector2f(float angle)
{
    return sf::Vector2f(cos(angle), sin(angle));
}

void printVector(sf::Vector2i a, bool enter)
{
    std::cout << "{" << a.x << ", " << a.y << "}";
    if(enter) std::cout << std::endl;
}

void printVector(sf::Vector2f a, bool enter)
{
    std::cout << "{" << a.x << ", " << a.y << "}";
    if(enter) std::cout << std::endl;
}

bool linesCollision(sf::Vector2f c1, sf::Vector2f c2, sf::Vector2f d1, sf::Vector2f d2)
{
    //f1 = a1 * x + b1 : c1c2
    //f2 = a2 * x + b2 : d1d2
    sf::Vector2f l1;
    sf::Vector2f l2;
    sf::Vector2f r1;
    sf::Vector2f r2;

    if(c1.x <= c2.x)
    {
	l1 = c1;
	r1 = c2;
    }
    else
    {
	l1 = c2;
	r1 = c1;
    }

    if(d1.x <= d2.x)
    {
	l2 = d1;
	r2 = d2;
    }
    else
    {
	l2 = d2;
	r2 = d1;
    }


    float a1 = (r1.y - l1.y)/(r1.x - l1.x);
    float a2 = (r2.y - l2.y)/(r2.x - l2.x);
    float b1 = l1.y - a1 * l1.x;
    float b2 = l2.y - a2 * l2.x;

    if(std::max(l1.x, l2.x) > std::min(r1.x, r2.x)) return false;
    else if(l1.x == r1.x)
    {
	if(l2.x == r2.x) return (l2.x == l1.x);
	else
	{
	    return ((a2 * l1.x + b2 <= l1.y && a2 * l1.x + b2 >= r1.y) ||
		    (a2 * l1.x + b2 >= l1.y && a2 * l1.x + b2 <= r1.y));
	}
    }
    else if(l2.x == r2.x)
    {
	return ((a1 * l2.x + b1 <= l2.y && a1 * l2.x + b1 >= r2.y) ||
		(a1 * l2.x + b1 >= l2.y && a1 * l2.x + b1 <= r2.y));
    }
    else
    {
	return ((a1 * std::max(l1.x, l2.x) + b1 >= a2 * std::max(l1.x, l2.x) + b2 &&
		 a1 * std::min(r1.x, r2.x) + b1 <= a2 * std::min(r1.x, r2.x) + b2) ||
		(a1 * std::max(l1.x, l2.x) + b1 <= a2 * std::max(l1.x, l2.x) + b2 &&
		 a1 * std::min(r1.x, r2.x) + b1 >= a2 * std::min(r1.x, r2.x) + b2));
    }
}

sf::Color colorStep(sf::Color current, sf::Color end, int step)
{
    int r = current.r + ((int)std::round(((float)end.r - current.r) / step));
    int g = current.g + ((int)std::round(((float)end.g - current.g) / step));
    int b = current.b + ((int)std::round(((float)end.b - current.b) / step));

    return sf::Color(r, g, b);
}
