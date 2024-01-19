#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "game.h"
int moveMonsters(Monster monsters[], Point path[], int pathSize, float deltaTime, Game *game)
{
    if (!monsters)
        return 0;
    int count = (monsters[0].type == BOSS) ? 2 : (monsters[0].type == CROWD) ? 24
                                                                             : 12;

    for (int i = 0; i < count; i++)
    {
        if (monsters[i].hp <= 0){
            continue;
    }
        if (monsters[i].pathIndex < 0)
        {
            monsters[i].pathIndex += deltaTime;
            if (monsters[i].pathIndex < 0)
                continue;
            monsters[i].pathIndex = 0;
        }

        float fluctuation = 0.9 + (rand() % 21) / 100.0; // Random between 0.9 and 1.1
        float adjustedSpeed = monsters[i].speed * fluctuation;

        float distanceToMove = adjustedSpeed * deltaTime;

        while (distanceToMove > 0 && monsters[i].pathIndex < pathSize - 1)
        {
            float dx = path[monsters[i].pathIndex + 1].x - monsters[i].x;
            float dy = path[monsters[i].pathIndex + 1].y - monsters[i].y;
            float segmentLength = sqrt(dx * dx + dy * dy);

            if (segmentLength == 0)
            {
                monsters[i].pathIndex++;
                continue;
            }

            dx /= segmentLength;
            dy /= segmentLength;

            float step = fmin(distanceToMove, segmentLength);

            monsters[i].x += dx * step;
            monsters[i].y += dy * step;
            distanceToMove -= step;

            if (step == segmentLength){
                monsters[i].pathIndex++;
            }
        }

        if (monsters[i].pathIndex >= pathSize - 1){
            // Respawn at the start of the path
            monsters[i].x = path[0].x;
            monsters[i].y = path[0].y;
            monsters[i].pathIndex = 0;

            // Deduct mana as penalty for letting monster reach the base
            game->mana -= (int)((0.15 * monsters->max_hp) * pow(1.3, game->level_mana));
            if (game->mana < 0)
            {
                game->quit = 1; // End the game if mana falls below zero
            }
        }
    }
    return count;
}

static int monster_all_dead(Wave * wave){
    for(int i = 0 ; i < wave->Nb_Monsters ; i++){
        if(wave->monsters[i].hp > 0) return 0; // if a monster is alive the wave is still alive
    }
    return 1;
}


void check_wave_dead(Game * game){
    if(NULL == game->wavesHead){
        return;
    }
    Wave * currentWave = game->wavesHead;
    Wave * previousWave = NULL;
    while (currentWave != NULL && !monster_all_dead(currentWave)) { 
            previousWave = currentWave;
            currentWave = currentWave->next;
    }    
    if (currentWave == game->wavesHead){
        game->wavesHead = currentWave->next;
    }else if (currentWave != NULL){
        previousWave->next = currentWave->next;
    }
    free(currentWave);
}

void placeTower(Game *game, Point gridPosition, Gemme *gemme)
{
    // Check if we can build a tower here and if the maximum number of towers hasn't been reached
    if (!CanBuildTower(game->grid, gridPosition) || game->nb_tower >= MAX_TOWERS)
    {
        return;
    }

    // Calculate the cost for the next tower
    int towerCost = game->nb_tower < 3 ? 0 : 100 * pow(2, (game->nb_tower + 1) - 4);

    // Check if we have enough mana to build the tower
    if (game->mana < towerCost)
    {
        return;
    }

    game->mana -= towerCost;
    game->nb_tower++;

    game->grid[gridPosition.y][gridPosition.x] = TOWER;

    Tower newTower;
    InitializeTower(&newTower, gridPosition);
    if (gemme)
    {
        PlaceGemmeInTower(&newTower, gemme);
    }
    game->towers[game->tower_count] = newTower;
    game->tower_count++;

}

void add_mana(Game *game, int mana)
{
    game->mana = game->mana + mana >= game->mana_max ? game->mana_max : game->mana + mana;
}

void buy_gemme(Game *game)
{

    if (game->inventory_size < 6 && 100 * pow(2, game->level_gemme_in_shop) <= game->mana)
    {
        game->mana -= 100 * pow(2, game->level_gemme_in_shop);
        createGem(game->level_gemme_in_shop);
        for(int i = 0 ; i < 6 ; i++){
            if(game->inventaire[i] == NULL ){
                game->inventaire[i] = createGem(game->level_gemme_in_shop);
                game->inventory_size++;
                break;
            }
                
        }
        
    }
}

void upgrade_mana_storage(Game *game)
{
    if (game->mana >= (int)(500 * pow(1.4, game->level_mana)))
    {
        game->mana -= (int)(500 * pow(1.4, game->level_mana));
        game->level_mana++;
        game->mana_max = (int)(2000 * pow(1.4, game->level_mana));
    }
}

