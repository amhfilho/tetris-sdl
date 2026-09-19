//
// Created by Filho, Antonio on 16/09/2026.
//
#include "Block.h"
#include "Colors.h"

Block::Block() {
    rotationState = 0;
    cellSize = 30;
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw(SDL_Renderer *renderer, int offsetX, int offsetY) {
    std::vector<Position> positions = cells[rotationState];
    Color color = GetCellColors()[id];
    for (const Position& pos : positions) {
        SDL_FRect block
        {
            static_cast<float>(cellSize) * pos.column + offsetX + static_cast<float>(columnOffset) * cellSize,
            static_cast<float>(cellSize) * pos.row + offsetY + static_cast<float>(rowOffset) * cellSize,
            static_cast<float>(cellSize-1),
            static_cast<float>(cellSize-1)
        };
        SDL_SetRenderDrawColor(renderer,color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &block);
    }
}

void Block::Move(int rows, int columns) {
    rowOffset += rows;
    columnOffset += columns;
}

std::vector<Position> Block::GetPositions() const {
    std::vector<Position> tiles = cells.at(rotationState);
    std::vector<Position> movedCells;
    for (const Position& tile : tiles) {
        movedCells.push_back({tile.row + rowOffset, tile.column + columnOffset});
    }
    return movedCells;
}

void Block::Rotate() {
    rotationState ++;
    if (rotationState == (int)cells.size()) {
        rotationState = 0;
    }
}

void Block::RotateBack() {
    rotationState --;
    if (rotationState < 0) {
        rotationState = (int)cells.size() - 1;
    }
}
