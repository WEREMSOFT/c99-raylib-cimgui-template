#include <stdio.h>
#include <raylib.h>
#include "cimgui_raylib.h"
// #include <imgui_impl_opengl3.h>

#define WIDTH 800
#define HEIGHT 600

int main(void){
    #ifdef OS_Windows_NT
    printf("Windows dettected\n");
    #elif defined OS_Linux
    printf("LINUS dettected\n");
    #elif defined OS_Darwin
    printf("MacOS dettected\n");
    #endif

    struct ImGuiContext* ctx;
    struct ImGuiIO* io;

    InitWindow(WIDTH, HEIGHT, "This is a imgui test");
    SetTargetFPS(60);

    ctx = igCreateContext(NULL);
    io = igGetIO();

    igStyleColorsDark(NULL);

    ImGui_ImplRaylib_Init();
    unsigned char *text_pixels = NULL;
    int text_w, text_h;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &text_pixels, &text_w, &text_h, NULL);

    Camera3D camera = {0};

    camera.fovy = 45.0f;
    camera.target = (Vector3) {.0f, .0f, .0f};
    camera.position = (Vector3) { 0.0f, 10.0f, 10.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.type = CAMERA_PERSPECTIVE;
    
    Vector3 cube_position = {0.0f, 0.0f, 0.0f};

    ImDrawData* draw_data;

    while(!WindowShouldClose()){

        ImVec2 display_size;
        display_size.x = WIDTH;
        display_size.y = HEIGHT;
        io->DisplaySize = display_size;
        io->DeltaTime = 1.0f / 60.0f;

        ImGui_ImplRaylib_NewFrame();
        igNewFrame();
        ImGui_ImplRaylib_ProcessEvent();


        BeginDrawing();
        {
            ClearBackground(WHITE);
            DrawFPS(10, 10);

            BeginMode3D(camera);
            {
                DrawCube(cube_position, 1, 1, 1, RED);
                DrawCubeWires(cube_position, 1, 1, 1, BLUE);
                DrawGrid(10, 1);
            } EndMode3D();

            if(IsKeyDown(KEY_KP_ADD)) camera.fovy += 1.0f;
            if(IsKeyDown(KEY_KP_SUBTRACT)) camera.fovy -= 1.0f;

            if(IsKeyPressed(KEY_LEFT)) cube_position.x -= 1.0f;
            if(IsKeyPressed(KEY_RIGHT)) cube_position.x += 1.0f;
            if(IsKeyPressed(KEY_UP)) cube_position.z -= 1.0f;
            if(IsKeyPressed(KEY_DOWN)) cube_position.z += 1.0f;

            igShowDemoWindow(false);
            igRender();
            draw_data = igGetDrawData();
        } EndDrawing();
    }

    CloseWindow();

    return 0;
}