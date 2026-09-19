#include <algorithm>
//
// Created by Filho, Antonio on 16/09/2026.
//
#include <iostream>
#include <SDL3/SDL.h>
#include "Game.h"
#include "Colors.h"
#include "Blocks.cpp"
#include <vector>
#include <random>

Game::Game()
    : running(true)
    , window(nullptr)
    , renderer(nullptr)
    , grid(nullptr)
    , gameOver(false)
    , fallTimer(0.0f)
    , score(0)
    , level(1)
    , totalLinesCleared(0)
{
    mTicksCount = SDL_GetTicks();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::InitializeWindow() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error starting SDL3: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL3 Tetris", 500, 620, 0);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    grid = Grid(renderer);
    return true;
}

void Game::HandleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (gameOver) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    Reset();
                    break;
            }
        }
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_LEFT:
                        currentBlock.Move(0, -1);
                        if (IsBlockOutside() || !BlockFits()) {
                            currentBlock.Move(0, 1);
                        }
                        break;
                    case SDLK_RIGHT:
                        currentBlock.Move(0, 1);
                        if (IsBlockOutside() || !BlockFits()) {
                            currentBlock.Move(0, -1);
                        }
                        break;
                    case SDLK_DOWN:
                        downPressed = true;
                        MoveBlockDown();
                        break;
                    case SDLK_UP:
                        RotateBlock();
                        break;
                }
                break;
        }
    }
}

void Game::Update() {
    if (gameOver) {
        return;
    }
    Uint64 elapsed = SDL_GetTicks() - mTicksCount;

    if (elapsed < 16) {
        SDL_Delay(static_cast<Uint32>(16 - elapsed));
    }

    Uint64 currentTicks = SDL_GetTicks();
    float deltaTime = (currentTicks - mTicksCount) / 1000.0f;
    mTicksCount = currentTicks;
    fallTimer += deltaTime;

    float fallTime = std::pow(0.8f - ((level - 1) * 0.007f), level - 1);
    if (fallTimer >= fallTime)
    {
        fallTimer -= fallTime;
        MoveBlockDown();
    }
}

void Game::Draw() {
    SDL_SetRenderDrawColor(renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_RenderClear(renderer);
    grid.Draw();
    currentBlock.Draw(renderer, 11, 11);

    DrawScoreBoard();
    DrawNextArea();

    if (gameOver) {
        SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 170, 250, "GAME OVER");
    }

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_RenderPresent(renderer);
}


