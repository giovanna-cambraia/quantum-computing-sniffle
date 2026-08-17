#include "render.h"
#include "raymath.h"

void render_bloch_frame(Vector3 center)
{
    DrawSphereWires(center, 0.8f, 16, 16, LIGHTGRAY);

    DrawLine3D(Vector3Add(center, (Vector3){-1.1f, 0, 0}), Vector3Add(center, (Vector3){1.1f, 0, 0}), GRAY);
    DrawLine3D(Vector3Add(center, (Vector3){0, -1.1f, 0}), Vector3Add(center, (Vector3){0, 1.1f, 0}), GRAY);
    DrawLine3D(Vector3Add(center, (Vector3){0, 0, -1.1f}), Vector3Add(center, (Vector3){0, 0, 1.1f}), GRAY);
}

void render_bloch_marker(Vector3 center, Vector3 localPos)
{
    Vector3 worldPos = Vector3Add(center, localPos);
    DrawLine3D(center, worldPos, RED);
    DrawSphere(worldPos, 0.05f, RED);
}

void render_hud(const TwoQubit *s, int last_q0, int last_q1, double concurrence)
{
    DrawText(TextFormat("P(00)=%.2f  P(01)=%.2f  P(10)=%.2f  P(11)=%.2f",
                        twoqubit_prob(s, 0), twoqubit_prob(s, 1), twoqubit_prob(s, 2), twoqubit_prob(s, 3)),
             10, 10, 20, DARKGRAY);

    DrawText(TextFormat("Concurrence (entanglement): %.2f", concurrence), 10, 35, 20, MAROON);

    if (last_q0 >= 0)
    {
        DrawText(TextFormat("Last measurement: q0=|%d>  q1=|%d>", last_q0, last_q1), 10, 60, 18, MAROON);
    }

    DrawText("Q0: h x y z    Q1: H X Y Z    C = CNOT(0->1)    R = reset    M = measure", 10, 570, 16, GRAY);
}

Vector3 bloch_slerp(Vector3 from, Vector3 to, float t)
{
    float fromLen = Vector3Length(from);
    float toLen = Vector3Length(to);
    float lerpedLen = fromLen + (toLen - fromLen) * t;

    if (fromLen < 0.001f || toLen < 0.001f)
    {
        // one end is at/near center — direction is undefined, just lerp linearly
        Vector3 result = Vector3Lerp(from, to, t);
        return result;
    }

    Vector3 fromDir = Vector3Scale(from, 1.0f / fromLen);
    Vector3 toDir = Vector3Scale(to, 1.0f / toLen);

    float dot = Vector3DotProduct(fromDir, toDir);
    if (dot > 0.9995f)
    {
        Vector3 dir = Vector3Normalize(Vector3Lerp(fromDir, toDir, t));
        return Vector3Scale(dir, lerpedLen);
    }
    if (dot < -1.0f)
        dot = -1.0f;
    if (dot > 1.0f)
        dot = 1.0f;

    float theta = acosf(dot) * t;
    Vector3 relative = Vector3Normalize(Vector3Subtract(toDir, Vector3Scale(fromDir, dot)));

    Vector3 a = Vector3Scale(fromDir, cosf(theta));
    Vector3 b = Vector3Scale(relative, sinf(theta));
    Vector3 dir = Vector3Add(a, b);

    return Vector3Scale(dir, lerpedLen);
}