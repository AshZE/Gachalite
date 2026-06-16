#ifndef GUARD_ROGUE_SYSTEM_H
#define GUARD_ROGUE_SYSTEM_H

#include "global.h"


// ---------------------------------------------------------------------------
// Run lifecycle
// ---------------------------------------------------------------------------

void Rogue_StartRun(void);
void Rogue_EndRun(void);
void Rogue_AdvanceFloor(void);
void Rogue_InitFloor(void);
void Rogue_OnBattleWon(void);
void Rogue_ReplacePartyMon(u8 partySlot, u16 species)
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
