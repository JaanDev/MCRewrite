#include "includes.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Timer.hpp"
// #include "rcamera.h"

struct HitResult {
    RayCollision coll;
    Faces face;
    BlockPos blockPos;
};

int main() {
    SetRandomSeed(time(0));

    const int winW = 1024;
    const int winH = 768;
    const float fogColor[4] = {14.f / 255.f, 11.f / 255.f, 10.f / 255.f, 1.f};
    const float fov = 45.f;

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
    cam.Setup(fov, {0, 0, 0});
    cam.NearPlane = 0.005;
    cam.FarPlane = 1000.0;

    auto lvl = std::make_shared<Level>();
    auto player = std::make_shared<Player>(lvl);
    auto timer = std::make_shared<Timer>(60.f);

    // RayCollision collision;
    HitResult coll;

    auto plane = GenMeshPlane(1.f, 1.f, 1, 1);
    UploadMesh(&plane, false);
    auto planeModel = LoadModelFromMesh(plane);

    while (!WindowShouldClose()) {
        timer->advanceTime();

        auto a = timer->getA();

        for (uint32_t i = 0; i < timer->getTicks(); i++) {
            player->tick();
        }

        player->turn(GetMouseDelta());

        auto playerPos = player->getPos();

        cam.SetCameraPosition(player->getPos());
        auto angle = player->getRot();
        cam.setAngle(angle);
        Vector3 target = Vector3Transform(Vector3 {0, 0, 1}, MatrixRotateZYX(Vector3 {angle.y, -angle.x, 0}));
        cam.setTarget(cam.GetCamera().position + target);

        // block selection

        auto chunk = lvl->getChunk({(int)playerPos.x, (int)playerPos.y, (int)playerPos.z});
        if (chunk) {
            auto model = chunk->getModel();
            auto ray = cam.GetViewRay();
            coll.coll = GetRayCollisionMesh(ray, chunk->getModel()->meshes[0],
                                            MatrixTranslate(chunk->getPos().x * chunkSize, 0.f, chunk->getPos().z * chunkSize));
            if (coll.coll.hit) {
                auto normal = coll.coll.normal;
                if (normal == Vector3 {0, 1, 0})
                    coll.face = Faces::Up;
                else if (normal == Vector3 {0, -1, 0})
                    coll.face = Faces::Down;
                else if (normal == Vector3 {1, 0, 0})
                    coll.face = Faces::Left;
                else if (normal == Vector3 {-1, 0, 0})
                    coll.face = Faces::Right;
                else if (normal == Vector3 {0, 0, 1})
                    coll.face = Faces::Back;
                else if (normal == Vector3 {0, 0, -1})
                    coll.face = Faces::Front;

                auto pos = coll.coll.point;
                coll.blockPos = {static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z)};

                if (coll.face == Faces::Up)
                    coll.blockPos.y--;
                if (coll.face == Faces::Back)
                    coll.blockPos.z--;
                if (coll.face == Faces::Left)
                    coll.blockPos.x--;
            }
        } else {
            coll.coll.hit = false;
        }

        if (coll.coll.hit) {
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                lvl->setTile(coll.blockPos, BlockTypes::Air);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                lvl->setTile(coll.blockPos + BlockPos {(int)coll.coll.normal.x, (int)coll.coll.normal.y, (int)coll.coll.normal.z},
                             coll.blockPos.y == surfaceLevel - 1 ? BlockTypes::Grass : BlockTypes::Rock);
            }
        }

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

        if (coll.coll.hit) {
            // DrawSphere(coll.coll.point, .2f, RED);
            auto millis =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto col = Color {255, 255, 255, static_cast<uint8_t>((sin(millis / 100.0) * .2f + .4f) * 255.f)};
            // DrawPlane(coll.blockPos + Vector3 {0.5f, 1.01f, 0.5f}, {1.f, 1.f},
            //           {255, 255, 255, static_cast<uint8_t>((sin(millis / 100.0) * .2f + .4f) * 255.f)});
            drawFace(coll.blockPos, coll.face, col);
            // Vector3 pos, axis;
            // switch(coll.face) {
            //     case Faces::Up:
            //         pos = coll.blockPos + Vector3 {0, 1, 0};
            //         axis = {0, 0, 0};
            //         break;
            //     case Faces::Down:
            //         pos = coll.blockPos + Vector3 {0, 0, 0};
            //         axis = {0, 0, 0};
            //         break;
            //     case Faces::Left:
            //         pos = coll.blockPos + Vector3 {1, 0, 0};
            //         axis = {1, 0, 0};
            //         break;
            //     case Faces::Right:
            //         pos = coll.blockPos + Vector3 {0, 0, 0};
            //         axis = {-1, 0, 0};
            //         break;
            //     case Faces::Front:
            //         pos = coll.blockPos + Vector3 {0.5, 0.5, 0};
            //         axis = {1, 0, 0};
            //         break;
            //     case Faces::Back:
            //         pos = coll.blockPos + Vector3 {0, 0, 1};
            //         axis = {0, 0, -1};
            //         break;
            // }
            // DrawModelEx(planeModel, pos, axis, 90.f, {1.f, 1.f, 1.f}, col);
        }

        cam.EndMode3D();

        DrawFPS(0, 0);
        EndDrawing();
    }

    UnloadModel(planeModel);
    CloseWindow();
    return 0;
}