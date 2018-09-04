#ifndef _MISC_H_
#define _MISC_H_

Uint16 GetFrameNo(Sint16 dir);
void DrawPixmapFrame(CharacterData *my);
void DrawPixmap(PixData *my, Sint16 x, Sint16 y);
RcHitEnum HitDelete(CharacterData *my, CharacterData *your);
RcHitEnum HitNone(CharacterData *my, CharacterData *your);
RcHitEnum MoveStandard(CharacterData *my);
RcHitEnum MoveStandardNoFrame(CharacterData *my);
Uint16 GetDirection(CharacterData *your, CharacterData *my);
void GetDirectionAdd(Uint16, Sint16 *ax, Sint16 *ay, Uint16 speed);
void  ClearAndGameOver(void);
void Infomation(void);

#endif
