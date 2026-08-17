#include "raylib.h"
#include "raymath.h"
#include "qubit.h"
#include "gates.h"
#include "render.h"
#include <time.h>
#include <stdlib.h>

#define SLERP_SPEED 8.0f // higher = snappier animation, lower = dreamier 

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Qubit Superposition Simulator");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 3.0f, 3.0f, 3.0f };
    camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Qubit q = qubit_init();
    int last_measurement = -1;

    Vector3 displayPos = qubit_to_bloch(&q); 

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_H)) qubit_apply_gate(&q, &GATE_H);
        if (IsKeyPressed(KEY_X)) qubit_apply_gate(&q, &GATE_X);
        if (IsKeyPressed(KEY_Y)) qubit_apply_gate(&q, &GATE_Y);
        if (IsKeyPressed(KEY_Z)) qubit_apply_gate(&q, &GATE_Z);
        if (IsKeyPressed(KEY_S)) qubit_apply_gate(&q, &GATE_S);
        if (IsKeyPressed(KEY_T)) qubit_apply_gate(&q, &GATE_T);
        if (IsKeyPressed(KEY_R)) { q = qubit_init(); last_measurement = -1; }
        if (IsKeyPressed(KEY_M)) last_measurement = qubit_measure(&q);

        UpdateCamera(&camera, CAMERA_ORBITAL);

        // chase the real Bloch position each frame instead of snapping to it 
        Vector3 targetPos = qubit_to_bloch(&q);
        float t = 1.0f - expf(-SLERP_SPEED * GetFrameTime());
        displayPos = bloch_slerp(displayPos, targetPos, t);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                render_bloch_frame();
                render_bloch_marker(displayPos);
            EndMode3D();

            render_hud(&q, last_measurement);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}