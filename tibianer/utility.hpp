#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cmath>

#include <string>
#include <fstream>
#include <random>

namespace utility
{

std::random_device randomDevice;
std::default_random_engine defaultRandomEngine(randomDevice());

void toggleBool(bool& b)
{
    b = !b;
}

bool fileExists(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

float calculateDistance(float x1, float y1, float x2, float y2)
{
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

int getRandomNumber(int min, int max)
{
    std::uniform_int_distribution<int> uniformDistribution(min, max);

    return uniformDistribution(defaultRandomEngine);
}

float getRandomNumberFloat(float min, float max)
{
    std::uniform_real_distribution<float> uniformDistribution(min, max);

    return uniformDistribution(defaultRandomEngine);
}

/*
int getRandomNumber(int low, int high)
{
    return std::rand() % ((high - low) + 1) + low;
}

float getRandomNumberFloat(float low, float high)
{
    //float random = ((float)std::rand()) / (float)RAND_MAX;
    //float difference = high - low;
    //float r = random * difference;
    //return low + r;

    return low + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (high - low)));
}
*/

void eraseNullTerminator(std::string& text)
{
    text.erase(std::remove(text.begin(), text.end(), '\0'), text.end());
}

} // namespace utility

#endif // UTILITY_HPP
