#include "raylib.h"

int main(void){
    
    int screenWidhth = 800;
    int screenHeight = 450;
    InitWindow(screenWidhth,screenHeight,"title");

    Vector3 cubePositon = {0.0f, 0.0f, 0.0f};

    Camera3D camera = { 0 };
    camera.up= (Vector3){0.0f,10.0f,0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    SetTargetFPS(60);

    while (!WindowShouldClose()){
        if(IsKeyDown(KEY_W)) cubePositon.z -= 0.1f;
        if(IsKeyDown(KEY_S)) cubePositon.z += 0.1f;

        camera.target = cubePositon;
        camera.position = (Vector3){cubePositon.x, cubePositon.y + 5.0f, cubePositon.z + 10.0f};

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        DrawCube(cubePositon, 1.0f,1.0f,1.0f, RED);
        DrawGrid(10, 1.0f);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

