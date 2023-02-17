#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HIEGHT 1080
#define WIDTH 1920

#define COLOR_RED 0xFF0000FF
#define COLOR_BLUE 0xFFFF0000
#define COLOR_GREEN 0XFF00FF00

#define OUTPUT_FILE_PATH "voronoi.ppm"
#define SEED_COUNT 10
#define SEED_RADIUS 6

typedef uint32_t Color32;
typedef uint8_t Color8;
typedef struct {
    int x;
    int y;
} Point;

static Color32 image[HIEGHT][WIDTH];
static Point seeds[SEED_COUNT];

void fill_image(Color32 color) {
    for (size_t i = 0; i < HIEGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            image[i][j] = color;
        }
    }
}

void save_image_as_ppm(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Cannot write to %s", filename);
        exit(1);
    }
    fprintf(f, "P6\n%d %d 255\n", WIDTH, HIEGHT);
    for (size_t i = 0; i < HIEGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            Color32 pixel = image[i][j];
            Color8 bytes[3] = {
                (pixel & 0x0000FF) >> 0,
                (pixel & 0x00FF00) >> 8,
                (pixel & 0xFF0000) >> 16,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            assert(!ferror(f));
        }
    }
    int closed = fclose(f);
    assert(closed == 0);
}

void generate_random_seed() {
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        seeds[i].x = rand() % WIDTH;
        seeds[i].y = rand() % HIEGHT;
    }
}

void fill_circle(Point p, int radius, Color32 color) {
    int x0 = p.x - radius;
    int y0 = p.y - radius;
    int x1 = p.x + radius;
    int y1 = p.y + radius;
    for (int y = y0; y < y1; ++y) {
        if ((y <= 0) || y >= HIEGHT) {
            continue;
        }
        for (int x = x0; x < x1; ++x) {
            if ((x <= 0) || x >= WIDTH) {
                continue;
            }
            int dx = p.x - x;
            int dy = p.y - y;
            if (dx * dx + dy * dy < radius * radius) {
                image[y][x] = color;
            }
        }
    }
}

void fill_seeds(void) {
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        fill_circle(seeds[i], SEED_RADIUS, COLOR_GREEN);
    }
}

int main() {
    generate_random_seed();
    fill_image(COLOR_RED | COLOR_BLUE);
    fill_seeds();
    save_image_as_ppm(OUTPUT_FILE_PATH);
    return 0;
}
