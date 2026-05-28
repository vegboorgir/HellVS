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
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawCylinder(PlatformPos, 10.0f,10.0f,0.0f,24,GRAY);
                DrawCylinderWires(PlatformPos, 10.0f,10.0f,0.0f,24, WHITE);
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

