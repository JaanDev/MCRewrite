#include "includes.hpp"
#include "Level.hpp"

int main() {
    const int winW = 1024;
    const int winH = 768;
    const float fogColor[4] = {14.f / 255.f, 11.f / 255.f, 10.f / 255.f, 1.f};

    // SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(winW, winH, "Minecraft rd-132211");
    // SetTargetFPS(60);

    rlFPCamera cam;
    cam.Setup(70, {0, 0, 0});
    cam.MoveSpeed.z = 10;
    cam.MoveSpeed.x = 10;
    cam.MoveSpeed.y = 10;
    cam.FarPlane = 1000;

    auto lvl = std::make_shared<Level>();

    while (!WindowShouldClose()) {
        cam.Update();
        BeginDrawing();

        glShadeModel(GL_SMOOTH);
        ClearBackground({127, 204, 255});

        cam.BeginMode3D();

        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, 2048);
        glFogf(GL_FOG_DENSITY, .2f);
        glFogfv(GL_FOG_COLOR, fogColor);

        lvl->render();

        glDisable(GL_FOG);

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