Wave *initializeWave(int waveNumber, Point path[], int pathSize)
{
    // Allocate memory for a new Wave node
    Wave *newWave = (Wave *)malloc(sizeof(Wave));
    if (newWave == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new wave.\n");
        return NULL;
    }

    // Initialize the new Wave node
    newWave->monsters = initialize_Monster(waveNumber, path, pathSize);
    newWave->Nb_Monsters = (newWave->monsters[0].type == BOSS) ? COUNT_BOSS : (newWave->monsters[0].type == CROWD) ? COUNT_CROWD
                                                                          : (newWave->monsters[0].type == AGILE)   ? COUNT_AGILE
                                                                                                                   : COUNT_NORMAL;
    newWave->next = NULL;

    return newWave;
}

void handle_inventory_click(int m_x, int m_y, Game *game){
    if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315, 50, 50)){ // CASE INVENTAIRE 1
        if (game->inventaire[0] && game->gemme_selected != 0){
            game->gemme_selected = 0;
        }
        else if (game->inventaire[0] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[0] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[0] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315, 50, 50)){ // CASE INVENTAIRE 2
        if (game->inventaire[1] && game->gemme_selected != 1){
            game->gemme_selected = 1;
        }
        else if (game->inventaire[1] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[1] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[1] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315 + 80, 50, 50)){ // CASE INVENTAIRE 3
        if (game->inventaire[2] && game->gemme_selected != 2){
            game->gemme_selected = 2;
        }
        else if (game->inventaire[2] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[2] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[2] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315 + 80, 50, 50)){ // CASE INVENTAIRE 4
        if (game->inventaire[3] && game->gemme_selected != 3){
            game->gemme_selected = 3;
        }
        else if (game->inventaire[3] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[3] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[3] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315 + 160, 50, 50)){ // CASE INVENTAIRE 5
        if (game->inventaire[4] && game->gemme_selected != 4){
            game->gemme_selected = 4;
        }
        else if (game->inventaire[4] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[4] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[4] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315 + 160, 50, 50)){ // CASE INVENTAIRE 6
        if (game->inventaire[5]&& game->gemme_selected != 5){
            game->gemme_selected = 5;
        }
        else if (game->inventaire[5] == NULL){
            if( game->gemme_selected == 6 ){
                game->inventaire[5] = game->fusion_slot1;
                game->fusion_slot1 = NULL;
            } 
            if( game->gemme_selected == 7){
                game->inventaire[5] = game->fusion_slot2;
                game->fusion_slot2 = NULL;
            }
            game->gemme_selected = -1;
        }
    }else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 35, 245, 50, 50)){
        if(game->gemme_selected <= 5 && game->gemme_selected >=0){
            if(game->fusion_slot1){
                Gemme * gemme_temp = game->fusion_slot1;
                game->fusion_slot1 = game->inventaire[game->gemme_selected];
                game->inventaire[game->gemme_selected] = gemme_temp;
                game->gemme_selected = -1;
            }else{
                game->fusion_slot1 = game->inventaire[game->gemme_selected];
                game->inventaire[game->gemme_selected] = NULL;
                game->inventory_size--;
                game->gemme_selected = -1;
            }
        }else if(game->fusion_slot1){
            game->gemme_selected = 6;
        }else {
            game->gemme_selected = -1;
        }
    }else if(is_click_inside(m_x,m_y,WIDTH * CELL_SIZE + 115, 245, 50, 50) ){
        if(game->gemme_selected <= 5 && game->gemme_selected >=0){
            if(game->fusion_slot2){
                Gemme * gemme_temp = game->fusion_slot2;
                game->fusion_slot2 = game->inventaire[game->gemme_selected];
                game->inventaire[game->gemme_selected] = gemme_temp;
                game->gemme_selected = -1;
            }else{
                game->fusion_slot2 = game->inventaire[game->gemme_selected];
                game->inventaire[game->gemme_selected] = NULL;
                game->inventory_size--;
                game->gemme_selected = -1;
            }
        }else if(game->fusion_slot2){
            game->gemme_selected = 7;
        }else {
            game->gemme_selected = -1;
        }
    }
    
}

void try_fusion(Game *game) {
    // Check if there's enough mana for fusion
    if (game->mana >= 100) {
        Gemme *gemme_fusion = (Gemme *) malloc(sizeof(Gemme));
        if (!gemme_fusion) return; // Exit if memory allocation fails

        // Attempt to fuse the gems
        if (!fuseGems(game->fusion_slot1, game->fusion_slot2, gemme_fusion)) {
            game->mana -= 100; // Deduct mana cost for successful fusion

            // Find an empty slot in the inventory
            int slotFound = 0;
            for (int i = 0; i < 6; i++) {
                if (game->inventaire[i] == NULL) {
                    game->inventaire[i] = gemme_fusion;
                    game->inventory_size++;
                    slotFound = 1;
                    break;
                }
            }

            // Handle case where no slot is available
            if (!slotFound) {
                free(gemme_fusion);
                fprintf(stderr, "No space in inventory for fused gem.\n");
            }

            // Clean up fusion slots
            free(game->fusion_slot1);
            free(game->fusion_slot2);
            game->fusion_slot1 = NULL;
            game->fusion_slot2 = NULL;
        } else {
            // Fusion failed, free allocated memory
            free(gemme_fusion);
        }
    } else {
        fprintf(stderr, "Not enough mana to fuse gems.\n");
    }
}

