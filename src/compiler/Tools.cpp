#include "Tools.hpp"
#include <algorithm>
#include <cstring>
#include <random>

const char* words = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char* number_words = "0123456789";

int generateRandomNumber(int min, int max) {
    static std::random_device devices;
    static std::mt19937 rand(devices());
    uniform_int_distribution dist(min, max);
    return dist(devices);
}

string GenerateRandomName() {
    int size = 12;
    string name = "";
    const char* format = "xxxxyyyyxyxy";
    for(int i = 0; i < strlen(format); i++)
    {
        char __c = format[i];
        if( __c == 'x' )
            name += words[ generateRandomNumber(0, strlen(words) - 1) ];
        else if ( __c == 'y' )
            name += number_words[ generateRandomNumber(0, strlen( number_words ) - 1)];
    }

    return name;
}
