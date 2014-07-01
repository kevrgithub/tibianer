#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cmath>

#include <string>
#include <fstream>


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

#endif // UTILITY_HPP
