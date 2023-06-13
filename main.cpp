#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Match3 : public olc::PixelGameEngine {
public:
    Match3() {
        sAppName = "Match 3";
    }

private:
    const static int nGridWidth = 10;
    const static int nGridHeight = 10;
    int nCellSize = 16;

    int nCursorX = 0;
    int nCursorY = 0;

    const static int NUM_COLORS = 6;
    olc::Pixel dark_colors[NUM_COLORS] = { olc::DARK_BLUE, olc::DARK_CYAN, olc::DARK_GREEN,
        olc::DARK_MAGENTA, olc::DARK_RED, olc::DARK_YELLOW };

    olc::Pixel grid[nGridHeight][nGridWidth] = { 0 };

    void ReadInput() {
        if (GetKey(olc::Key::A).bPressed && nCursorX > 0) {
            nCursorX--;
        }
        if (GetKey(olc::Key::D).bPressed && nCursorX < nGridWidth - 1) {
            nCursorX++;
        }
        if (GetKey(olc::Key::W).bPressed && nCursorY > 0) {
            nCursorY--;
        }
        if (GetKey(olc::Key::S).bPressed && nCursorY < nGridHeight - 1) {
            nCursorY++;
        }

        if (GetKey(olc::Key::J).bPressed && nCursorX > 1) {
            olc::Pixel temp = grid[nCursorY][nCursorX];
            grid[nCursorY][nCursorX] = grid[nCursorY][nCursorX - 1];
            grid[nCursorY][nCursorX - 1] = temp;
        }
        if (GetKey(olc::Key::L).bPressed && nCursorX < nGridWidth - 1) {
            olc::Pixel temp = grid[nCursorY][nCursorX];
            grid[nCursorY][nCursorX] = grid[nCursorY][nCursorX + 1];
            grid[nCursorY][nCursorX + 1] = temp;
        }
        if (GetKey(olc::Key::I).bPressed && nCursorY > 1) {
            olc::Pixel temp = grid[nCursorY][nCursorX];
            grid[nCursorY][nCursorX] = grid[nCursorY - 1][nCursorX];
            grid[nCursorY - 1][nCursorX] = temp;
        }
        if (GetKey(olc::Key::K).bPressed && nCursorY < nGridHeight - 1) {
            olc::Pixel temp = grid[nCursorY][nCursorX];
            grid[nCursorY][nCursorX] = grid[nCursorY + 1][nCursorX];
            grid[nCursorY + 1][nCursorX] = temp;
        }
    }

    void ApplyGravity() {
        // Apply gravity
        for (int x = 0; x < nGridWidth; x++) {
            int nOffset = 0;
            for (int y = nGridHeight - 1; y >= 0; y--) {
                if (grid[y][x] != olc::BLANK) {
                    grid[y + nOffset][x] = grid[y][x];
                } else {
                    nOffset++;
                }
            }
            for (int y = 0; y < nOffset; y++) {
                grid[y][x] = dark_colors[rand() % NUM_COLORS];
            }
        }
    }

    void CheckMatches() {
        // Check for horizontal matches
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth - 2; x++) {
                if (grid[y][x] == grid[y][x + 1] && grid[y][x] == grid[y][x + 2]) {
                    grid[y][x] = olc::BLANK;
                    grid[y][x + 1] = olc::BLANK;
                    grid[y][x + 2] = olc::BLANK;
                }
            }
        }
        // Check for vertical matches
        for (int y = 0; y < nGridHeight - 2; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                if (grid[y][x] == grid[y + 1][x] && grid[y][x] == grid[y + 2][x]) {
                    grid[y][x] = olc::BLANK;
                    grid[y + 1][x] = olc::BLANK;
                    grid[y + 2][x] = olc::BLANK;
                }
            }
        }
    }

    void DrawGrid() {
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                olc::Pixel color = grid[y][x];
                FillRect(x * nCellSize + 1, y * nCellSize + 1, nCellSize - 1, nCellSize - 1, color);
            }
        }
    }

    void DrawCursor() {
        DrawRect((nCursorX + 1) * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::RED);
        DrawRect(nCursorX * nCellSize, (nCursorY + 1) * nCellSize, nCellSize, nCellSize, olc::RED);
        DrawRect((nCursorX - 1) * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::RED);
        DrawRect(nCursorX * nCellSize, (nCursorY - 1) * nCellSize, nCellSize, nCellSize, olc::RED);
        DrawRect(nCursorX * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::YELLOW);
    }

public:
    bool OnUserCreate() override {
        srand(time(NULL));

        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                grid[y][x] = dark_colors[rand() % NUM_COLORS];
            }
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);

        ReadInput();
        ApplyGravity();
        CheckMatches();
        DrawGrid();
        DrawCursor();

        return true;
    }
};

int main() {
    Match3 game;
    if (game.Construct(160, 160, 4, 4, false, true)) {
        game.Start();
    }
    return 0;
}