#include "graph.h"


#include <math.h>



static void draw_cell(int x, int y, MLV_Color color) {
    MLV_draw_filled_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
static void draw_grid_with_path(int grid[HEIGHT][WIDTH], Point *path, int pathSize) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            draw_cell(j, i, MLV_COLOR_GRAY); 
        }
    }

    for (int i = 0; i < pathSize; i++) {
        draw_cell(path[i].x, path[i].y, MLV_COLOR_WHITE); 
    }

    for (int i = 0; i <= HEIGHT; i++) {
        MLV_draw_line(0, i * CELL_SIZE, WIDTH * CELL_SIZE, i * CELL_SIZE, MLV_COLOR_BLACK);
    }
    for (int j = 0; j <= WIDTH; j++) {
        MLV_draw_line(j * CELL_SIZE, 0, j * CELL_SIZE, HEIGHT * CELL_SIZE, MLV_COLOR_BLACK);
    }
}

static void draw_start_and_finish(Point start, Point finish) {
    draw_cell(start.x, start.y, MLV_COLOR_GREEN);   
    draw_cell(finish.x, finish.y, MLV_COLOR_RED);
}

MLV_Color hueToRGB(int hue) {
    float r = 0.0, g = 0.0, b = 0.0;

    int i = hue / 60;
    float f = (float)(hue % 60) / 60.0;
    float q = 1 - f;

    switch(i) {
        case 0: r = 1; g = f; break;
        case 1: r = q; g = 1; break;
        case 2: g = 1; b = f; break;
        case 3: g = q; b = 1; break;
        case 4: r = f; b = 1; break;
        case 5: r = 1; b = q; break;
    }

    return MLV_convert_rgba_to_color((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
}

static void drawMonsters(Monster monsters[], int count) {
    int monsterSize = CELL_SIZE / 4; 

    for (int i = 0; i < count; i++) {
        if (monsters[i].hp <= 0) continue;

        int pixelX = monsters[i].x * CELL_SIZE + CELL_SIZE / 2;
        int pixelY = monsters[i].y * CELL_SIZE + CELL_SIZE / 2;

        MLV_Color monsterColor = hueToRGB(monsters[i].tint);
        MLV_draw_filled_circle(pixelX, pixelY, monsterSize, monsterColor);
    }
}

static void draw_wave_numberMana(Game * game){
    MLV_draw_text_box(WIDTH * CELL_SIZE + 15, 20, 170, 50,
    "Wave number : %d", 1, MLV_COLOR_RED, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave);

    double cost_mana = 500*pow(1.4,game->level_mana);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 15, 80, 170, 50,
    "Mana : %d/%d\n Level up for %d mana", 1, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->mana, game->mana_max,(int) cost_mana);
}

static void draw_shop(Game * game){
    int tower_cost = game->nb_tower < 3? 0 : 100 * pow(2,(game->nb_tower+1)-4);
    int sizeShop;
    int gemme_cost = 100* pow(2,game->level_gemme_in_shop);
    MLV_get_size_of_text("Shop :",&sizeShop, NULL);
    MLV_draw_text(WIDTH * CELL_SIZE + 100 - sizeShop/2, 140, "Shop :", MLV_COLOR_GREEN);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 5, 162, 92, 50,
    "Tower:\n %d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    game->want_to_place_tower ? MLV_COLOR_LIGHT_GREEN : MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, tower_cost); 


    MLV_draw_text_box(WIDTH * CELL_SIZE + 105, 162, 93, 50,
    "Gemme\n(level %d)\n%d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER,game->level_gemme_in_shop, gemme_cost);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 181, 146,17,17, "+",1,MLV_COLOR_GREEN1,MLV_COLOR_RED,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    MLV_draw_text_box(WIDTH * CELL_SIZE + 181, 211,17,17, " -",1,MLV_COLOR_GREEN1,MLV_COLOR_RED,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    
}

static void draw_fusion_and_inventory(Game * game){
    int sizeFusion;
    MLV_get_size_of_text("Fusion :",&sizeFusion, NULL);
    MLV_draw_text(WIDTH * CELL_SIZE + 100 - sizeFusion/2, 220, "Fusion :", MLV_COLOR_PURPLE2);

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 35,
           245,
           50,50,MLV_COLOR_PURPLE1 );

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 115,
           245,
            50,50,MLV_COLOR_PURPLE1 );

     MLV_draw_text(WIDTH * CELL_SIZE + 95, 260, "+", MLV_COLOR_PURPLE2);

    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            MLV_draw_rectangle(WIDTH * CELL_SIZE + 25 + (i*2) * 50,
            315 + (j*80),
            50,50,MLV_COLOR_LIGHTGRAY );
        }
    }
}
static void drawMonsterHealthBar(Monster *monster, int pathsize) {
    if (monster == NULL || monster->pathIndex <=0 || monster->pathIndex >= pathsize - 1) {
        return;
    }

    int pixelX = monster->x * CELL_SIZE;
    int pixelY = monster->y * CELL_SIZE;

    float healthRatio = monster->hp / monster->max_hp;

    MLV_Color healthColor = MLV_COLOR_GREEN;
    if (healthRatio < 0.5) {
        healthColor = MLV_COLOR_YELLOW;
    }
    if (healthRatio < 0.25) {
        healthColor = MLV_COLOR_RED;
    }

    MLV_draw_filled_rectangle(pixelX, pixelY - HEALTH_BAR_HEIGHT - 2, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, MLV_COLOR_GREY);
    MLV_draw_filled_rectangle(pixelX, pixelY - HEALTH_BAR_HEIGHT - 2, (int)(HEALTH_BAR_WIDTH * healthRatio), HEALTH_BAR_HEIGHT, healthColor);
}


