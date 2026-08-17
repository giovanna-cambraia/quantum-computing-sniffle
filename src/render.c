#include "render.h"
#include "raymath.h"

void render_bloch_frame(void) {
    DrawSphereWires((Vector3){0, 0, 0}, 1.0f, 16, 16, LIGHTGRAY);

    DrawLine3D((Vector3){-1.3f, 0, 0}, (Vector3){1.3f, 0, 0}, GRAY); // x 
    DrawLine3D((Vector3){0, -1.3f, 0}, (Vector3){0, 1.3f, 0}, GRAY); // y 
    DrawLine3D((Vector3){0, 0, -1.3f}, (Vector3){0, 0, 1.3f}, GRAY); // z 
}

void render_bloch_marker(Vector3 pos) {
    DrawLine3D((Vector3){0, 0, 0}, pos, RED);
    DrawSphere(pos, 0.06f, RED);
}

void render_hud(const Qubit *q, int last_measurement) {
    double p0 = qubit_prob0(q);
    double p1 = qubit_prob1(q);

    DrawText(TextFormat("P(|0>) = %.2f   P(|1>) = %.2f", p0, p1), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("alpha = %.2f + %.2fi", creal(q->alpha), cimag(q->alpha)), 10, 35, 18, DARKGRAY);
    DrawText(TextFormat("beta  = %.2f + %.2fi", creal(q->beta), cimag(q->beta)), 10, 55, 18, DARKGRAY);

    if (last_measurement >= 0) {
        DrawText(TextFormat("Last measurement: |%d>", last_measurement), 10, 85, 18, MAROON);
    }

    DrawText("H X Y Z S T = gates   R = reset   M = measure", 10, 570, 18, GRAY);
}

Vector3 bloch_slerp(Vector3 from, Vector3 to, float t) {
    float dot = Vector3DotProduct(from, to);
    if (dot > 0.9995f) {
        // vectors nearly identical — linear lerp avoids a divide-by-zero below 
        Vector3 result = Vector3Lerp(from, to, t);
        return Vector3Normalize(result);
    }

    if (dot < -1.0f) dot = -1.0f;
    if (dot > 1.0f)  dot = 1.0f;

    float theta = acosf(dot) * t;

    Vector3 relative = Vector3Subtract(to, Vector3Scale(from, dot));
    relative = Vector3Normalize(relative);

    Vector3 a = Vector3Scale(from, cosf(theta));
    Vector3 b = Vector3Scale(relative, sinf(theta));
    return Vector3Add(a, b);
}