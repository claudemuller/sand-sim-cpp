#ifndef INPUT_HPP
#define INPUT_HPP

#include <bitset>
#include <raylib.h>

struct Mouse {
    Vector2 px;
    std::bitset<8> btns;
};

struct Input {
    Mouse mouse;
};

void processInput(Input &input);

#endif // !INPUT_HPP
