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
    int nCellSize = 160;

    int nCursorX = 0;
    int nCursorY = 0;

    olc::Sprite *grid[nGridHeight][nGridWidth] = { 0 };

    const static int NUM_FRUITS = 18;
    olc::Sprite **fruits = nullptr;

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
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY][nCursorX - 1]);
        }
        if (GetKey(olc::Key::L).bPressed && nCursorX < nGridWidth - 1) {
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY][nCursorX + 1]);
        }
        if (GetKey(olc::Key::I).bPressed && nCursorY > 1) {
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY - 1][nCursorX]);
        }
        if (GetKey(olc::Key::K).bPressed && nCursorY < nGridHeight - 1) {
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY + 1][nCursorX]);
        }
    }

    void ApplyGravity() {
        // Apply gravity
        for (int x = 0; x < nGridWidth; x++) {
            int nOffset = 0;
            for (int y = nGridHeight - 1; y >= 0; y--) {
                if (grid[y][x] != nullptr) {
                    grid[y + nOffset][x] = grid[y][x];
                } else {
                    nOffset++;
                }
            }
            for (int y = 0; y < nOffset; y++) {
                grid[y][x] = fruits[rand() % NUM_FRUITS];
            }
        }
    }

    void CheckMatches() {
        // Check for horizontal matches
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth - 2; x++) {
                if (grid[y][x] == grid[y][x + 1] && grid[y][x] == grid[y][x + 2]) {
                    grid[y][x] = nullptr;
                    grid[y][x + 1] = nullptr;
                    grid[y][x + 2] = nullptr;
                }
            }
        }
        // Check for vertical matches
        for (int y = 0; y < nGridHeight - 2; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                if (grid[y][x] == grid[y + 1][x] && grid[y][x] == grid[y + 2][x]) {
                    grid[y][x] = nullptr;
                    grid[y + 1][x] = nullptr;
                    grid[y + 2][x] = nullptr;
                }
            }
        }
    }

    void DrawGrid() {
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                olc::Sprite *color = grid[y][x];
                if (color != nullptr) {
                    DrawSprite(x * nCellSize, y * nCellSize, color);
                }
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

    void InitFruits() {
        fruits = new olc::Sprite *[NUM_FRUITS];
        std::string fruit_names[NUM_FRUITS] = { "Apple", "Avocado", "Banana", "Blackberry",
            "Cherry", "Coconut", "Fig", "Grapes", "Kiwi", "Lemon", "Mango", "Orange", "Peach",
            "Pear", "Pineapple", "Plum", "Strawberry", "Watermelon" };
        for (int i = 0; i < NUM_FRUITS; i++) {
            std::string path = "Assets/" + fruit_names[i] + ".png";
            fruits[i] = new olc::Sprite(path);
        }
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                grid[y][x] = fruits[rand() % NUM_FRUITS];
            }
        }
    }

public:
    bool OnUserCreate() override {
        srand(time(NULL));

        InitFruits();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);

        ReadInput();
        ApplyGravity();
        CheckMatches();
        DrawCursor();
        DrawGrid();

        return true;
    }

    bool OnUserDestroy() override {
        for (int i = 0; i < NUM_FRUITS; i++) {
            delete fruits[i];
        }
        delete[] fruits;
        return true;
    }
};

int main() {
    Match3 game;
    if (game.Construct(1600, 1600, 1, 1, false, true)) {
        game.Start();
    }
    return 0;
}