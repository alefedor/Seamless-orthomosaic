#include "visual/visualizer.h"

// mi == -1 means "don't change"
static void mark(Image &a, int x, int y, int m1, int m2, int m3) {
    Pixel *pixel = a.getPixel(x, y);
    if (m1 != -1)
        pixel[0] = m1;
    if (m2 != -1)
        pixel[1] = m2;
    if (m3 != -1)
        pixel[2] = m3;
}


void Visualizer::showSeam(Image &a, Seam &s) {
    for (const Edge &e : s.edges) {
        for (int dy = -5; dy <= 5; dy++)
            for (int dx = -5; dx <= 5; dx++)
                if (a.inside(e.x + dx, e.y + dy))
                    mark(a, e.x + dx, e.y + dy, 255, 0, 0);
    }
}

void Visualizer::markImage(Image &a, int id) {
    int bottom = a.top + a.height;
    int right = a.left + a.width;
    int m1 = -1, m2 = -1, m3 = -1;
    if (id % 3 == 0) {
        m1 = ((id / 3 + 1) * 100) % 256;
    } else if (id % 3 == 1) {
        m2 = ((id / 3 + 1) * 100) % 256;
    } else {
        m3 = ((id / 3 + 1) * 100) % 256;
    }
    for (int y = a.top; y < bottom; y++)
        for (int x = a.left; x < right; x++)
            if (a.inside(x, y))
                mark(a, x, y, m1, m2, m3);
}
