#include "conversion.hpp"

ptg_color text_to_color(const char* text) {
    unsigned char values[3] = {0};
    for (int i = 0; *text != '\0'; ++text) {
        if (*text == ':') {
            ++i;
            continue;
        }

        if (*text < '0' || *text > '9')
            break;

        values[i] *= 10;
        values[i] += *text - '0';
    }

    ptg_color color;
    color.r = values[0];
    color.g = values[1];
    color.b = values[2];

    return color;
}
