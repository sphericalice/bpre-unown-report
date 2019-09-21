#ifndef GUARD_POKEDEX_H
#define GUARD_POKEDEX_H

enum
{
    FLAG_GET_SEEN,
    FLAG_GET_CAUGHT,
    FLAG_SET_SEEN,
    FLAG_SET_CAUGHT
};

s8 GetSetPokedexFlag(u16 nationalNum, u8 caseId);

#endif // GUARD_POKEDEX_H