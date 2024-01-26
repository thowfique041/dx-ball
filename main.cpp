#include <iostream>
#include <cstdlib>
#include <raylib.h>

using namespace std;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, RED);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        // Check and reverse direction if ball hits the left and right borders
        if (x + radius >= GetScreenWidth() || x - radius <= 0) {
            speed_x *= -1;
        }

        // Reverse direction if ball hits the top border
        if (y - radius <= 0) {
            speed_y *= -1;
        }
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, BLUE);
    }

    void Update() {
        // Move paddle left if left arrow key is pressed and within screen limits
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            x -= (speed + 5);
        }

        // Move paddle right if right arrow key is pressed and within screen limits
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            x += speed + 5;
        }

        // Ensure paddle stays within screen limits
        if (x < 0) {
            x = 0;
        }

        if (x + width > GetScreenWidth()) {
            x = GetScreenWidth() - width;
        }
    }
};

class Box {
public:
    float x, y;
    float width, height;
    bool active;
    Color color;

    void Draw() {
        if (active) DrawRectangle(x, y, width, height, color);
    }
};

Ball ball;
Paddle pd;
Box box[10][34];
// Specify the number of rows for boxes
int numRows = 10;
// Specify the number of columns for boxes
int numCols = 34;
int score = 0;
bool isPaused = true;

int main() {
    const int screenWidth = 1900;
    const int screenHeight = 900;
    const int groundThreshold = screenHeight + 50;

    cout << "Which level do you want to play?" << endl;
    cout << "Press 1 for the easy level." << endl;
    cout << "Press 2 for the normal level." << endl;
    cout << "Press 3 for the hard level." << endl;

    int l, sp;
    cin >> l;

    while (l <= 0 || l >= 4) {
        cout << "Your level choice is invalid. Please enter a valid level (1 to 3):" << endl;
        cin >> l;
    }

    cout << "You chose level " << l << ". Let's play!" << endl;
    sp = 7 + (l - 1) * 3;

    InitWindow(screenWidth, screenHeight, "GROUP MEMBER: Thowfiq & Fardin. TEAM NAME: Project Dx. CSE-21 SUST");
    SetTargetFPS(60);

    // Load background image
    Texture2D background = LoadTexture("dx.png");

    // Initialize ball properties
    ball.x = screenWidth / 2;
    ball.y = 850;
    ball.radius = 15;
    ball.speed_x = sp;
    ball.speed_y = sp;

    // Initialize paddle properties
    pd.x = screenWidth / 2 - 100;
    pd.y = 850 + 15;
    pd.width = 200;
    pd.height = 20;
    pd.speed = sp;

    // Initialize box properties
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) {
            box[j][i].width = 50;
            box[j][i].height = 20;
            box[j][i].y = 100 + j * 25;
            box[j][i].x = 10 + i * (box[j][i].width + 5);
            box[j][i].active = true;
            box[j][i].color = { static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), 255 };
        }
    }

    // Game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw background image
        DrawTexture(background, 0, 0, WHITE);

        // Update and draw ball only if the game is not paused
        if (isPaused) {
            ball.Update();
            pd.Update();
        }

        // Check collision with paddle and reverse direction if collided
        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { pd.x, pd.y, pd.width, pd.height })) {
            ball.speed_y *= -1;
        }

        // Check collision with boxes, reverse direction, and deactivate box if collided
        for (int j = 0; j < numRows; j++) {
            for (int i = 0; i < numCols; i++) {
                if (box[j][i].active && CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { box[j][i].x, box[j][i].y, box[j][i].width, box[j][i].height })) {
                    ball.speed_y *= -1;
                    box[j][i].active = false;
                    score += 10;
                }
            }
        }

        // Draw elements on the screen
        ball.Draw();
        pd.Draw();

        for (int j = 0; j < numRows; j++) {
            for (int i = 0; i < numCols; i++) {
                box[j][i].Draw();
            }
        }

        // Display game-related information
        DrawText("Enjoy Dx ball game", 800, 20, 40, RED);
        DrawText(("Score: " + to_string(score)).c_str(), 10, 10, 30, GREEN);

        // Display team information
        DrawText("Idea developed by", 1250, 700, 20, WHITE);
        DrawText("Team name: Project-Dx", 1250, 700 + 30, 20, WHITE);
        DrawText("Team members:", 1250, 700 + 60, 20, WHITE);
        DrawText("Thowfiqur Bari Chowdhury & Fardin Hasan Samy", 1250, 700 + 90, 20, WHITE);
        DrawText("CSE, SUST", 1250, 700 + 120, 20, WHITE);

        // Display game over and restart option
        if (ball.y > groundThreshold) {
            DrawText("Game Over! ", 550, screenHeight / 2 - 25, 50, RED);
            DrawText(("Final Score: " + to_string(score)).c_str(), 550, screenHeight / 2 + 50, 50, GREEN);
            DrawText("Press SPACE to play again. ", 550, screenHeight / 2 + 50 + 75, 50, RED);
            DrawText("Exit Game: Press 'Esc' key.", 550, screenHeight / 2 + 50 + 75 + 75, 50, RED);

            // Restart the game if the space key is pressed
            if (IsKeyPressed(KEY_SPACE)) {
                // Reset ball position and speed
                ball.x = screenWidth / 2;
                ball.y = 850;
                ball.speed_x = sp;
                ball.speed_y = sp;

                // Reset paddle position
                pd.x = screenWidth / 2 - 100;

                // Reset box activation and color
                for (int j = 0; j < numRows; j++) {
                    for (int i = 0; i < numCols; i++) {
                        box[j][i].active = true;
                        box[j][i].color = { static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), 255 };
                    }
                }

                // Reset the score
                score = 0;

                // Unpause the game
                isPaused = true;
            }
        }

        // Pause Menu
        if (IsKeyPressed(KEY_P)) {
            isPaused = !isPaused;
        }

        EndDrawing();
    }

    // Unload background image texture
    UnloadTexture(background);

    // Close the window
    CloseWindow();
    return 0;
}
