
//{{BLOCK(unown)

//======================================================================
//
//	unown, 256x160@4, 
//	+ palette 16 entries, not compressed
//	+ 30 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), lz77 compressed, 32x20 
//	Total size: 32 + 324 + 272 = 628
//
//	Time-stamp: 2019-09-19, 08:23:44
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_UNOWN_H
#define GRIT_UNOWN_H

#define unownTilesLen 324
extern const unsigned int unownTiles[81];

#define unownMapLen 272
extern const unsigned short unownMap[136];

#define unownPalLen 32
extern const unsigned short unownPal[16];

#endif // GRIT_UNOWN_H

//}}BLOCK(unown)
