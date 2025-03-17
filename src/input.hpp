#ifndef INPUT_HPP
#define INPUT_HPP

#include <bitset>
#include <raylib.h>

typedef struct {
	Vector2 px;
	std::bitset<8> btns;
} Mouse;

typedef struct {
	Mouse mouse;
} Input;

void processInput(Input& input);

#endif // !INPUT_HPP
