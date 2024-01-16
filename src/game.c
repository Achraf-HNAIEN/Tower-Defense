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
        if (monsters[i].hp <= 0)
            continue;

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

            if (step == segmentLength)
            {
                monsters[i].pathIndex++;
            }
        }

        if (monsters[i].pathIndex >= pathSize - 1)
        {
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

void placeTower(Game *game, Point gridPosition, Gemme *gemme)
{
    // Check if we can build a tower here and if the maximum number of towers hasn't been reached
    if (!CanBuildTower(game->grid, gridPosition) || game->nb_tower >= MAX_TOWERS)
    {
        printf("Cannot build tower here or maximum number of towers reached.\n");
        return;
    }

    // Calculate the cost for the next tower
    int towerCost = game->nb_tower < 3 ? 0 : 100 * pow(2, (game->nb_tower + 1) - 4);

    // Check if we have enough mana to build the tower
    if (game->mana < towerCost)
    {
        printf("Not enough mana to build a tower.\n");
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

    // Log the placement for debugging
    printf("Tower placed at (%d, %d).\n", gridPosition.x, gridPosition.y);
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
        game->inventaire[game->inventory_size++] = createGem(game->level_gemme_in_shop);
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

void handle_inventory_click(int m_x, int m_y, Game *game)
{
    if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315, 50, 50))
    { // CASE INVENTAIRE 1
        if (game->inventory_size - 1 >= 0 && game->gemme_selected != 0)
        {
            game->gemme_selected = 0;
        }
        else
        {
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315, 50, 50))
    { // CASE INVENTAIRE 2
        if (game->inventory_size - 1 >= 1 && game->gemme_selected != 1)
        {
            game->gemme_selected = 1;
        }
        else
        {
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315 + 80, 50, 50))
    { // CASE INVENTAIRE 3
        if (game->inventory_size - 1 >= 2 && game->gemme_selected != 2)
        {
            game->gemme_selected = 2;
        }
        else
        {
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315 + 80, 50, 50))
    { // CASE INVENTAIRE 4
        if (game->inventory_size - 1 >= 3 && game->gemme_selected != 3)
        {
            game->gemme_selected = 3;
        }
        else
        {
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25, 315 + 160, 50, 50))
    { // CASE INVENTAIRE 5
        if (game->inventory_size - 1 >= 4 && game->gemme_selected != 4)
        {
            game->gemme_selected = 4;
        }
        else
        {
            game->gemme_selected = -1;
        }
    }
    else if (is_click_inside(m_x, m_y, WIDTH * CELL_SIZE + 25 + 100, 315 + 160, 50, 50))
    { // CASE INVENTAIRE 6
        if (game->inventory_size - 1 >= 4 && game->gemme_selected != 4)
        {
            game->gemme_selected = 4;
        }
        else
        {
            game->gemme_selected = -1;
        }
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
        return; // Ignore if the game is NULL or if the array is full
    }

    game->visualProjectiles[game->numVisualProjectiles++] = vp;
}

void shootAtMonster(Game *game, Monster *targetMonster, Tower *tower) {
    if (game == NULL || targetMonster == NULL || tower == NULL || tower->gemme == NULL) {
        printf("Error ------> Shooting function has a NULL parameter.\n");
        return;
    }

    Gemme *gemme = tower->gemme;
    float damage = calculateGemDamage(gemme, targetMonster);
    applay_Damage(targetMonster, damage);
    applyElementalEffect(gemme, targetMonster);

    VisualProjectile vp = {
        .start = tower->position,
        .end = {targetMonster->x, targetMonster->y},
        .color = determineProjectileColor(gemme) 
    };
    addVisualProjectile(game, vp); 

    printf("Shot at monster with %f damage.\n", damage);
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
            if (tower->gemme->cooldown > 0)
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
                tower->gemme->cooldown -= deltaTime;
                if (tower->gemme->cooldown <= 0)
                {
                    Monster * targetID = findStrongestMonsterWithinRange(game, tower->position, 3);
                    if (targetID != NULL)
                    {
                        shootAtMonster(game, targetID, tower);
                        tower->gemme->cooldown = 0.5;
                    }
                }
            }
        }
    }
}



// void shootAtMonster(Game *game, Monster *targetMonster, Gemme *gemme) {
//     if (game == NULL || targetMonster == NULL || gemme == NULL) {
//         printf("Error ------> shooting function have a NULL parameter to check\n");
//         return;
//     }

//     float damage = calculateGemDamage(gemme, targetMonster);

//     applay_Damage(targetMonster, damage);

//     applyElementalEffect(gemme, targetMonster);

//     printf("Shot at monster with %f damage.\n", damage);
// }
