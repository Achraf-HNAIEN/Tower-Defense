#include "graph.h"
#include "game.h"


void draw_cell(int x, int y, MLV_Color color) {
    MLV_draw_filled_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
void draw_grid_with_path(int grid[HEIGHT][WIDTH], Point *path, int pathSize) {
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

void draw_start_and_finish(Point start, Point finish) {
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

void drawMonsters(Monster monsters[], int count) {
    int monsterSize = CELL_SIZE / 4; 

    for (int i = 0; i < count; i++) {
        if (monsters[i].hp <= 0) continue;

        int pixelX = monsters[i].x * CELL_SIZE + CELL_SIZE / 2;
        int pixelY = monsters[i].y * CELL_SIZE + CELL_SIZE / 2;

        MLV_Color monsterColor = hueToRGB(monsters[i].tint);
        MLV_draw_filled_circle(pixelX, pixelY, monsterSize, monsterColor);
    }
}

void drawAll(Game * game, Monster * Monsters, int count){
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_grid_with_path(game->grid, game->path, game->pathSize);
    drawMonsters(Monsters, count);
    draw_start_and_finish(game->path[0], game->path[game->pathSize - 1]);
    draw_side_information(game);
    MLV_actualise_window();
}

void draw_side_information(Game * game){
    MLV_draw_text_box(WIDTH * CELL_SIZE + 25, 20, 150, 50,
    "Wave number : %d", 1, MLV_COLOR_RED, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 25, 90, 150, 50,
    "Mana : %d/%d", 1, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->mana, game->mana_max);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 5, 160, 92, 50,
    "Buy tower:\n %d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave); // a changer mettre cout toour

    MLV_draw_text_box(WIDTH * CELL_SIZE + 102, 160, 93, 50,
    "Buy gemme:\n %d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave); // a changer mettre cout gemme
    int sizeFusion;
    MLV_get_size_of_text("Fusion :",&sizeFusion, NULL);
    MLV_draw_text(WIDTH * CELL_SIZE + 100 - sizeFusion/2, 220, "Fusion :", MLV_COLOR_PURPLE2);

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 35,
           245,
           50,50,MLV_COLOR_PURPLE1 );

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 115,
           245,
            50,50,MLV_COLOR_PURPLE1 );

    /*MLV_draw_circle(WIDTH * CELL_SIZE + 60,
            270, 27, MLV_COLOR_PURPLE1);

    MLV_draw_circle(WIDTH * CELL_SIZE + 140,
            270, 27, MLV_COLOR_PURPLE1);*/

    MLV_draw_text(WIDTH * CELL_SIZE + 95, 260, "+", MLV_COLOR_PURPLE2);

    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            MLV_draw_rectangle(WIDTH * CELL_SIZE + 25 + (i*2) * 50,
            315 + (j*80),
            50,50,MLV_COLOR_LIGHTGRAY );
        }
    }
    

}

void drawAll(Game * game, Monster * Monsters, int count){
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_grid_with_path(game->grid, game->path, game->pathSize);
    drawMonsters(Monsters, count);
    draw_start_and_finish(game->path[0], game->path[game->pathSize - 1]);
    draw_side_information(game);
    MLV_actualise_window();
}

void draw_side_information(Game * game){
    MLV_draw_text_box(WIDTH * CELL_SIZE + 25, 20, 150, 50,
    "Wave number : %d", 1, MLV_COLOR_RED, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 25, 90, 150, 50,
    "Mana : %d/%d", 1, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->mana, game->mana_max);

    MLV_draw_text_box(WIDTH * CELL_SIZE + 5, 160, 92, 50,
    "Buy tower:\n %d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave); // a changer mettre cout toour

    MLV_draw_text_box(WIDTH * CELL_SIZE + 102, 160, 93, 50,
    "Buy gemme:\n %d mana", 1, MLV_COLOR_GREEN, MLV_COLOR_WHITE,
    MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,
    MLV_VERTICAL_CENTER, game->wave); // a changer mettre cout gemme
    int sizeFusion;
    MLV_get_size_of_text("Fusion :",&sizeFusion, NULL);
    MLV_draw_text(WIDTH * CELL_SIZE + 100 - sizeFusion/2, 220, "Fusion :", MLV_COLOR_PURPLE2);

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 35,
           245,
           50,50,MLV_COLOR_PURPLE1 );

    MLV_draw_rectangle(WIDTH * CELL_SIZE + 115,
           245,
            50,50,MLV_COLOR_PURPLE1 );

    /*MLV_draw_circle(WIDTH * CELL_SIZE + 60,
            270, 27, MLV_COLOR_PURPLE1);

    MLV_draw_circle(WIDTH * CELL_SIZE + 140,
            270, 27, MLV_COLOR_PURPLE1);*/

    MLV_draw_text(WIDTH * CELL_SIZE + 95, 260, "+", MLV_COLOR_PURPLE2);

    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            MLV_draw_rectangle(WIDTH * CELL_SIZE + 25 + (i*2) * 50,
            315 + (j*80),
            50,50,MLV_COLOR_LIGHTGRAY );
        }
    }
    

}