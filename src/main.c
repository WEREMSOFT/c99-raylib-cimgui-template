#include <stdio.h>
#include <raylib.h>
#include <rlgl.h>
#include "cimgui_raylib.h"

#define WIDTH 2560
#define HEIGHT 1440

void raylib_render_draw_triangles(int count, ImDrawIdx *idx_buffer, ImDrawVert *idx_vert)
{
    // DEbug triangle (this shows us that we can draw a triangle)
    rlPushMatrix();
    rlBegin(RL_TRIANGLES);
    rlColor4ub(0xff, 0, 0, 0xff);
    rlVertex2f(0, 0);
    rlVertex2f(0, 50);
    rlVertex2f(50, 50);
    rlEnd();
    rlPopMatrix();

    // Draw the imgui triangle data
    rlPushMatrix();
    for (int i = 0; i < count; i += 3)
    {
        rlBegin(RL_QUADS);

        Color *c;

        c = (Color *)&idx_vert[i].col;
        // printf("%x\n", c);
        rlColor4ub(0xff, 0, 0xff, 0x77);
        // rlColor4ub(c->a, c->b, c->b, c->a);
        rlTexCoord2f(idx_vert[i].uv.x, idx_vert[i].uv.y);
        rlVertex2f(idx_vert[i].pos.x, idx_vert[i].pos.y);

        c = (Color *)&idx_vert[i + 1].col;
        // rlColor4ub(c->a, c->b, c->b, c->a);
        rlTexCoord2f(idx_vert[i + 1].uv.x, idx_vert[i + 1].uv.y);
        rlVertex2f(idx_vert[i + 1].pos.x, idx_vert[i + 1].pos.y);

        c = (Color *)&idx_vert[i + 2].col;
        // rlColor4ub(c->a, c->b, c->b, c->a);
        rlTexCoord2f(idx_vert[i + 2].uv.x, idx_vert[i + 2].uv.y);
        rlVertex2f(idx_vert[i + 2].pos.x, idx_vert[i + 2].pos.y);
        // Since we can not draw textured triangles with rlgl, we draw a textured quadn with the 
        // last 2 points overlapping.
        rlVertex2f(idx_vert[i + 2].pos.x, idx_vert[i + 2].pos.y);
        rlEnd();
    }
    rlPopMatrix();
}

void raylib_render_cimgui(ImDrawData *draw_data)
{
    // TODO: Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    // TODO: Setup viewport covering draw_data->DisplayPos to draw_data->DisplayPos + draw_data->DisplaySize
    // TODO: Setup orthographic projection matrix cover draw_data->DisplayPos to draw_data->DisplayPos + draw_data->DisplaySize
    // TODO: Setup shader: vertex { float2 pos, float2 uv, u32 color }, fragment shader sample color from 1 texture, multiply by vertex color.
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data; // vertex buffer generated by Dear ImGui
        const ImDrawIdx *idx_buffer = cmd_list->IdxBuffer.Data;  // index buffer generated by Dear ImGui
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = cmd_list->CmdBuffer.Data; // cmd_list->CmdBuffer->data[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // The texture for the draw call is specified by pcmd->TextureId.
                // The vast majority of draw calls will use the Dear ImGui texture atlas, which value you have set yourself during initialization.
                // MyEngineBindTexture((MyTexture *)pcmd->TextureId);
                // We are using scissoring to clip some objects. All low-level graphics API should supports it.
                // - If your engine doesn't support scissoring yet, you may ignore this at first. You will get some small glitches
                //   (some elements visible outside their bounds) but you can fix that once everything else works!
                // - Clipping coordinates are provided in imgui coordinates space (from draw_data->DisplayPos to draw_data->DisplayPos + draw_data->DisplaySize)
                //   In a single viewport application, draw_data->DisplayPos will always be (0,0) and draw_data->DisplaySize will always be == io.DisplaySize.
                //   However, in the interest of supporting multi-viewport applications in the future (see 'viewport' branch on github),
                //   always subtract draw_data->DisplayPos from clipping bounds to convert them to your viewport space.
                // - Note that pcmd->ClipRect contains Min+Max bounds. Some graphics API may use Min+Max, other may use Min+Size (size being Max-Min)
                // ImVec2 pos = draw_data->DisplayPos;
                // MyEngineScissor((int)(pcmd->ClipRect.x - pos.x), (int)(pcmd->ClipRect.y - pos.y), (int)(pcmd->ClipRect.z - pos.x), (int)(pcmd->ClipRect.w - pos.y));
                // Render 'pcmd->ElemCount/3' indexed triangles.
                // By default the indices ImDrawIdx are 16-bit, you can change them to 32-bit in imconfig.h if your engine doesn't support 16-bit indices.
                rlEnableTexture(pcmd->TextureId);
                raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer);
                rlDisableTexture();
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
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
    ImDrawData *draw_data;

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
    Image image = GenImageColor(WIDTH, HEIGHT, WHITE);
    Texture2D texture = LoadTextureFromImage(image); //MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA32)
    io->Fonts->TexID = (void *)&texture.id;

    // Setup Camera
    Camera3D camera = {0};
    camera.fovy = 45.0f;
    camera.target = (Vector3){.0f, .0f, .0f};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_ORBITAL);

    // This is just to have something to play with.
    Vector3 cube_position = {0.0f, 0.0f, 0.0f};

    // Main loop
    while (!WindowShouldClose())
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
            }
            EndMode3D();
            
            igShowDemoWindow(NULL);
            igRender();
            draw_data = igGetDrawData();
            raylib_render_cimgui(draw_data);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}