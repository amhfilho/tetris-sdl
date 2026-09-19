//
// Created by Filho, Antonio on 16/09/2026.
//
#pragma once
#include <vector>
#include <map>
#include <SDL3/SDL.h>

struct Position {
    int row;
    int column;
};
class Block {
public:
    Block();
    int id;
    std::map<int, std::vector<Position>> cells;
    std::vector<Position> GetPositions() const;
    void Draw(SDL_Renderer* renderer, int offsetX, int offsetY);
    void Move(int rows, int columns);
    void Rotate();
    void RotateBack();
private:
    int rotationState;
    int cellSize;
    int rowOffset;
    int columnOffset;
};
