#include "input.hpp"
#include <cstdio>
#include <raylib.h>
#include <sstream>

#define MAX_SCREEN_WIDTH 1024
#define MAX_SCREEN_HEIGHT 768

#define PARTICLE_SIZE 3.0f
#define GRAVITY 0.01f

const int NUM_PARTICLES_IN_ROW = MAX_SCREEN_WIDTH / PARTICLE_SIZE;
const int NUM_PARTICLES_IN_COL = MAX_SCREEN_HEIGHT / PARTICLE_SIZE;
const int TOTAL_NUM_PARTICLES = NUM_PARTICLES_IN_ROW * NUM_PARTICLES_IN_COL;

typedef enum {
    NONE,
    SAND,
    WATER,
} ParticleType;

typedef struct {
    Vector2 px;
    Vector2 size;
    ParticleType type;
} Particle;

typedef struct {
    Input input;
} GameState;

Particle grid[(int)TOTAL_NUM_PARTICLES];
GameState game;

void setup();
void update();
void moveSand(const int i);
void moveWater(const int i, const ParticleType type);
void render();
void drawDebugUI();

int main()
{
    constexpr int screenWidth = NUM_PARTICLES_IN_ROW * PARTICLE_SIZE;
    constexpr int screenHeight = NUM_PARTICLES_IN_COL * PARTICLE_SIZE;

    InitWindow(screenWidth, screenHeight, "Sand Simulation");
    SetTargetFPS(1000);
    SetExitKey(KEY_ESCAPE);

    setup();

    while (!WindowShouldClose()) {
        processInput(game.input);
        update();
        render();
    }

    CloseWindow();
}

void setup()
{
    for (int i = 0; i < TOTAL_NUM_PARTICLES; i++) {
        int x{i % static_cast<int>(NUM_PARTICLES_IN_ROW)};
        int y{i / static_cast<int>(NUM_PARTICLES_IN_ROW)};

        grid[i].px = {x * PARTICLE_SIZE, y * PARTICLE_SIZE};
        grid[i].size = {PARTICLE_SIZE, PARTICLE_SIZE};
    }
}

void update()
{
    if (game.input.mouse.btns.test(MOUSE_BUTTON_LEFT)) {
        int gridX{static_cast<int>(game.input.mouse.px.x / PARTICLE_SIZE)};
        int gridY{static_cast<int>(game.input.mouse.px.y / PARTICLE_SIZE)};
        int idx{static_cast<int>(gridY * NUM_PARTICLES_IN_ROW + gridX)};

        grid[idx].type = ParticleType::SAND;
    }

    if (game.input.mouse.btns.test(MOUSE_BUTTON_RIGHT)) {
        int gridX{static_cast<int>(game.input.mouse.px.x / PARTICLE_SIZE)};
        int gridY{static_cast<int>(game.input.mouse.px.y / PARTICLE_SIZE)};
        int idx{static_cast<int>(gridY * NUM_PARTICLES_IN_ROW + gridX)};

        grid[idx].type = ParticleType::WATER;
    }

    float dt = GetFrameTime();
    static float accumulator;
    accumulator += dt;

    while (accumulator >= GRAVITY) {
        accumulator -= GRAVITY;

        for (int i = TOTAL_NUM_PARTICLES - 1; i >= 0; i--) {
            switch (grid[i].type) {
            case ParticleType::SAND: {
                moveSand(i);
            } break;

            case ParticleType::WATER: {
                moveWater(i, ParticleType::NONE);
            } break;

            default: {
            } break;
            }
        }
    }
}

void moveWater(const int i, const ParticleType type)
{
    int iBelow{i + NUM_PARTICLES_IN_ROW};
    int iLeft{i - 1};
    int iRight{i + 1};
    int idx{iBelow};

    if (iBelow >= TOTAL_NUM_PARTICLES || iRight >= TOTAL_NUM_PARTICLES || iLeft < 0) return;

    if (grid[idx].type != ParticleType::NONE) idx = iLeft;
    if (grid[idx].type != ParticleType::NONE) idx = iRight;
    if (grid[idx].type != ParticleType::NONE) return;

    grid[idx].type = ParticleType::WATER;
    grid[i].type = type;
}

void moveSand(const int i)
{
    int iBelow = i + NUM_PARTICLES_IN_ROW;

    if (iBelow >= TOTAL_NUM_PARTICLES) return;

    if (grid[iBelow].type == ParticleType::WATER) {
        moveWater(iBelow, ParticleType::SAND);
        // continue;
    }

    if (grid[iBelow].type != ParticleType::SAND) {
        grid[iBelow].type = ParticleType::SAND;
        grid[i].type = ParticleType::NONE;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    int waters{};
    int sands{};

    for (Particle p : grid) {
        if (p.type != NONE) {
            Color colour;
            switch (p.type) {
            case ParticleType::SAND: {
                colour = BEIGE;
                sands++;
            } break;

            case ParticleType::WATER: {
                colour = BLUE;
                waters++;
            } break;

            default: {
            } break;
            }

            DrawRectangleV(p.px, p.size, colour);
        }
    }

    // Debug
    {
        std::ostringstream txtss;
        txtss << "waters:" << waters << " sands:" << sands;
        DrawText(txtss.str().c_str(), 10, 10, 20, BLACK);
    }

    // drawDebugUI();

    EndDrawing();
}

void drawDebugUI()
{
    // Debug
    int gridX{static_cast<int>(game.input.mouse.px.x / PARTICLE_SIZE)};
    int gridY{static_cast<int>(game.input.mouse.px.y / PARTICLE_SIZE)};
    int idx{static_cast<int>(gridY * NUM_PARTICLES_IN_ROW + gridX)};
    std::ostringstream txtss;
    txtss << "mx:" << gridX << " my:" << gridY << " idx:" << idx;

    DrawText(txtss.str().c_str(), 10, 10, 20, BLACK);
}
