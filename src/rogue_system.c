#include "global.h"
#include "random.h"
#include "item.h"
#include "battle.h"
#include "rogue_system.h"
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/moves.h"

// ---------------------------------------------------------------------------
// NOTE: ABILITY_STRIKER is a custom ability not yet added to
// include/constants/abilities.h. Remove it from sAbilityPool_B until it is
// defined, then add it back.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Ability pools (static — not exposed in header)
// ---------------------------------------------------------------------------

static const enum Ability sAbilityPool_S[] = {
    ABILITY_SPEED_BOOST,
    ABILITY_MAGIC_GUARD,
};

static const enum Ability sAbilityPool_A[] = {
    ABILITY_INTIMIDATE,
    ABILITY_REGENERATOR,
    ABILITY_DROUGHT,
    ABILITY_DRIZZLE,
    ABILITY_ADAPTABILITY,
    ABILITY_PROTEAN,
    ABILITY_LIBERO,
    ABILITY_MOXIE,
    ABILITY_SHEER_FORCE,
};

static const enum Ability sAbilityPool_B[] = {
    ABILITY_GUTS,
    ABILITY_SWIFT_SWIM,
    ABILITY_CHLOROPHYLL,
    ABILITY_FLASH_FIRE,
    ABILITY_LEVITATE,
    ABILITY_THICK_FAT,
    ABILITY_IRON_FIST,
    ABILITY_RECKLESS,
    ABILITY_HUSTLE,
    // ABILITY_STRIKER,  // TODO: add once custom ability is defined
};

// ---------------------------------------------------------------------------
// Pokemon pools
// ---------------------------------------------------------------------------

static const u16 sPokemonPool_S[] = {
    SPECIES_LARVITAR,
    SPECIES_BAGON,
    SPECIES_BELDUM,
    SPECIES_GIBLE,
    SPECIES_DEINO,
    SPECIES_DREEPY,
};

static const u16 sPokemonPool_A[] = {
    SPECIES_RALTS,
    SPECIES_RIOLU,
    SPECIES_EEVEE,
    SPECIES_GASTLY,
    SPECIES_DRATINI,
    SPECIES_MAGBY,
    SPECIES_SCYTHER,
};

static const u16 sPokemonPool_B[] = {
    SPECIES_MACHOP,
    SPECIES_ABRA,
    SPECIES_PONYTA,
    SPECIES_MAGNEMITE,
    SPECIES_SLOWPOKE,
    SPECIES_MAREEP,
    SPECIES_SNEASEL,
    SPECIES_MISDREAVUS,
};

// ---------------------------------------------------------------------------
// Move pools
// ---------------------------------------------------------------------------

static const u16 sMovePool_S[] = {
    MOVE_EARTHQUAKE,
    MOVE_CLOSE_COMBAT,
    MOVE_DRAGON_DANCE,
    MOVE_CALM_MIND,
    MOVE_QUIVER_DANCE,
    MOVE_NASTY_PLOT,
};

static const u16 sMovePool_A[] = {
    MOVE_FLAMETHROWER,
    MOVE_THUNDERBOLT,
    MOVE_ICE_BEAM,
    MOVE_SURF,
    MOVE_SHADOW_BALL,
    MOVE_ENERGY_BALL,
    MOVE_STONE_EDGE,
};

static const u16 sMovePool_B[] = {
    MOVE_AERIAL_ACE,
    MOVE_ROCK_SLIDE,
    MOVE_BRICK_BREAK,
    MOVE_SHADOW_CLAW,
    MOVE_FIRE_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_THUNDER_PUNCH,
};

// ---------------------------------------------------------------------------
// Internal: RollTier
// Returns 0 = S, 1 = A, 2 = B
// Approximate odds: S ~5%, A ~25%, B ~70%
// ---------------------------------------------------------------------------

static u8 RollTier(void)
{
    u32 roll = Random() % 100;
    if (roll < 5)  return 0; // S
    if (roll < 30) return 1; // A
    return 2;                // B
}

// ---------------------------------------------------------------------------
// Gacha roll functions — pure logic, no side effects
// ---------------------------------------------------------------------------

enum Ability Rogue_RollAbility(void)
{
    u8 tier = RollTier();
    switch (tier)
    {
    case 0: return sAbilityPool_S[Random() % ARRAY_COUNT(sAbilityPool_S)];
    case 1: return sAbilityPool_A[Random() % ARRAY_COUNT(sAbilityPool_A)];
    default: return sAbilityPool_B[Random() % ARRAY_COUNT(sAbilityPool_B)];
    }
}

u16 Rogue_RollPokemon(void)
{
    u8 tier = RollTier();
    switch (tier)
    {
    case 0: return sPokemonPool_S[Random() % ARRAY_COUNT(sPokemonPool_S)];
    case 1: return sPokemonPool_A[Random() % ARRAY_COUNT(sPokemonPool_A)];
    default: return sPokemonPool_B[Random() % ARRAY_COUNT(sPokemonPool_B)];
    }
}

u16 Rogue_RollMove(void)
{
    u8 tier = RollTier();
    switch (tier)
    {
    case 0: return sMovePool_S[Random() % ARRAY_COUNT(sMovePool_S)];
    case 1: return sMovePool_A[Random() % ARRAY_COUNT(sMovePool_A)];
    default: return sMovePool_B[Random() % ARRAY_COUNT(sMovePool_B)];
    }
}

// ---------------------------------------------------------------------------
// Run lifecycle
// ---------------------------------------------------------------------------

void Rogue_StartRun(void)
{
    struct RogueRun *run = &gSaveBlock1Ptr->rogueRun;
    run->isRunActive         = TRUE;
    run->currentFloor        = 0;
    run->battlesWon          = 0;
    run->bossDefeated        = FALSE;
    run->curseFlags          = 0;
    run->encountersTotal     = 2;
    run->encountersRemaining = 2;
}

void Rogue_EndRun(void)
{
    gSaveBlock1Ptr->rogueRun.isRunActive = FALSE;
}

void Rogue_AdvanceFloor(void)
{
    struct RogueRun *run = &gSaveBlock1Ptr->rogueRun;
    run->battlesWon++;
    run->currentFloor++;
}

void Rogue_InitFloor(void)
{
    struct RogueRun *run = &gSaveBlock1Ptr->rogueRun;
    // Scale encounters with depth: floors 0-4 = 2, floors 5-9 = 3, etc.
    run->encountersTotal     = 2 + (run->currentFloor / 5);
    run->encountersRemaining = run->encountersTotal;
}

void Rogue_OnBattleWon(void)
{
    struct RogueRun *run = &gSaveBlock1Ptr->rogueRun;
    if (run->encountersRemaining > 0)
        run->encountersRemaining--;
}

// ---------------------------------------------------------------------------
// Run state queries
// ---------------------------------------------------------------------------

bool32 Rogue_IsRunActive(void)
{
    return gSaveBlock1Ptr->rogueRun.isRunActive;
}

u8 Rogue_GetCurrentFloor(void)
{
    return gSaveBlock1Ptr->rogueRun.currentFloor;
}

u8 Rogue_CheckEncountersRemaining(void)
{
    return gSaveBlock1Ptr->rogueRun.encountersRemaining;
}
