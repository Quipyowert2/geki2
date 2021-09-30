#ifndef _LOAD_H_
#define _LOAD_H_

PixData **LoadPixmapSplit(const char *fname, Uint8 blend, Uint16 max, float per);
PixData *LoadPixmap(const char *fname, Uint8 blend, float per);
PixData **LoadPixmaps(const char *fname, Uint8 blend, Uint16 max, float per);
void UnLoadPixmap(PixData *my);
void UnLoadPixmaps(PixData **my, int max);
void CreatePixmap(void);
void DeletePixmap(void);
void LoadStageData(void);
void UnLoadStageData(void);

#endif
