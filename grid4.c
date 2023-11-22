#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_WIDTH 28
#define GRID_HEIGHT 22
#define CELL_SIZE 40
#define MAX(a,b) (((a)>(b))?(a):(b))

// Function prototypes
void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]);
int is_valid_move(int x, int y, int grid[GRID_WIDTH][GRID_HEIGHT]);
int calculate_extent(int x, int y, int dx, int dy, int grid[GRID_WIDTH][GRID_HEIGHT]);
int choose_initial_direction(int extents[4]);
int draw_segment(int grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int direction);
void update_direction(int* direction, int left_extent, int right_extent);


// Function to initialize the grid to empty
void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j] = 0;  // 0 represents an empty cell
        }
    }
}

// Function to check if a move is valid
int is_valid_move(int x, int y, int grid[GRID_WIDTH][GRID_HEIGHT]) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT && grid[x][y] == 0;
}

// Function to calculate the extent in a given direction
int calculate_extent(int x, int y, int dx, int dy, int grid[GRID_WIDTH][GRID_HEIGHT]) {
    int extent = 0;
    while (is_valid_move(x + dx, y + dy, grid)) {
        extent++;
        x += dx;
        y += dy;
    }
    return extent;
}

void draw_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    MLV_clear_window(MLV_COLOR_GRAY);

    // Dessiner les lignes verticales
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        MLV_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, MLV_COLOR_BLACK);
    }

    // Dessiner les lignes horizontales
    for (int j = 0; j <= GRID_HEIGHT; ++j) {
        MLV_draw_line(0, j * CELL_SIZE, GRID_WIDTH * CELL_SIZE, j * CELL_SIZE, MLV_COLOR_BLACK);
    }

    // Remplir les cellules
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (grid[i][j] == 1) {
                MLV_draw_filled_rectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, MLV_COLOR_WHITE);
            }
        }
    }

    MLV_actualise_window();
}


// Function to choose the initial direction based on extents
int choose_initial_direction(int extents[4]) {
    int total_extent = 0;
    for (int i = 0; i < 4; ++i) {
        total_extent += extents[i];
    }

    int direction = -1;  // -1 means no direction
    int random = rand() % total_extent;
    int sum = 0;
    for (int i = 0; i < 4; ++i) {
        sum += extents[i];
        if (random < sum) {
            direction = i;
            break;
        }
    }

    return direction;
}

// Function to draw a random path segment in the current direction
int draw_segment(int grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int direction) {
    int s = 0;
    int extent = calculate_extent(x, y, (direction == 1) - (direction == 3), (direction == 0) - (direction == 2), grid);

    for (int i = 0; i < extent; ++i) {
        if (rand() % 4 < 3) {
            s++;
        }
    }

    int segment = MAX(s, 3);

    for (int i = 0; i < segment; ++i) {
        grid[x][y] = 1;
        x += (direction == 1) - (direction == 3);
        y += (direction == 0) - (direction == 2);
    }

    return segment;
}

// Function to update the direction based on left and right extents
void update_direction(int* direction, int left_extent, int right_extent) {
    int total_extent = left_extent + right_extent;
    int random = rand() % total_extent;

    if (random < left_extent) {
        *direction = (*direction + 3) % 4;
    } else {
        *direction = (*direction + 1) % 4;
    }
}
// Function to generate a random path
void generate_path(int grid[GRID_WIDTH][GRID_HEIGHT], int length) {
    int startX, startY;
    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};  // North, East, South, West

    // Étape 1: Initialiser la grille à vide
    initialize_grid(grid);

    // Étape 2: Choisir une case aléatoire avec une distance d'au moins 3 des bords
    do {
        startX = rand() % (GRID_WIDTH - 6) + 3;
        startY = rand() % (GRID_HEIGHT - 6) + 3;
    } while (!is_valid_move(startX, startY, grid));

    int x = startX;
    int y = startY;

    int last_direction = -1;  // -1 signifie aucune direction

    // Boucle principale pour étendre le chemin
    for (int i = 0; i < length; ++i) {
        grid[x][y] = 1;  // Marquer la case actuelle comme faisant partie du chemin

        // Étape 3: Choisir aléatoirement la direction initiale avec une probabilité proportionnelle à l'étendue
        int direction = choose_direction(x, y, grid, last_direction);

// Étape 4: Si l'étendue de la direction courante est inférieure ou égale à 2, passer à l'étape 7
if (calculate_extent(x, y, directions[direction][0], directions[direction][1], grid) <= 2) {
    break;
}


        // Étape 5: Tirer n valeurs aléatoires et calculer le nombre de cases à ajouter au chemin
        int segment = draw_segment(grid, x, y, direction);

        // Étape 6: Avancer jusqu'à la case au bout du nouveau segment de chemin
        move_along_direction(&x, &y, directions[direction], segment);

        // Calculer les étendus dans les deux directions perpendiculaires
int left_extent = calculate_extent(x, y, directions[(direction + 3) % 4][0], directions[(direction + 3) % 4][1], grid);
int right_extent = calculate_extent(x, y, directions[(direction + 1) % 4][0], directions[(direction + 1) % 4][1], grid);


        // Choisir la nouvelle direction avec une probabilité proportionnelle aux étendus
        last_direction = choose_new_direction(left_extent, right_extent);
    }

    // Étape 7: Vérifier si le chemin obtenu a fait au moins 7 virages et est de longueur au moins 75
    // Si non, recommencer à l'étape 1
    int turns = count_turns(grid);
    int path_length = count_path_length(grid);

    if (turns < 7 || path_length < 75) {
        generate_path(grid, length);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int grid[GRID_WIDTH][GRID_HEIGHT];
    
    MLV_create_window("Random Path Generation", "Random Path", GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);

    generate_path(grid, 75);

    draw_grid(grid);

    MLV_actualise_window();
    MLV_wait_seconds(5);
    MLV_free_window();

    return 0;
}
