#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Logger.hpp>

int main() {
    srand(time(0));

    const int width = 1024;
    const int height = 768;
    constexpr float fogColor[] = {14.f / 255.f, 11.f / 255.f, 10.f / 255.f, 1.f};

    if (!glfwInit()) {
        logE("GLFW init error!");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Game", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        logE("Error while creating window");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        logE("Error while loading GLADs");
        return -1;
    }

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.8f, 1.f, 0.f);
    glClearDepth(1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // rlFPCamera cam;
    // cam.Setup(fov, {0, 0, 0});
    // cam.NearPlane = 0.005;
    // cam.FarPlane = 1000.0;

    // auto lvl = std::make_shared<Level>();
    // auto player = std::make_shared<Player>(lvl);
    // auto timer = std::make_shared<Timer>(60.f);

    // HitResult coll;

    // auto lastTime = system_clock::now();
    // size_t frames = 0;

    // while (!WindowShouldClose()) {
    //     timer->advanceTime();

    //     auto a = timer->getA();

    //     for (uint32_t i = 0; i < timer->getTicks(); i++) {
    //         player->tick();
    //     }

    //     player->turn(GetMouseDelta());

    //     auto playerPos = player->getPos();

    //     cam.SetCameraPosition(player->getPos());
    //     auto angle = player->getRot();
    //     cam.setAngle(angle);
    //     Vector3 target = Vector3Transform(Vector3 {0, 0, 1}, MatrixRotateZYX(Vector3 {angle.y, -angle.x, 0}));
    //     cam.setTarget(cam.GetCamera().position + target);

    //     // block selection
    //     auto camRay = cam.GetViewRay();

    //     coll.coll.hit = false;
    //     auto plBlockPos = BlockPos {(int)playerPos.x, (int)playerPos.y, (int)playerPos.z};
    //     auto chunkPos = plBlockPos.chunkPos();

    //     // TODO: rework this ._.
    //     vector<std::shared_ptr<Chunk>> chunksAroundPlayer;
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {0, 0}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {1, 0}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {0, 1}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {1, 1}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {-1, 0}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {0, -1}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {-1, -1}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {1, -1}));
    //     chunksAroundPlayer.push_back(lvl->getChunk(chunkPos + ChunkPos {-1, 1}));
    //     for (const auto& chunk : chunksAroundPlayer) {
    //         if (chunk) {
    //             chunk->cameraLook(camRay, coll, plBlockPos);
    //             if (coll.coll.hit)
    //                 break;
    //         }
    //     }

    //     if (coll.coll.hit) {
    //         if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
    //             lvl->setTile(coll.blockPos, BlockTypes::Air);
    //         }

    //         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    //             lvl->setTile(coll.blockPos + BlockPos {(int)coll.coll.normal.x, (int)coll.coll.normal.y, (int)coll.coll.normal.z},
    //                          coll.blockPos.y + (int)coll.coll.normal.y == surfaceLevel ? BlockTypes::Grass : BlockTypes::Rock);
    //         }
    //     }

    //     BeginDrawing();

    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     cam.BeginMode3D();
    //     glEnable(GL_CULL_FACE);
    //     glEnable(GL_FOG);
    //     glFogi(GL_FOG_MODE, 2048);
    //     glFogf(GL_FOG_DENSITY, .2f);
    //     glFogfv(GL_FOG_COLOR, fogColor);

    //     lvl->render();

    //     // DrawGrid(10, 1);
    //     // DrawRay({{0, 0, 0}, {1, 0, 0}}, RED);   // x
    //     // DrawRay({{0, 0, 0}, {0, 1, 0}}, GREEN); // y
    //     // DrawRay({{0, 0, 0}, {0, 0, 1}}, BLUE);  // z

    //     if (coll.coll.hit) {
    //         auto millis =
    //             std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //         auto col = Color {255, 255, 255, static_cast<uint8_t>((sin(millis / 100.0) * .2f + .4f) * 255.f)};
    //         drawFace(coll.blockPos, coll.face, col);
    //     }

    //     glDisable(GL_FOG);

    //     cam.EndMode3D();

    //     DrawFPS(0, 0);

    //     EndDrawing();

    //     frames++;
    //     if (system_clock::now() - lastTime >= 1s) {
    //         lastTime = system_clock::now();
    //         logD("{} fps", frames);
    //         frames = 0;
    //     }
    // }

    // CloseWindow();
    return 0;
}