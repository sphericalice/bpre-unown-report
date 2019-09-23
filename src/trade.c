#include "global.h"
#include "constants/species.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "unown_report.h"

void SetTradedMonPokedexFlags(u8 partyIdx) {
    struct Pokemon *mon = &gPlayerParty[partyIdx];

    if (!GetMonData(mon, MON_DATA_IS_EGG))
    {
        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

        if (species == SPECIES_UNOWN) {
            SetCaughtUnown(GetUnownLetterByPersonality(personality));
        }

        species = SpeciesToNationalPokedexNum(species);
        GetSetPokedexFlag(species, FLAG_SET_SEEN);
        HandleSetPokedexFlag(species, FLAG_SET_CAUGHT, personality);
    }
}
