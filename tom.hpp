#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


constexpr std::array<char, 6> Samo = {'a', 'e', 'i', 'o', 'u', 'y'};
constexpr std::array<char, 20> Spol = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'z'};

template <typename T> int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

int boolToSign(bool input);
std::string strPrecRound(double value, short precision);
int randomI(int min, int max);
float randomF(float min, float max, float step = 0.01f);
bool randomB();
char randomSa();
char randomSp();
std::vector<int> range(int min, int max);
std::vector<int> choose(std::vector<int> from, int many);
float modulo(float a, float b);
float length(sf::Vector2f distance);
float degToRad(float angle);
float radToDeg(float angle);
sf::Vector2f rotate(sf::Vector2f vector, float angle);
sf::Vector2f angleToVector2f(float angle);
void printVector(sf::Vector2i a, bool enter = false);
void printVector(sf::Vector2f a, bool enter = false);
bool linesCollision(sf::Vector2f c1, sf::Vector2f c2, sf::Vector2f d1, sf::Vector2f d2);
sf::Color colorStep(sf::Color current, sf::Color end, int step);

