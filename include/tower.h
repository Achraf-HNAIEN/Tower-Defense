#ifndef TOWER_H
#define TOWER_H

#include "monstre.h"  // Assuming this contains the Monster definition

// Constants for different element types
#define ELEMENT_TYPE_NONE 0
#define ELEMENT_TYPE_PYRO 1
#define ELEMENT_TYPE_DENDRO 2
#define ELEMENT_TYPE_HYDRO 3
#define TOWER 2
// Tower structure
typedef struct {
    int x, y;                // Tower position
    float range;             // Shooting range
    float damage;            // Base damage per shot
    float attack_speed;      // Attacks per second
    float mana_cost;         // Cost to build/upgrade
    int level;               // Tower level
    int element_type;        // Element type (e.g., Pyro, Dendro, Hydro)
    int targetID;            // ID of the currently targeted monster, -1 if none
    float cooldown;          // Cooldown timer (in seconds)
    int gem_level;           // Level of the gem equipped in the tower
    int reaction_type;       // Type of elemental reaction this tower can cause
    float effect_duration;   // Duration of the elemental effect
    float slow_effect;       // Slow effect percentage (for Hydro towers)
    float splash_radius;     // Splash radius (for Pyro towers)
    float parasite_damage;   // Parasite damage per second (for Dendro towers)
} Tower;

// Function declarations
void InitializeTower(Tower *tower, int x, int y, float range, float damage, 
                     float attack_speed, float mana_cost, int element_type);
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

#endif // TOWER_H
