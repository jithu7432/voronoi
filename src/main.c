#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HIEGHT 1080
#define WIDTH 1920
#define COLOR_RED 0xFF0000FF
#define COLOR_BLUE 0xFFFF0000
#define COLOR_GREEN 0XFF00FF00
#define COLOR_BLACK 0XFF000000
#define COLOR_WHITE 0XFFFFFFFF

#define OUTPUT_FILE_PATH "voronoi.ppm"
#define SEED_COUNT 500
#define SEED_RADIUS 3 

#define COLOR_1 0xFF00000
#define COLOR_2 0xFF14205
#define COLOR_3 0xFF0F153
#define COLOR_4 0xFF432A4
#define COLOR_5 0xFFBC0F8
#define COLOR_6 0xFFeeeeee
#define COLOR_7 0xFF6767d7
#define COLOR_8 0xFF4b211a
#define COLOR_9 0xFF727272
#define COLOR_10 0xFFd581cb
#define COLOR_11 0xFF2A22DB
#define COLOR_12 0xFF9B8D09
#define COLOR_13 0xFF5E3C05
#define COLOR_14 0xFF5510BC
#define COLOR_15 0xFF0C60FA

typedef uint32_t Color32;
typedef uint8_t Color8;
typedef struct {
    int x;
    int y;
} Point;

static Point seeds[SEED_COUNT];
static Color32 image[HIEGHT][WIDTH];
static Color32 PALETTE[] = {
    // COLOR_1, COLOR_2,  COLOR_3, COLOR_4, COLOR_5,
    // COLOR_6, COLOR_10, COLOR_8, COLOR_9, COLOR_7,
    COLOR_11, COLOR_12, COLOR_13, COLOR_14, COLOR_15,
};

#define PALETTE_SIZE sizeof(PALETTE) / sizeof(PALETTE[0])

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
    srand(time(NULL));
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        seeds[i].x = rand() % WIDTH;
        seeds[i].y = rand() % HIEGHT;
    }
}

void fill_circle(Point center, int radius, Color32 color) {
    int x0 = center.x - radius;
    int y0 = center.y - radius;
    int x1 = center.x + radius;
    int y1 = center.y + radius;
    for (int y = y0; y < y1; ++y) {
        if ((y < 0) || y >= HIEGHT) {
            continue;
        }
        for (int x = x0; x < x1; ++x) {
            if ((x < 0) || x >= WIDTH) {
                continue;
            }
            int dx = center.x - x;
            int dy = center.y - y;
            if (dx * dx + dy * dy < radius * radius) {
                image[y][x] = color;
            }
        }
    }
}

void fill_seeds(void) {
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        fill_circle(seeds[i], SEED_RADIUS, COLOR_BLACK);
    }
}

int calc_dist(int x0, int y0, int x1, int y1) {
    return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
}

void compute_voronoi() {
    int j;
    for (int y = 0; y < HIEGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            j = 0;
            for (int i = 1; i < SEED_COUNT; ++i) {
                if ((calc_dist(x, y, seeds[i].x, seeds[i].y)) <
                    (calc_dist(x, y, seeds[j].x, seeds[j].y)))
                    j = i;
            }
            image[y][x] = PALETTE[j % PALETTE_SIZE];
        }
    }
}

int main() {
    generate_random_seed();
    fill_image(COLOR_RED | COLOR_GREEN);
    compute_voronoi();
    fill_seeds();
    save_image_as_ppm(OUTPUT_FILE_PATH);
    return 0;
}
