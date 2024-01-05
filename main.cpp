#include <iostream>
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

        if (x + radius >= GetScreenWidth() || x - radius <= 0) {
            speed_x *= -1;
        }

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
        DrawRectangle(x, y, width, height, YELLOW);
    }

    void Update() {
        if (IsKeyDown(KEY_LEFT) && x > 0) {
            x -= (speed + 5);
        }

        if (IsKeyDown(KEY_RIGHT) && x + width < GetScreenWidth()) {
            x += speed + 5;
        }
    }
};

class Box {
public:
    float x, y;
    float width, height;
    bool active;

    void Draw() {
        if (active) DrawRectangle(x, y, width, height, GREEN);
    }
};

Ball ball;
Paddle player;
Box box[10][28];
int score = 0;

int main() {
    const int screenWidth = 1900;
    const int screenHeight = 900;
    const int groundThreshold = screenHeight + 50;

    InitWindow(screenWidth, screenHeight, "GROUP MEMBER: Thowfiq & Fardin. TEAM NAME: Project Dx. CSE-21 SUST");
    SetTargetFPS(60);

    ball.x = screenWidth / 2;
    ball.y = 850;
    ball.radius = 15;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.x = screenWidth / 2 - 100;
    player.y = 850 + 15;
    player.width = 200;
    player.height = 20;
    player.speed = 8;

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 28; i++) {
            box[j][i].width = 100;
            box[j][i].height = 20;
            box[j][i].y = 100 + j * 25;
            box[j][i].x = 100 + i * 55;
            box[j][i].active = true;
        }
    }

    while (WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(BLACK);

        ball.Update();
        player.Update();

        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height })) {
            ball.speed_y *= -1;
        }

        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 28; i++) {
                if (box[j][i].active && CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { box[j][i].x, box[j][i].y, box[j][i].width, box[j][i].height })) {
                    ball.speed_y *= -1;
                    box[j][i].active = false;
                    score += 10;
                }
            }
        }

        ball.Draw();
        player.Draw();

        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 28; i++) {
                box[j][i].Draw();
            }
        }

        DrawText("Enjoy Dx ball game", 800, 20, 40, RED);
        DrawText(("Score: " + to_string(score)).c_str(), 10, 10, 30, GREEN);

        DrawText("Idea developed by", 1250, 700, 20, WHITE);
        DrawText("Team name : Project-Dx", 1250, 700 + 30, 20, WHITE);
        DrawText("Team members:", 1250, 700 + 60, 20, WHITE);
        DrawText("Thowfiqur Bari Chowdhury & Fardin Hasan Samy", 1250, 700 + 90, 20, WHITE);
        DrawText("CSE , SUST", 1250, 700 + 120, 20, WHITE);

        if (ball.y > groundThreshold) {
            DrawText("Game Over! ", 550, screenHeight / 2 - 25, 50, RED);
            DrawText(("Final Score: " + to_string(score)).c_str(), 550, screenHeight / 2 + 50, 50, GREEN);
            DrawText("Press SPACE to restart. ", 550, screenHeight / 2 +50+75, 50, RED);
            if (IsKeyPressed(KEY_SPACE)) {
                ball.x = screenWidth / 2;
                ball.y = 850;
                ball.speed_x = 7;
                ball.speed_y = 7;

                player.x = screenWidth / 2 - 100;

                for (int j = 0; j < 10; j++) {
                    for (int i = 0; i < 28; i++) {
                        box[j][i].active = true;
                    }
                }

                score = 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
