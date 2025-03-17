#include "input.hpp"
#include <raylib.h>

void processInput(Input &input)
{
    input.mouse.px = GetMousePosition();

    input.mouse.btns.reset();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) input.mouse.btns.set(MOUSE_BUTTON_LEFT);
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) input.mouse.btns.reset(MOUSE_BUTTON_LEFT);
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) input.mouse.btns.set(MOUSE_BUTTON_RIGHT);
    if (IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) input.mouse.btns.reset(MOUSE_BUTTON_RIGHT);
}
