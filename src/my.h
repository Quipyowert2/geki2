#ifndef _MY_H_
#define _MY_H_

RcHitEnum MoveShotBomb(CharacterData *my);
RcHitEnum HitMyShot(CharacterData *my, CharacterData *your);
RcHitEnum HitMy(CharacterData *my, CharacterData *your);
RcHitEnum MoveMy(CharacterData *my);
RcHitEnum MoveMyBomb(CharacterData *my);
void CreateMy(void);

#endif
