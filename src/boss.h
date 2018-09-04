#ifndef _BOSS_H_
#define _BOSS_H_

RcHitEnum HitEnemyToLargeBomb(CharacterData *my, CharacterData *your);
RcHitEnum MoveWarning(CharacterData *my);
void CreateWarning(void);
RcHitEnum MoveBoss1(CharacterData *my);
RcHitEnum MoveBoss2(CharacterData *my);
RcHitEnum MoveBoss3(CharacterData *my);
RcHitEnum MoveBoss3Laser(CharacterData *my);
RcHitEnum MoveBoss4(CharacterData *my);
RcHitEnum HitEnemyShotBomb(CharacterData *my, CharacterData *your);
RcHitEnum MoveBoss5(CharacterData *my);
RcHitEnum MoveBoss5Laser(CharacterData *my);
RcHitEnum MoveBoss6(CharacterData *my);

#endif