static MLV_Color determineProjectileColor(Gemme *gemme) {
    if (gemme == NULL) {
        return MLV_COLOR_WHITE; // Default color in case of NULL pointer
    }

    switch (gemme->elementType) {
        case PYRO:
            return MLV_COLOR_RED;
        case DENDRO:
            return MLV_COLOR_GREEN;
        case HYDRO:
            return MLV_COLOR_BLUE;
        default:
            return MLV_COLOR_WHITE; // Default color for other types
    }
}

void addVisualProjectile(Game *game, VisualProjectile vp) {
    if (game == NULL || game->numVisualProjectiles >= MAX_PROJECTILES) {
        return;
    }

    game->visualProjectiles[game->numVisualProjectiles++] = vp;
}

void shootAtMonster(Game *game, Monster *targetMonster, Tower *tower) {
    float damage = calculateGemDamage(tower->gemme, targetMonster);

    applay_Damage(targetMonster, damage);
    applyElementalEffect(tower->gemme, targetMonster);

    VisualProjectile vp = {
        .start_x = tower->position.x,
        .start_y = tower->position.y,
        .end_x = targetMonster->x,
        .end_y = targetMonster->y,
        .targetMonster = targetMonster,
        .color = determineProjectileColor(tower->gemme),
        .active = 1
    };
    addVisualProjectile(game, vp);
}


static Monster* findStrongestMonsterWithinRange(Game *game, Point towerPos, float range) {
    Monster* targetMonster = NULL;
    float maxHP = 0;

    Wave *currentWave = game->wavesHead;
    while (currentWave != NULL) {
        for (int i = 0; i < currentWave->Nb_Monsters; i++) {
            Monster *monster = &currentWave->monsters[i];
            float dist = distanceBetweenPoints(towerPos, (Point){monster->x, monster->y});

            if (dist <= range && monster->hp > maxHP) {
                maxHP = monster->hp;
                targetMonster = monster;
            }
        }
        if (targetMonster != NULL) {
        }
        currentWave = currentWave->next;
    }

    return targetMonster;
}


void UpdateGemmesAndShoot(Game *game, float deltaTime)
{


    for (int i = 0; i < game->tower_count; i++)
    {
        Tower *tower = &game->towers[i];
        if (tower->gemme != NULL)
        {
            if (tower->gemme->cooldown >= 0)
            {
                tower->gemme->cooldown -= deltaTime;

                if (tower->gemme->cooldown <= 0)
                {
                    tower->gemme->isReadyToShoot = 1;
                    tower->gemme->cooldown = 0.5;
                }
            }
            if (tower->gemme->isReadyToShoot)
            {
                Monster *targetMonster = findStrongestMonsterWithinRange(game, tower->position, 3);
                if (targetMonster != NULL)
                {
                    shootAtMonster(game, targetMonster, tower);
                    tower->gemme->isReadyToShoot = 0;
                    tower->gemme->cooldown = 0.5; 
                }
                else
                {
                }
            }
        }
    }
}



Tower * find_Tower_Pos(Game * game, Point position){
    if (!game) return NULL;
    for(int i = 0;i < game->tower_count;i++){
        if(game->towers[i].position.x == position.x && game->towers[i].position.y == position.y){
            return &game->towers[i];// tower was found 
        }
    }
    return NULL; // no Tower in that pos 
}

void placeGemmeOnTower(Game *game, Point towerPosition) {
    if (game->gemme_selected != -1) {
        Tower *tower = find_Tower_Pos(game, towerPosition);
        if (tower != NULL) {
            Gemme *selectedGemme = game->inventaire[game->gemme_selected];
            PlaceGemmeInTower(tower, selectedGemme);
            game->gemme_selected = -1;
        }
    }
}

void try_place_gemme_on_tower(Game *game, int mouse_x, int mouse_y)
{
    if (game == NULL)
    {
        fprintf(stderr, "Error ----> Game is NULL in Try place gemme on tower function\n");
        return;
    }

    Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
    Tower *tower = find_Tower_Pos(game, gridPosition);
    if (game->gemme_selected >= 0 && game->gemme_selected <= 5 && tower != NULL)
    {
        Gemme *selectedGemme = game->inventaire[game->gemme_selected];
        if (tower->gemme == NULL){
            if(PlaceGemmeInTower(tower, selectedGemme)){
                if(game->gemme_selected <= 5){
                    game->inventaire[game->gemme_selected] = NULL;
                }
                game->gemme_selected = -1;
                game->inventory_size--;
            }
        }
    }
}
void try_remove_gemme_on_tower(Game * game, int mouse_x,int mouse_y){
    Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
    Tower *tower;
    if((tower = find_Tower_Pos(game, gridPosition))){
        if(game->inventory_size <= 5){
            for(int i = 0 ; i < 6 ; i++){
                if(game->inventaire[i] == NULL){
                    game->inventaire[i] = tower->gemme;
                    tower->gemme = NULL;
                    return;
                }
            }
            
        }
        
    }
}
