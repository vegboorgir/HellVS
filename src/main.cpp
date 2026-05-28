#include "raylib.h"
#include <cmath>
#include <math.h>

int main(void){
    
    int screenWidth = 800;
    int screenHeight = 450;
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    int monitor = GetCurrentMonitor();
    InitWindow(GetMonitorWidth(monitor),GetMonitorHeight(monitor),"title");

    Vector3 PlatformPos = {0.0f, 0.0f, 0.0f};

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f,3.0f,5.0f};
    camera.target = (Vector3){0.0f,3.0f,0.0f};
    camera.up= (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    float velocityY = 0.0f;
    float gravity = -0.01f;
    float jumpForce = 0.25f;
    float groundY = 3.0f;

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 0.2f : 0.1f;

        Vector3 movement = {0};
        if(IsKeyDown(KEY_W)) movement.x = speed;
        if(IsKeyDown(KEY_S)) movement.x = -speed;
        if(IsKeyDown(KEY_A)) movement.y = -speed;
        if(IsKeyDown(KEY_D)) movement.y = speed;
        if(IsKeyPressed(KEY_F11)) ToggleFullscreen(); 

        Vector3 rotation = {
            GetMouseDelta().x * 0.07f,
            GetMouseDelta().y * 0.07f,
            0.0f
        };

        UpdateCameraPro(&camera, movement, rotation,0.0f);

        if(IsKeyPressed(KEY_SPACE) && camera.position.y <= groundY)
            velocityY = jumpForce;

        velocityY += gravity;
        float diff = camera.target.y - camera.position.y;
        camera.position.y += velocityY;
        camera.target.y = camera.position.y + diff;

        if(camera.position.y < groundY){
            camera.target.y += (groundY - camera.position.y);
            camera.position.y = groundY;
            velocityY = 0.0f;
        }

        float platformRadius = 29.0f;
        float dx = camera.position.x - PlatformPos.x;
        float dz = camera.position.z - PlatformPos.z;
        float dist = sqrtf(dx*dx + dz*dz);

        if(dist>platformRadius){
            camera.position.x = PlatformPos.x + (dx/dist) * platformRadius;
            camera.position.z = PlatformPos.z + (dz/dist) * platformRadius;
            camera.target.x = camera.position.x + (camera.target.x - camera.position.x);
            camera.target.z = camera.position.z + (camera.target.z - camera.position.z);
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
            (Color){5, 0, 10, 255},
            (Color){80, 0, 0, 255});

            BeginMode3D(camera);
                DrawCylinder(PlatformPos, 30.0f,30.0f,0.5f,24,(Color){40,10,5,255});
                DrawCylinderWires(PlatformPos, 30.0f,30.0f,0.5f,24,(Color){200,50,0,255});
                DrawGrid(50,1.0f);
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

