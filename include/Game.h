#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Block.h"
#include "Grid.h"

class Game
{
public:
    Game();
    ~Game();
    bool running;
    bool InitializeWindow();
    void HandleInput();
    void Update();
    void Draw();

private:
    int level;
    float fallTimer;
    int score;
    int totalLinesCleared;
    Uint64 mTicksCount;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    bool gameOver;
    bool downPressed;
    std::vector<Block> GetAllBlocks();
    Block GetRandomBlock();
    void RotateBlock();
    void CalculateScore(int linesCleared);
    void DrawScoreBoard();
    void MoveBlockDown();
    bool IsBlockOutside();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void DrawNextArea();
    void DrawFilledRoundedRect(SDL_FRect rect, float x);
    void DrawFilledCircle(int cx, int cy, int radius);
    void DrawRoundedRect(float x, float y, float w, float h);
};
