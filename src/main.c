#include "raylib.h"
#include "raymath.h"
#include "twoqubit.h"
#include "gates.h"
#include "render.h"
#include "oracle.h"
#include "dj.h"
#include <time.h>
#include <stdlib.h>

#define SLERP_SPEED 8.0f

int main(void)
{
    const int screenWidth = 900;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "Two-Qubit Entanglement Simulator");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 4.0f, 6.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 center0 = (Vector3){-1.8f, 0.0f, 0.0f};
    Vector3 center1 = (Vector3){1.8f, 0.0f, 0.0f};

    TwoQubit s = twoqubit_init();
    int last_q0 = -1, last_q1 = -1;

    OracleType dj_last_oracle = ORACLE_CONSTANT_0;
    int dj_active = 0; // whether the last state came from a DJ run (for HUD labeling)

    Vector3 displayPos0 = twoqubit_reduced_bloch(&s, 0);
    Vector3 displayPos1 = twoqubit_reduced_bloch(&s, 1);

    while (!WindowShouldClose())
    {
        // qubit 0 gates: lowercase
        if (IsKeyPressed(KEY_H))
            twoqubit_apply_gate_q0(&s, &GATE_H);
        if (IsKeyPressed(KEY_X))
            twoqubit_apply_gate_q0(&s, &GATE_X);
        if (IsKeyPressed(KEY_Y))
            twoqubit_apply_gate_q0(&s, &GATE_Y);
        if (IsKeyPressed(KEY_Z))
            twoqubit_apply_gate_q0(&s, &GATE_Z);

        // qubit 1 gates: shift + same key -> use LEFT_SHIFT check
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            twoqubit_apply_gate_q1(&s, &GATE_H);
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_X))
            twoqubit_apply_gate_q1(&s, &GATE_X);
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Y))
            twoqubit_apply_gate_q1(&s, &GATE_Y);
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Z))
            twoqubit_apply_gate_q1(&s, &GATE_Z);

        if (IsKeyPressed(KEY_C))
            twoqubit_apply_cnot(&s, 0, 1); // control=q0, target=q1

        if (IsKeyPressed(KEY_J))
        {
            dj_last_oracle = oracle_random();
            dj_prepare(&s, dj_last_oracle);
            dj_active = 1;
            last_q0 = last_q1 = -1;
        }

        if (IsKeyPressed(KEY_R))
        {
            s = twoqubit_init();
            last_q0 = last_q1 = -1;
            dj_active = 0;
        }

        if (IsKeyPressed(KEY_M))
        {
            twoqubit_measure(&s, &last_q0, &last_q1);
            dj_active = 0;
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);

        Vector3 target0 = twoqubit_reduced_bloch(&s, 0);
        Vector3 target1 = twoqubit_reduced_bloch(&s, 1);
        float t = 1.0f - expf(-SLERP_SPEED * GetFrameTime());
        displayPos0 = bloch_slerp(displayPos0, target0, t);
        displayPos1 = bloch_slerp(displayPos1, target1, t);

        double concurrence = twoqubit_concurrence(&s);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        render_bloch_frame(center0);
        render_bloch_frame(center1);
        render_bloch_marker(center0, displayPos0);
        render_bloch_marker(center1, displayPos1);
        EndMode3D();

        DrawText("Qubit 0", (int)(screenWidth / 2 - 220), 100, 20, DARKGRAY);
        DrawText("Qubit 1", (int)(screenWidth / 2 + 150), 100, 20, DARKGRAY);

        render_hud(&s, last_q0, last_q1, concurrence);

        if (dj_active)
        {
            DrawText(TextFormat("Oracle: %s", oracle_name(dj_last_oracle)), 10, 85, 18, DARKBLUE);
            if (last_q0 >= 0)
            {
                const char *guess = (last_q0 == 0) ? "constant" : "balanced";
                int correct = (last_q0 == 0) == oracle_is_constant(dj_last_oracle);
                DrawText(TextFormat("Algorithm says: %s  (%s)", guess, correct ? "correct" : "WRONG"),
                         10, 105, 18, correct ? DARKGREEN : RED);
            }
        }
        DrawText("[H/X/Y/Z] q0 gates  [SHIFT+H/X/Y/Z] q1 gates  [C] CNOT  [J] DJ  [M] Measure  [R] Reset",
                 10, screenHeight - 30, 16, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}