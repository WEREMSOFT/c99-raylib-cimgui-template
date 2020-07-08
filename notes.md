## Notas del desarrollo

_(you may also go to Demo>About Window, and click "Config/Build Information" to obtain a bunch of detailed information that you can paste here)_

**Version/Branch of Dear ImGui:**

Version: 1.76
Branch: master

**OpenGL3.0/Raylib/gcc/Linux**

Back-ends: I'm using RLGL, an openGL abstraction layer for raylib with the objective of creating an implementation for it.
Compiler: GCC 9.3.0
Operating System: Linux/Ubuntu

**My Issue/Question:**

I'm trying to create an implementation for the raylib game engine (raylib.com), this uses a custom abstraction layer for openGL called rlgl.

I managed to create the functions needed to make umgui run(using cimgui because raylib is writen in C). The problem is that under certain circumstances, the index buffers contain values out of bound with respect of the vertex buffer.

```
void raylib_render_draw_triangles(unsigned int count, const ImDrawIdx *idx_buffer, const ImDrawVert *idx_vert, unsigned int texture_id)
{
    // Draw the imgui triangle data
    rlPushMatrix();
    for (unsigned int i = 0; i < (count - 3); i += 3)
    {
        rlBegin(RL_TRIANGLES);
        rlEnableTexture(texture_id);

        ImDrawIdx index;
        ImDrawVert vertex;
        index = idx_buffer[i]; // idx_buffer contains a numer way higuer than the count parameter
        vertex = idx_vert[index]; // crash by Out of bounds.
        draw_triangle_vertex(vertex);
        
        index = idx_buffer[i+2];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);
        
        index = idx_buffer[i+1];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);
        rlDisableTexture();
        rlEnd();
        rlPopMatrix();
    }
}
```

**Screenshots/Video**

XXX _(you can drag files here)_

**Standalone, minimal, complete and verifiable example:** _(see https://github.com/ocornut/imgui/issues/2261)_
```
// Here's some code anyone can copy and paste to reproduce your issue
ImGui::Begin("Example Bug");
MoreCodeToExplainMyIssue();
ImGui::End();
```




