#ifndef TOWER_H
#define TOWER_H

#include "monstre.h"  
#include "grid.h"    
#include "gemme.h"    

// Constants for different element types
#define ELEMENT_TYPE_NONE 0
#define ELEMENT_TYPE_PYRO 1
#define ELEMENT_TYPE_DENDRO 2
#define ELEMENT_TYPE_HYDRO 3

// Default tower attributes
#define DEFAULT_TOWER_DAMAGE 50
#define DEFAULT_TOWER_RANGE 3
#define DEFAULT_TOWER_ATTACK_SPEED 1.0
#define DEFAULT_TOWER_MANA_COST 100


// Tower structure
typedef struct {
    Point position;           
    float range;             
    float damage;             // basic damage
    float attack_speed;       // en secondes
    float mana_cost;          
    int level;
    int tint;              
    int element_type;         //Pyro, Dendro, Hydro.....
    int targetID;             // ID of the currently targeted, -1 if none
    float cooldown;           // seconds
    int gem_level;            // Level of the gem equipped in the tower
    Gemme_pure gem;           // Gem equipped in the tower
    int reaction_type;        // Type of elemental reaction this tower can cause
    float effect_duration;    // Duration of the elemental effect
    float slow_effect;        // Slow effect in % (for Hydro towers)
    float splash_radius;      // Splash radius (for Pyro towers)
    float parasite_damage;    // Parasite damage per second (for Dendro towers)
} Tower;

void InitializeTower(Tower *tower, Point position, int element_type);
int CanBuildTower(Point position, int grid[HEIGHT][WIDTH]);
void UpgradeTower(Tower *tower);
void SetTarget(Tower *tower, Monster *monsters, int monster_count);
void ShootTarget(Tower *tower, Monster *target);
void ApplyElementalEffect(Tower *tower, Monster *target);
void CauseElementalReaction(Tower *tower, Monster *target);
float CalculateDamage(Tower *tower, Monster *target);
void UpdateTowerCooldown(Tower *tower, float deltaTime);
void HandleSplashDamage(Tower *tower, Monster *monsters, int monster_count);
void ApplySlowEffect(Tower *tower, Monster *target);
void ApplyParasiteDamage(Tower *tower, Monster *target);
void SellTower(Tower *tower);
void DrawTower(const Tower *tower);
void DisplayTowerInfo(const Tower *tower);


#endif 
