#include "global.h"
#include "constants/species.h"
#include "battle.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "unown_report.h"

void atkF1_TrySetCaughtMonDexFlags(void) {
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);
    u32 personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY, NULL);

    if (species == SPECIES_UNOWN) {
        SetCaughtUnown(GetUnownLetterByPersonality(personality));
    }

    species = SpeciesToNationalPokedexNum(species);
    if (GetSetPokedexFlag(species, FLAG_GET_CAUGHT)) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    } else {
        HandleSetPokedexFlag(species, FLAG_SET_CAUGHT, personality);
        gBattlescriptCurrInstr += 5;
    }
}
