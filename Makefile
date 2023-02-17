.PHONY: run
run: ./src/main.c
	clear && cc -Wall -Wextra -o voronoi ./src/main.c && ./voronoi && eog voronoi.ppm