static void draw_side_information(Game * game){
    
    draw_wave_numberMana(game);
    draw_shop(game);
    draw_fusion_and_inventory(game);
    return ;
}

int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height) {
    return mouse_x >= x && mouse_x < x + width && mouse_y >= y && mouse_y < y + height;
}

void drawTower(const Tower *tower) {
    if (tower == NULL) {
        fprintf(stderr, "Error: Null tower pointer in drawTower function\n");
        return;
    }
    
    int centerX = tower->position.x * CELL_SIZE + CELL_SIZE / 2;
    int centerY = tower->position.y * CELL_SIZE + CELL_SIZE / 2;
    int towerSize = CELL_SIZE / 2; 

    MLV_Color towerColor = MLV_COLOR_BLACK;
    if (tower->gemme != NULL) {
        switch (tower->gemme->elementType) {
            case PYRO:
                towerColor = MLV_COLOR_RED;
                break;
            case DENDRO:
                towerColor = MLV_COLOR_GREEN;
                break;
            case HYDRO:
                towerColor = MLV_COLOR_BLUE;
                break;
            default:
                towerColor = MLV_COLOR_GREY;
        }
    }
    
    
    MLV_draw_filled_circle(centerX, centerY, towerSize, towerColor);
    
    
    MLV_draw_circle(centerX, centerY, towerSize, MLV_COLOR_WHITE);
    //printf("Drawing tower at: x=%d, y=%d, size=%d\n", centerX, centerY, towerSize);

}

void drawAll(Game * game, Monster * Monsters, int count){
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_side_information(game);

    draw_grid_with_path(game->grid, game->path, game->pathSize);
    drawMonsters(Monsters, count);
    draw_start_and_finish(game->path[0], game->path[game->pathSize - 1]);
    for (int i = 0; i < game->tower_count; i++) {
        drawTower(&(game->towers[i]));
    }
    
    for (int i = 0; i < count; i++) {
        drawMonsterHealthBar(&Monsters[i], game->pathSize);
        // printf("%d\n",Monsters[i].pathIndex);
    }
    MLV_actualise_window();
}
