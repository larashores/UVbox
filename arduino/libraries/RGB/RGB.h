#ifndef RGB_h
#define RGB_h

struct Color
{
    constexpr Color(int red, int green, int blue) : red{red}, green{green}, blue{blue} {}

    int red;
    int green;
    int blue;
};

#endif