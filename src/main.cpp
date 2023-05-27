#include "includes.hpp"

int main() {
    logD("Test");
    logW("Test");
    logE("Test");

    const int winW = 1280;
    const int winH = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(winW, winH, "Minecraft rd-132211");
    SetTargetFPS(60);

    rlFPCamera cam;
    cam.Setup(70, {0, 0, 0});
    cam.MoveSpeed.z = 10;
    cam.MoveSpeed.x = 10;
    cam.MoveSpeed.y = 10;
    cam.FarPlane = 1000;

    while (!WindowShouldClose()) {
        cam.Update();
        BeginDrawing();

        ClearBackground(WHITE);

        cam.BeginMode3D();

        DrawGrid(10, 1);
        DrawRay({{0, 0, 0}, {1, 0, 0}}, RED);   // x
        DrawRay({{0, 0, 0}, {0, 1, 0}}, GREEN); // y
        DrawRay({{0, 0, 0}, {0, 0, 1}}, BLUE);  // z

        cam.EndMode3D();

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}