std::vector<Block> Game::GetAllBlocks() {
    return {LBlock(), JBlock(), IBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

Block Game::GetRandomBlock() {
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, blocks.size() - 1);
    int randomIndex = dis(gen);
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

void Game::RotateBlock()
{
    if (gameOver) return;
    currentBlock.Rotate();
    if (IsBlockOutside() || !BlockFits())
    {
        currentBlock.RotateBack();
    }
}

void Game::MoveBlockDown() {
    currentBlock.Move(1, 0);
    if (downPressed) {
        score++;
    }
    if (IsBlockOutside() || !BlockFits())
    {
        currentBlock.Move(-1, 0);
        if (downPressed) {
            score--;
        }
        LockBlock();
    }
    downPressed = false;
}

bool Game::IsBlockOutside()
{
    std::vector<Position> cells = currentBlock.GetPositions();
    for (const auto& cell : cells)
    {
        int row = cell.row;
        int col = cell.column;
        if (grid.isCellOutside(row, col))
        {
            return true;
        }
    }
    return false;
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetPositions();
    for (const auto& tile : tiles)
    {
        grid.grid[tile.row][tile.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (!BlockFits())
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int linesCleared = grid.ClearFullRows();
    CalculateScore(linesCleared);
}

bool Game::BlockFits() {
    std::vector<Position> tiles = currentBlock.GetPositions();
    for (const auto& tile : tiles)
    {
        int row = tile.row;
        int col = tile.column;
        if (!grid.isCellEmpty(row, col))
        {
            return false;
        }
    }
    return true;
}

void Game::CalculateScore(int linesCleared) {
    if (linesCleared == 1) {
        score += 40 * level;
    } else if (linesCleared == 2) {
        score += 100 * level;
    } else if (linesCleared == 3) {
        score += 300 * level;
    } else if (linesCleared >= 4) {
        score += 1200 * level;
    }
    totalLinesCleared += linesCleared;
    level = totalLinesCleared / 10 + 1;
}

void Game::DrawScoreBoard() {
    DrawRoundedRect(320, 50, 170, 240);

    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_RenderDebugText(renderer, 170, 40, "SCORE");
    SDL_RenderDebugText(renderer, 170, 50, (std::to_string(score)).c_str());

    SDL_RenderDebugText(renderer, 170, 80, "LEVEL");
    SDL_RenderDebugText(renderer, 170, 90, (std::to_string(level)).c_str());

    SDL_RenderDebugText(renderer, 170, 120, "LINES");
    SDL_RenderDebugText(renderer, 170, 130, (std::to_string(totalLinesCleared)).c_str());

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void Game::DrawNextArea() {
    const float rectangleX = 320;
    const float rectangleY = 300;
    const float rectangleWidth = 170;
    const float rectangleHeight = 170;

    DrawRoundedRect(rectangleX, rectangleY, rectangleWidth, rectangleHeight);

    // Block::Draw places a cell at cellSize * (row/column incl. block offset) + offset,
    // so measure the piece's bounding box from its current positions.
    const int cellSize = 30; // same as Block::cellSize
    const std::vector<Position> positions = nextBlock.GetPositions();
    int minRow = positions[0].row, maxRow = positions[0].row;
    int minCol = positions[0].column, maxCol = positions[0].column;
    for (const Position& pos : positions) {
        minRow = std::min(minRow, pos.row);
        maxRow = std::max(maxRow, pos.row);
        minCol = std::min(minCol, pos.column);
        maxCol = std::max(maxCol, pos.column);
    }

    const float pieceWidth = static_cast<float>((maxCol - minCol + 1) * cellSize - 1);
    const float pieceHeight = static_cast<float>((maxRow - minRow + 1) * cellSize - 1);

    const float offsetX = rectangleX + (rectangleWidth - pieceWidth) / 2.0f - static_cast<float>(minCol * cellSize);
    const float offsetY = rectangleY + (rectangleHeight - pieceHeight) / 2.0f - static_cast<float>(minRow * cellSize);

    nextBlock.Draw(renderer, static_cast<int>(offsetX), static_cast<int>(offsetY));
}

void Game::DrawRoundedRect(float x, float y, float w, float h) {
    SDL_SetRenderDrawColor(renderer, darkGrey.r, darkGrey.g, darkGrey.b, 255);
    SDL_FRect rect = { x, y, w, h };
    DrawFilledRoundedRect(rect, 40.0f);
}

void Game::DrawFilledRoundedRect(SDL_FRect rect, float radius) {
    if (radius < 0) radius = 0;
    if (radius > rect.w / 2) radius = rect.w / 2;
    if (radius > rect.h / 2) radius = rect.h / 2;

    // Center rectangle
    SDL_FRect center = { rect.x + radius, rect.y, rect.w - 2 * radius, rect.h };
    SDL_RenderFillRect(renderer, &center);

    // Left rectangle
    SDL_FRect left = { rect.x, rect.y + radius, radius, rect.h - 2 * radius };
    SDL_RenderFillRect(renderer, &left);

    // Right rectangle
    SDL_FRect right = { rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius };
    SDL_RenderFillRect(renderer, &right);

    // Four rounded corners
    DrawFilledCircle((int)(rect.x + radius), (int)(rect.y + radius), (int)radius); // Top-left
    DrawFilledCircle((int)(rect.x + rect.w - radius), (int)(rect.y + radius), (int)radius); // Top-right
    DrawFilledCircle((int)(rect.x + radius), (int)(rect.y + rect.h - radius), (int)radius); // Bottom-left
    DrawFilledCircle((int)(rect.x + rect.w - radius), (int)(rect.y + rect.h - radius), (int)radius); // Bottom-right
}

void Game::DrawFilledCircle(int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
}

void Game::Reset() {
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    mTicksCount = SDL_GetTicks();
    totalLinesCleared = 0;
    score = 0;
}
