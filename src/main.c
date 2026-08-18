#include "raylib.h"
#include "raymath.h"
#include "twoqubit.h"
#include "threequbit.h"
#include "gates.h"
#include "render.h"
#include "oracle.h"
#include "dj.h"
#include "teleport.h"
#include "bars.h"
#include "ghz.h"
#include "grover.h"
#include "decoherence.h"
#include <time.h>
#include <stdlib.h>

#define SLERP_SPEED 8.0f

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Two-Qubit Entanglement Simulator");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 3.0f, 6.5f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 center0 = (Vector3){-1.6f, 0.9f, 0.0f};
    Vector3 center1 = (Vector3){1.6f, 0.9f, 0.0f};
    Vector3 center2 = (Vector3){0.0f, -1.1f, 0.0f};

    TwoQubit s = twoqubit_init();
    int last_q0 = -1, last_q1 = -1;

    OracleType dj_last_oracle = ORACLE_CONSTANT_0;
    int dj_active = 0;

    ThreeQubit t = threequbit_init();
    Vector3 preTeleportBloch = {0, 0, 0};
    int tp_m0 = -1, tp_m1 = -1;
    int tp_done = 0;

    int ghz_active = 0;
    int ghz_m0 = -1, ghz_m1 = -1, ghz_m2 = -1;

    int grover_active = 0;
    GroverTarget grover_target = 3; // marks |11> 

    // Decoherence state variables
    int noise_active = 0;
    double noise_strength = 0.3; 

    Vector3 displayPos0 = twoqubit_reduced_bloch(&s, 0);
    Vector3 displayPos1 = twoqubit_reduced_bloch(&s, 1);
    Vector3 displayPosT = threequbit_reduced_bloch(&t, 2);

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

        // P: prepare a random-ish message state on q0
        if (IsKeyPressed(KEY_P))
        {
            t = threequbit_init();
            threequbit_apply_gate(&t, 0, &GATE_H);
            threequbit_apply_gate(&t, 0, &GATE_T);
            preTeleportBloch = threequbit_reduced_bloch(&t, 0);
            tp_done = 0;
            tp_m0 = tp_m1 = -1;
        }

        if (IsKeyPressed(KEY_K))
        {
            teleport_run(&t, &tp_m0, &tp_m1);
            tp_done = 1;
        }

        if (IsKeyPressed(KEY_G))
        {
            ghz_prepare(&t);
            ghz_active = 1;
            tp_done = 0; 
            ghz_m0 = ghz_m1 = ghz_m2 = -1;
        }

        if (IsKeyPressed(KEY_V))
        {
            if (!grover_active)
            {
                grover_init_superposition(&s);
                grover_active = 1;
                dj_active = 0;
                ghz_active = 0;
                last_q0 = last_q1 = -1;
            }
            else
            {
                grover_iterate(&s, grover_target);
            }
        }

        // N: Toggle decoherence
        if (IsKeyPressed(KEY_N))
            noise_active = !noise_active;

        // UP/DOWN: Adjust decoherence strength
        if (IsKeyDown(KEY_UP))
        {
            noise_strength += 0.3 * GetFrameTime();
            if (noise_strength > 1.0)
                noise_strength = 1.0;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            noise_strength -= 0.3 * GetFrameTime();
            if (noise_strength < 0.0)
                noise_strength = 0.0;
        }

        if (IsKeyPressed(KEY_R))
        {
            s = twoqubit_init();
            last_q0 = last_q1 = -1;
            dj_active = 0;

            t = threequbit_init();
            preTeleportBloch = (Vector3){0, 0, 0};
            tp_m0 = tp_m1 = -1;
            tp_done = 0;

            ghz_active = 0;
            ghz_m0 = ghz_m1 = ghz_m2 = -1;

            grover_active = 0;
        }

        if (IsKeyPressed(KEY_M))
        {
            if (ghz_active)
            {
                ghz_m0 = threequbit_measure_single(&t, 0);
                ghz_m1 = threequbit_measure_single(&t, 1);
                ghz_m2 = threequbit_measure_single(&t, 2);
            }
            else
            {
                twoqubit_measure(&s, &last_q0, &last_q1);
                dj_active = 0;
                grover_active = 0;
            }
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Apply decoherence every frame if active
        if (noise_active) {
            decoherence_step_q0(&s, noise_strength, GetFrameTime());
        }

        Vector3 target0, target1, targetT;
        if (ghz_active)
        {
            target0 = threequbit_reduced_bloch(&t, 0);
            target1 = threequbit_reduced_bloch(&t, 1);
            targetT = threequbit_reduced_bloch(&t, 2);
        }
        else
        {
            target0 = twoqubit_reduced_bloch(&s, 0);
            target1 = twoqubit_reduced_bloch(&s, 1);
            targetT = threequbit_reduced_bloch(&t, 2); // still shows teleport target when not in GHZ 
        }

        float t_slerp = 1.0f - expf(-SLERP_SPEED * GetFrameTime());
        displayPos0 = bloch_slerp(displayPos0, target0, t_slerp);
        displayPos1 = bloch_slerp(displayPos1, target1, t_slerp);
        displayPosT = bloch_slerp(displayPosT, targetT, t_slerp);

        double concurrence = twoqubit_concurrence(&s);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        render_bloch_frame(center0);
        render_bloch_marker(center0, displayPos0);
        render_bloch_frame(center1);
        render_bloch_marker(center1, displayPos1);
        render_bloch_frame(center2);
        render_bloch_marker(center2, displayPosT);
        EndMode3D();

        // 3D-aware labels that follow the spheres (1.0f clearance above 0.8f radius)
        Vector2 label0 = GetWorldToScreen(Vector3Add(center0, (Vector3){0, 1.0f, 0}), camera);
        Vector2 label1 = GetWorldToScreen(Vector3Add(center1, (Vector3){0, 1.0f, 0}), camera);
        Vector2 label2 = GetWorldToScreen(Vector3Add(center2, (Vector3){0, 1.0f, 0}), camera);

        DrawText("Qubit 0", (int)label0.x - 35, (int)label0.y, 20, DARKGRAY);
        DrawText("Qubit 1", (int)label1.x - 35, (int)label1.y, 20, DARKGRAY);
        DrawText("Qubit 2 (teleport target)", (int)label2.x - 110, (int)label2.y, 20, DARKGRAY);

        render_hud(&s, last_q0, last_q1, concurrence);

        // amplitude bars - positioned with breathing room from hud text
        render_amplitude_bars(&s, 20, 300, 300, 120);

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

        if (tp_done && !ghz_active)
        {
            Vector3 post = threequbit_reduced_bloch(&t, 2);
            DrawText(TextFormat("Alice measured: m0=%d m1=%d", tp_m0, tp_m1), 10, 130, 18, DARKBLUE);
            DrawText(TextFormat("q0 before: (%.2f,%.2f,%.2f)  q2 after: (%.2f,%.2f,%.2f)",
                                preTeleportBloch.x, preTeleportBloch.y, preTeleportBloch.z,
                                post.x, post.y, post.z),
                     10, 150, 16, DARKGREEN);
        }

        if (ghz_active)
        {
            DrawText("GHZ state: (|000> + |111>) / sqrt(2)", 10, 85, 18, DARKBLUE);
            if (ghz_m0 >= 0)
            {
                int allAgree = (ghz_m0 == ghz_m1) && (ghz_m1 == ghz_m2);
                DrawText(TextFormat("Measured: q0=%d q1=%d q2=%d  (%s)",
                                    ghz_m0, ghz_m1, ghz_m2, allAgree ? "all agree" : "MISMATCH?!"),
                         10, 105, 18, allAgree ? DARKGREEN : RED);
            }
        }

        if (grover_active)
        {
            DrawText("Grover search: target = |11>", 10, 85, 18, DARKBLUE);
            DrawText("Press V again to run an iteration", 10, 105, 18, DARKGRAY);
        }

        if (noise_active) {
            DrawText(TextFormat("Decoherence ON — strength %.2f (UP/DOWN to adjust)", noise_strength),
                10, 85, 18, RED);
        }

        DrawText("[H/X/Y/Z] q0 gates  [SHIFT+H/X/Y/Z] q1 gates  [C] CNOT  [J] DJ  [P] Prep teleport  [K] Teleport  [G] GHZ  [V] Grover  [N] Noise  [M] Measure  [R] Reset",
                 10, screenHeight - 30, 16, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}