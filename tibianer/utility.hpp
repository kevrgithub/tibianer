#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cmath>

#include <string>
#include <fstream>

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
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

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

#endif // UTILITY_HPP
