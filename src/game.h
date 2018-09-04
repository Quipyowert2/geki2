#ifndef _GAME_H_
#define _GAME_H_

Sint16 CopyMyNew(RcHitEnum (*act)(CharacterData *my),
              RcHitEnum (*hit)(CharacterData *my, CharacterData *your),
              void (*re)(CharacterData *my));
Sint16 CopyYourNew(RcHitEnum (*act)(CharacterData *my),
              RcHitEnum (*hit)(CharacterData *my, CharacterData *your),
              void (*re)(CharacterData *my));
void Game(void);
void DeleteAll(void);

#endif
