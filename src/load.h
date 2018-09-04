#ifndef _LOAD_H_
#define _LOAD_H_

PixData **LoadPixmapSplit(Uint8 *fname, Uint8 blend, Uint16 max, float per);
PixData *LoadPixmap(Uint8 *fname, Uint8 blend, float per);
PixData **LoadPixmaps(Uint8 *fname, Uint8 blend, Uint16 max, float per);
void UnLoadPixmap(PixData *my);
void UnLoadPixmaps(PixData **my, int max);
void CreatePixmap(void);
void DeletePixmap(void);
void LoadStageData(void);

#endif
