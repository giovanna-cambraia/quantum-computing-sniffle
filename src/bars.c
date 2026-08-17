#include "bars.h"
#include <stdio.h>

static const char *BASIS_LABELS[4] = { "|00>", "|01>", "|10>", "|11>" };

void render_amplitude_bars(const TwoQubit *s, int x, int y, int width, int height) {
    int barCount = 4;
    int gap = 12;
    int barWidth = (width - gap * (barCount - 1)) / barCount;

    DrawText("Amplitude interference", x, y - 34, 18, DARKGRAY);

    for (int i = 0; i < barCount; i++) {
        double prob = twoqubit_prob(s, i);
        double re = creal(s->amp[i]);

        int barHeight = (int)(prob * height);
        if (barHeight < 1) barHeight = 1;

        int barX = x + i * (barWidth + gap);
        int barY = y + height - barHeight;

        Color barColor = (re >= 0) ? SKYBLUE : ORANGE;

        DrawRectangle(barX, barY, barWidth, barHeight, barColor);
        DrawRectangleLines(barX, y, barWidth, height, LIGHTGRAY);

        DrawText(BASIS_LABELS[i], barX, y + height + 6, 14, DARKGRAY);

        char probText[16];
        snprintf(probText, sizeof(probText), "%.2f", prob);
        DrawText(probText, barX, barY - 18, 14, DARKGRAY);
    }

    DrawText("blue = positive amplitude   orange = negative amplitude",
        x, y + height + 26, 14, GRAY);
}