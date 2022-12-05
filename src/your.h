#ifndef _YOUR_H_
#define _YOUR_H_

/** Item **/
void CreateItem(Sint16 x, Sint16 y, PixData **pix);
RcHitEnum MoveItem(CharacterData *my);

/** Bomb **/
RcHitEnum MoveBomb(CharacterData *my);
RcHitEnum HitEnemyToBomb(CharacterData *my, CharacterData *your);
RcHitEnum MoveEnemyBomb(CharacterData *my);
RcHitEnum MoveMissile(CharacterData *my);
void CreateEnemyShot1(Sint16 x, Sint16 y, Uint16 dir, Uint8 speed, Uint8 sel);
void CreateEnemyShot2(Sint16 x, Sint16 y, PixData **spr, Uint8 max, Uint16 dir, Uint8 speed);


/** ザコ **/
void CreateZako(Sint16 x, Sint16 dir, Uint8 speed);
RcHitEnum MoveZako(CharacterData *my);

/** タンク **/
void CreateTank(void);
RcHitEnum MoveTank(CharacterData *my);

/** ユラユラ **/
void CreateUraura(Sint16 x, Uint8 sel);
RcHitEnum MoveUraura(CharacterData *my);

/** ジワ **/
void CreateJiwa(Sint16 x, Sint16 y, Uint16 dir);
RcHitEnum MoveJiwa(CharacterData *my);

/** 砲台 **/
void CreateHoudai(Sint16 x, Uint16 dir, Uint8 speed);
RcHitEnum MoveHoudai(CharacterData *my);

/** Uターン **/
void CreateUturn(Sint16 x);
RcHitEnum MoveUturn(CharacterData *my);

/** くるくる **/
void CreateKuru(void);
RcHitEnum MoveKuru(CharacterData *my);

/** 中ボス **/
void CreateMiddleBoss(void);
RcHitEnum MoveMiddleBoss(CharacterData *my);

/** ちょろ **/
void CreateChoro(Sint16 x);
RcHitEnum MoveChoro(CharacterData *my);

#endif
