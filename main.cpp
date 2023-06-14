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
    int nCellSize = 80;

    int nCursorX = 0;
    int nCursorY = 0;

    olc::Sprite *grid[nGridHeight][nGridWidth] = { 0 };

    const static int NUM_FRUITS = 7;
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

        if (GetKey(olc::Key::J).bPressed && nCursorX > 0) {
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY][nCursorX - 1]);
        }
        if (GetKey(olc::Key::L).bPressed && nCursorX < nGridWidth - 1) {
            std::swap(grid[nCursorY][nCursorX], grid[nCursorY][nCursorX + 1]);
        }
        if (GetKey(olc::Key::I).bPressed && nCursorY > 0) {
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

    bool HasMatch(int x1, int y1, int x2, int y2) {
        std::swap(grid[y1][x1], grid[y2][x2]);
        bool match = CheckMatches(false);
        std::swap(grid[y1][x1], grid[y2][x2]);
        return match;
    }

    bool CheckMatches(bool clear) {
        // Check for horizontal matches
        bool match = false;
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                int nMatch = 0;
                while (x + nMatch < nGridWidth && grid[y][x + nMatch] == grid[y][x]) {
                    nMatch++;
                }
                if (nMatch >= 3) {
                    for (int i = 0; clear && i < nMatch; i++) {
                        grid[y][x + i] = nullptr;
                    }
                    x += nMatch - 1;
                    match = true;
                }
            }
        }
        // Check for vertical matches
        for (int y = 0; y < nGridHeight - 2; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                int nMatch = 0;
                while (y + nMatch < nGridHeight && grid[y + nMatch][x] == grid[y][x]) {
                    nMatch++;
                }
                if (nMatch >= 3) {
                    for (int i = 0; clear && i < nMatch; i++) {
                        grid[y + i][x] = nullptr;
                    }
                    y += nMatch - 1;
                    match = true;
                }
            }
        }
        return match;
    }

    void DrawGrid() {
        SetPixelMode(olc::Pixel::MASK);
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                olc::Sprite *color = grid[y][x];
                if (color != nullptr) {
                    DrawSprite(x * nCellSize, y * nCellSize, color);
                }
            }
        }
        SetPixelMode(olc::Pixel::NORMAL);
    }

    void DrawCursor() {
        FillRect(
            (nCursorX + 1) * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::DARK_RED);
        FillRect(
            nCursorX * nCellSize, (nCursorY + 1) * nCellSize, nCellSize, nCellSize, olc::DARK_RED);
        FillRect(
            (nCursorX - 1) * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::DARK_RED);
        FillRect(
            nCursorX * nCellSize, (nCursorY - 1) * nCellSize, nCellSize, nCellSize, olc::DARK_RED);
        FillRect(
            nCursorX * nCellSize, nCursorY * nCellSize, nCellSize, nCellSize, olc::DARK_YELLOW);
    }

    void InitFruits() {
        fruits = new olc::Sprite *[NUM_FRUITS];
        std::string fruit_names[NUM_FRUITS] = {
            "Apple",
            "Banana",
            "Blackberry",
            "Cherry",
            "Coconut",
            "Orange",
            "Pear",
        };
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

    void HighlightMatches() {
        for (int y = 0; y < nGridHeight; y++) {
            for (int x = 0; x < nGridWidth; x++) {
                if ((x > 0 && HasMatch(x, y, x - 1, y))
                    || (x < nGridWidth - 1 && HasMatch(x, y, x + 1, y))
                    || (y > 0 && HasMatch(x, y, x, y - 1))
                    || (y < nGridHeight - 1 && HasMatch(x, y, x, y + 1))) {
                    FillRect(x * nCellSize, y * nCellSize, nCellSize, nCellSize, olc::DARK_GREEN);
                }
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
        CheckMatches(true);
        DrawCursor();
        HighlightMatches();
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
    if (game.Construct(800, 800, 1, 1, false, true)) {
        game.Start();
    }
    return 0;
}