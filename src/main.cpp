#include "raylib.h"

int main(void){
    
    int screenWidhth = 800;
    int screenHeight = 450;
    InitWindow(screenWidhth,screenHeight,"title");

    Vector3 PlatformPos = {0.0f, 0.0f, 0.0f};

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f,2.0f,5.0f};
    camera.target = (Vector3){0.0f,2.0f,0.0f};
    camera.up= (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 0.3f : 0.1f;

        Vector3 movement = {0};
        if(IsKeyDown(KEY_W)) movement.x = speed;
        if(IsKeyDown(KEY_S)) movement.x = -speed;
        if(IsKeyDown(KEY_A)) movement.y = -speed;
        if(IsKeyDown(KEY_D)) movement.y = speed;

        Vector3 rotation = {
            GetMouseDelta().x * 0.05f,
            GetMouseDelta().y * 0.05f,
            0.0f
        };

        UpdateCameraPro(&camera, movement, rotation,0.0f);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawCylinder(PlatformPos, 30.0f,30.0f,0.0f,24,GRAY);
                DrawCylinderWires(PlatformPos, 30.0f,30.0f,0.0f,24, WHITE);
                DrawGrid(50,1.0f);
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

