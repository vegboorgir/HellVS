#include "raylib.h"
#include "raymath.h" 
#include "rlgl.h"
#include <cmath>
#include <ctime>
#include <math.h>

int main(void){
    
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    int monitor = GetCurrentMonitor();
    InitWindow(GetMonitorWidth(monitor),GetMonitorHeight(monitor),"title");

    Vector3 PlatformPos = {0.0f, 0.0f, 0.0f};
    Vector3 EnemyPos = {1.0f, 3.0f, 25.0f};

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f,3.0f,-25.0f};
    camera.target = (Vector3){0.0f,3.0f,25.0f};
    camera.up= (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    float velocityY = 0.0f;
    float gravity = -0.01f;
    float jumpForce = 0.25f;
    float groundY = 3.0f;
    float enemySpeed = 3.0f;
    float duelRange = 4.0f;
    float enemyVelocityY = 0.0f;

    float attackTimer = 3.0f;
    float coolDown = 3.0f;
    bool isAttacking = false;

    Vector3 attackStartPos = {0};
    Vector3 attackTargetPos = {0};
    float attackT = 0.0f;

    float playerHP = 100.0f;
    float damage = 10.0f;
    bool hitRegistered = false;
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

        enemyVelocityY += gravity;
        EnemyPos.y += enemyVelocityY;

        if(EnemyPos.y <= groundY){
            EnemyPos.y = groundY;
            enemyVelocityY = 0.0f;
        }
        if(!isAttacking){
            Vector3 dir = Vector3Subtract(camera.position, EnemyPos);
            float distToPlayer = Vector3Length(dir);
            Vector3 dirNorm = Vector3Normalize(dir);

            if(distToPlayer > duelRange){
                EnemyPos = Vector3Add(EnemyPos, Vector3Scale(dirNorm, enemySpeed * GetFrameTime()));
            }else if (distToPlayer < duelRange - 1.0f) {
                EnemyPos = Vector3Subtract(EnemyPos, Vector3Scale(dirNorm, enemySpeed * GetFrameTime()));
            }

            attackTimer -= GetFrameTime();
            if(attackTimer <= 0.0f){
                isAttacking = true;
                attackStartPos = EnemyPos;
                attackTargetPos = camera.position;
                attackT = 0.0f;
            }
        }else{
            attackT += GetFrameTime() * 4.0f;
            if(attackT > 1.0f) attackT  = 1.0f;

            EnemyPos = Vector3Lerp(attackStartPos,attackTargetPos,attackT);

            float lungeDist = Vector3Distance(EnemyPos, camera.position);
            if(lungeDist < 2.0f && !hitRegistered){
                playerHP -= damage;
                hitRegistered = true;
            }

            if(attackT >= 1.0f){
                isAttacking = false;
                attackTimer = coolDown;
                attackT = 0.0f;
                hitRegistered = false;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
            (Color){5, 0, 10, 255},
            (Color){80, 0, 0, 255});

            BeginMode3D(camera);
                DrawCylinder(PlatformPos, 30.0f,30.0f,0.5f,24,(Color){40,10,5,255});
                DrawCylinderWires(PlatformPos, 30.0f,30.0f,0.5f,24,(Color){200,50,0,255});
                DrawCapsule({EnemyPos.x, EnemyPos.y + 1.0f, EnemyPos.z}, {EnemyPos.x, EnemyPos.y - 1.0f, EnemyPos.z}, 0.5f, 8, 8, RED);

                Vector3 toPlayer = Vector3Normalize(Vector3Subtract(camera.position, EnemyPos));
                Vector3 enemyRight = Vector3CrossProduct(toPlayer, (Vector3){0,1,0});

                Vector3 spearOrigin = Vector3Add(EnemyPos, Vector3Scale(toPlayer,-1.5f));
                spearOrigin = Vector3Add(spearOrigin, Vector3Scale(enemyRight, 0.6f));
                spearOrigin.y += 0.5f;

                spearOrigin.y += 0.5f;
                Vector3 spearStart = Vector3Add(spearOrigin, Vector3Scale(toPlayer, 0.5f));
                Vector3 spearEnd = Vector3Add(spearOrigin, Vector3Scale(toPlayer, 4.0f));

                DrawCylinderEx(spearStart, spearEnd, 0.08f, 0.04f, 8, GRAY);
                DrawCylinderEx(spearEnd, Vector3Add(spearEnd, Vector3Scale(toPlayer, 0.5f)), 0.04f,0.0f, 8, GRAY);
            EndMode3D();

            rlDrawRenderBatchActive();
            rlDisableDepthTest();

            BeginMode3D(camera);
                Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
                Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
                Vector3 up =  Vector3CrossProduct(right, forward);

                Vector3 handPos = Vector3Add(camera.position, Vector3Scale(forward, 0.3f));
                handPos = Vector3Add(handPos, Vector3Scale(right, 0.2f));
                handPos = Vector3Add(handPos, Vector3Scale(up, -0.15f));

                Vector3 handBack = Vector3Add(handPos, Vector3Scale(forward, -0.15f));
                Vector3 handFront = Vector3Add(handPos, Vector3Scale(forward, 0.15));

                DrawCylinderEx(handBack, handFront, 0.05f, 0.05f, 8, BROWN);
            EndMode3D();

            rlDrawRenderBatchActive();
            rlEnableDepthTest();

            DrawText(TextFormat("HP: %.0f", playerHP), 20,20,30, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

