#include "render.h"

void render_bloch_frame(void)
{
    DrawSphereWires((Vector3){0, 0, 0}, 1.0f, 16, 16, LIGHTGRAY);

    // axes
    DrawLine3D((Vector3){-1.3f, 0, 0}, (Vector3){1.3f, 0, 0}, GRAY)     // x
        DrawLine3D((Vector3){0, -1.3f, 0}, (Vector3){0, 1.3f, 0}, GRAY) // y
        DrawLine3D((Vector3){0, 0, -1.3f}, (Vector3){0, 0, 1.3f}, GRAY) // z: |0>..|1>
}

void render_bloch_state(const Qubit *q)
{
    Vector3 pos = qubit_to_blonch(q);
    DrawLine3D((Vector3){0, 0, 0}, pos, RED);
    DrawSphere(pos, 0.06f, RED);
}

void render_hud(const Qubit *q, int last_measurement)
{
    double p0 = qubit_prob0(q);
    double p1 = qubit_prob1(q);

    DrawText(TextFormat("P(|0>) = %.2f   P(|1>) = %.2f", p0, p1), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("alpha = %.2f + %.2fi", creal(q->alpha), cimag(q->alpha)), 10, 35, 18, DARKGRAY);
    DrawText(TextFormat("beta  = %.2f + %.2fi", creal(q->beta), cimag(q->beta)), 10, 55, 18, DARKGRAY);

    if (last_measurement >= 0)
    {
        DrawText(TextFormat("Last measurement: |%d>", last_measurement), 10, 85, 18, MAROON);
    }

    DrawText("H X Y Z S T = gates   R = reset   M = measure", 10, 570, 18, GRAY);
}