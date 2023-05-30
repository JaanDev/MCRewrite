#include "includes.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Timer.hpp"
// #include "rcamera.h"

int main() {
    SetRandomSeed(time(0));

    const int winW = 1024;
    const int winH = 768;
    const float fogColor[4] = {14.f / 255.f, 11.f / 255.f, 10.f / 255.f, 1.f};

    // SetConfigFlags(FLAG_MSAA_4X_HINT);   
    InitWindow(winW, winH, "Minecraft rd-132211");
    // SetTargetFPS(60);
    SetTargetFPS(2600); // so it isnt too much bc then my pc starts making a high frequency noise which isnt good i suppose

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(.5f, .8f, 1.f, 0.f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // glMatrixMode(5889);
    // glLoadIdentity();
    // glMatrixMode(5888);

    rlFPCamera cam;
    cam.Setup(70, {0, 0, 0});
    cam.NearPlane = 0.005;
    cam.FarPlane = 1000.0;

    auto lvl = std::make_shared<Level>();
    auto player = std::make_shared<Player>(lvl);
    auto timer = std::make_shared<Timer>(60.f);

    while (!WindowShouldClose()) {
        timer->advanceTime();

        auto a = timer->getA();

        for (uint32_t i = 0; i < timer->getTicks(); i++) {
            player->tick();
        }

        player->turn(GetMouseDelta());

        cam.SetCameraPosition(player->getPos());
        auto angle = player->getRot();
        cam.setAngle(angle);
        Vector3 target = Vector3Transform(Vector3 {0, 0, 1}, MatrixRotateZYX(Vector3 {angle.y, -angle.x, 0}));
        cam.setTarget(cam.GetCamera().position + target);
        
        BeginDrawing();

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