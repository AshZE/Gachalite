#ifndef GUARD_ROGUE_SYSTEM_H
#define GUARD_ROGUE_SYSTEM_H

#include "global.h"


// ---------------------------------------------------------------------------
// Upgrade Essence — single source of truth is sUpgradeTable in rogue_system.c.
// BoxPokemon stores only a 1-bit "isUpgraded" flag; everything else is
// resolved at read-time via species lookup.
// ---------------------------------------------------------------------------

#define MAX_UPGRADE_ABILITIES    3
#define MAX_UPGRADE_MOVE_UNLOCKS 8

struct UpgradeEssenceEntry
{
    enum Type newType1;    // TYPE_NONE = no change
    enum Type newType2;    // TYPE_NONE = no change
    s16 statBuffs[NUM_STATS];
    enum Ability newAbilities[MAX_UPGRADE_ABILITIES]; // ABILITY_NONE = no change for that slot
    enum Move moveUnlocks[MAX_UPGRADE_MOVE_UNLOCKS];  // MOVE_NONE-terminated
};

bool32 Rogue_MonCanBeUpgraded(u16 species);
bool32 Rogue_GetUpgradeTypes(u16 species, enum Type *type1, enum Type *type2);
bool32 Rogue_GetUpgradeStatBuffs(u16 species, s16 *buffsOut); // buffsOut: s16[NUM_STATS]
enum Ability Rogue_GetUpgradedAbility(u16 species, u8 abilityNum);
bool32 Rogue_MoveIsUpgradeUnlock(u16 species, enum Move move);
void Rogue_ApplyUpgradeEssence(struct Pokemon *mon);

// ---------------------------------------------------------------------------
// Run lifecycle
// ---------------------------------------------------------------------------

void Rogue_StartRun(void);
void Rogue_EndRun(void);
void Rogue_AdvanceFloor(void);
void Rogue_InitFloor(void);
void Rogue_OnBattleWon(void);
void Rogue_ReplacePartyMon(u8 partySlot, u16 species);
u8   Rogue_GetLevelCap(void);
// ---------------------------------------------------------------------------
// Run state queries
// ---------------------------------------------------------------------------

bool32 Rogue_IsRunActive(void);
u8     Rogue_GetCurrentFloor(void);
u8     Rogue_CheckEncountersRemaining(void);

// ---------------------------------------------------------------------------
// Gacha roll functions — return the result; callers handle all side effects
// ---------------------------------------------------------------------------

enum Ability Rogue_RollAbility(void);  // returns ABILITY_NONE on failure
u16          Rogue_RollPokemon(void);  // returns SPECIES_NONE on failure
u16          Rogue_RollMove(void);     // returns MOVE_NONE on failure

#endif // GUARD_ROGUE_SYSTEM_H
