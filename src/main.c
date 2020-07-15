#include <stdio.h>
#include <raylib.h>
#include <rlgl.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#include "cimgui_impl_raylib.h"
#define WIDTH 1024
#define HEIGHT 768

Camera3D camera = {0};
ImDrawData *draw_data;
// This is just to have something to play with.
Vector3 cube_position = {0.0f, 0.0f, 0.0f};



void update_frame()
{

    if (IsKeyDown(KEY_KP_ADD))
        camera.fovy += 1.0f;
    if (IsKeyDown(KEY_KP_SUBTRACT))
        camera.fovy -= 1.0f;

    if (IsKeyPressed(KEY_LEFT))
        cube_position.x -= 1.0f;
    if (IsKeyPressed(KEY_RIGHT))
        cube_position.x += 1.0f;
    if (IsKeyPressed(KEY_UP))
        cube_position.z -= 1.0f;
    if (IsKeyPressed(KEY_DOWN))
        cube_position.z += 1.0f;

    UpdateCamera(&camera);
    ImGui_ImplRaylib_NewFrame();
    ImGui_ImplRaylib_ProcessEvent();
    igNewFrame();

    BeginDrawing();
    {
        ClearBackground(WHITE);
        DrawFPS(10, 10);

        BeginMode3D(camera);
        {
            DrawCube(cube_position, 1, 1, 1, RED);
            DrawCubeWires(cube_position, 1, 1, 1, BLUE);
            DrawGrid(10, 1);
        }
        EndMode3D();
        igShowDemoWindow(NULL);
        igRender();
        draw_data = igGetDrawData();
        raylib_render_cimgui(draw_data);
    }
    EndDrawing();
}

int main(void)
{
#ifdef OS_Windows_NT
    printf("Windows dettected\n");
#elif defined OS_Linux
    printf("LINUS dettected\n");
#elif defined OS_Darwin
    printf("MacOS dettected\n");
#endif

    // Init Raylib
    InitWindow(WIDTH, HEIGHT, "This is a imgui test");
    SetTargetFPS(60);

    // cimgui variables
    struct ImGuiContext *ctx;
    struct ImGuiIO *io;

    // Initialize imgui
    igCreateContext(NULL);
    igStyleColorsDark(NULL);
    // Initialize keyboard and mouse events
    ImGui_ImplRaylib_Init();

    // Create textures

    // Build and load the texture atlas into a texture
    // (In the examples/ app this is usually done within the ImGui_ImplXXX_Init() function from one of the demo Renderer)
    io = igGetIO();
    unsigned char *pixels = NULL;

    int width = WIDTH;
    int height = HEIGHT;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, NULL);
    // At this point you've got the texture data and you need to upload that your your graphic system:
    // After we have created the texture, store its pointer/identifier (_in whichever format your engine uses_) in 'io.Fonts->TexID'.
    // This will be passed back to your via the renderer. Basically ImTextureID == void*. Read FAQ for details about ImTextureID.
    Image image = LoadImageEx(pixels, width, height);
    Texture2D texture = LoadTextureFromImage(image); //MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA32)
    io->Fonts->TexID = (void *)&texture.id;

    // Setup Camera
    camera.fovy = 45.0f;
    camera.target = (Vector3){.0f, .0f, .0f};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_ORBITAL);

    // Main loop

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update_frame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        update_frame();
    }
#endif
    CloseWindow();

    return 0;
}