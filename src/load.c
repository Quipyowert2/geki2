#include "geki2.h"
#include "extern.h"

void SetPer(PixData *my, float per)
{
  my->r.Width  = (Uint16)((float)my->Image->Width  * per + 0.5);
  my->r.Height = (Uint16)((float)my->Image->Height * per + 0.5);
  my->r.Left   = (my->Image->Width  - my->r.Width)  / 2;
  my->r.Top    = (my->Image->Height - my->r.Height) / 2;
}

/**********************
  ピックスマップロード
  分割
 **********************/
PixData **LoadPixmapSplit(Uint8 *fname, Uint8 blend, Uint16 max, float per)
{
  Uint8 filename[64];
  Uint16 w, h, i;
  KXL_Image *img;
  PixData **new;
  KXL_Rect rect;

  /* xpmファイル読み込み */
  sprintf(filename, BMP_PATH "/%s.bmp", fname);
  img = KXL_LoadBitmap(filename, blend);
  w = img->Width / max;
  h = img->Height;
  /* イメージの分割 */
  new = (PixData **)KXL_Malloc(sizeof(PixData *) * max);
  for (i = 0; i < max; i ++) {
    new[i] = (PixData *)KXL_Malloc(sizeof(PixData));
    rect.Left   = i * w;
    rect.Top    = 0;
    rect.Width  = w;
    rect.Height = h;
    new[i]->Image = KXL_CopyImage(img, rect);
    SetPer(new[i], per);
  }
  /* 読み込み用Imageを解放 */
  KXL_DeleteImage(img);
  return new;
}

/**********************
  ピックスマップロード
 **********************/
PixData *LoadPixmap(Uint8 *fname, Uint8 blend, float per)
{
  Uint8 filename[64];
  PixData *new;

  /* xpmファイル読み込み */
  sprintf(filename, BMP_PATH "/%s.bmp", fname);
  new = (PixData *)KXL_Malloc(sizeof(PixData));
  new->Image = KXL_LoadBitmap(filename, blend);
  SetPer(new, per);
  return new;
}

/**********************
  ピックスマップロード
 **********************/
PixData **LoadPixmaps(Uint8 *fname, Uint8 blend, Uint16 max, float per)
{
  PixData **new;
  Uint8 filename[64];
  Uint16 i;

  new = (PixData **)KXL_Malloc(sizeof(PixData *) * max);
  for (i = 0; i < max; i ++) {
    sprintf(filename, "%s%d", fname, i + 1);
    new[i] = LoadPixmap(filename, blend, per);
  }
  return new;
}

/**********************
  ピックスマップ解放
 **********************/
void UnLoadPixmap(PixData *my)
{
  KXL_DeleteImage(my->Image);
}

/**********************
  ピックスマップ解放
 **********************/
void UnLoadPixmaps(PixData **my, int max)
{
  while (max)
    UnLoadPixmap(my[-- max]);
  free(my);
}

/**********************
  ピックスマップ作成
 **********************/
void CreatePixmap(void)
{
  PixMy         = LoadPixmapSplit("my", 0, 2 * 2, 0.5);
  PixMyShot     = LoadPixmapSplit("myshot", 0, 3, CHECK_PER);
  PixMyLaser    = LoadPixmaps("mylaser", 0, 4, CHECK_PER);
  PixItem       = LoadPixmapSplit("item", 0, 1 * 2,CHECK_PER);
  PixItemShot   = LoadPixmapSplit("ishot", 0, 2, CHECK_PER);
  PixItemLaser  = LoadPixmapSplit("ilaser", 0, 2, CHECK_PER);
  PixZako       = LoadPixmapSplit("zako", 0, 9 * 2, CHECK_PER);
  PixKuru       = LoadPixmapSplit("kuru", 0, 12 * 2, CHECK_PER);
  PixHoudai     = LoadPixmapSplit("houdai", 0, 13, CHECK_PER);
  PixUraUra     = LoadPixmapSplit("uraura", 0, 5 * 2, CHECK_PER);
  PixUturn      = LoadPixmapSplit("uturn", 0, 7 * 2, CHECK_PER);
  PixJiwa       = LoadPixmapSplit("jiwa", 0, 4 * 2, CHECK_PER);
  PixChoro      = LoadPixmapSplit("choro", 0, 12 * 2, CHECK_PER);
  PixMboss      = LoadPixmapSplit("mboss", 0, 1 * 2, CHECK_PER);
  PixEnemyShot1 = LoadPixmapSplit("enemyshot1", 0, 1, CHECK_PER);
  PixEnemyShot2 = LoadPixmapSplit("enemyshot2", 0, 6, CHECK_PER);
  PixEnemyShot3 = LoadPixmapSplit("enemyshot3", 0, 1, CHECK_PER);
  PixEnemyShot4 = LoadPixmapSplit("enemyshot4", 0, 3, CHECK_PER);
  PixEnemyShot5 = LoadPixmapSplit("enemyshot5", 0, 4, CHECK_PER);
  PixBomb1      = LoadPixmapSplit("bomb1", 0, 13, 0.0);
  PixBomb2      = LoadPixmapSplit("bomb2", 0, 9, 0.0);
  PixFlash      = LoadPixmapSplit("flash", 0, 1, CHECK_PER);
  PixBoss3Laser = LoadPixmapSplit("boss3laser", 0, 4, CHECK_PER);
  PixBoss5Laser = LoadPixmapSplit("elaser", 255, 1, 0.0);
  PixWarning    = LoadPixmapSplit("warning", 0, 2, CHECK_PER);
  
}

/**********************
  ピックスマップ削除
 **********************/
void DeletePixmap(void)
{
  UnLoadPixmaps(PixMy, 2 * 2);
  UnLoadPixmaps(PixMyShot, 3);
  UnLoadPixmaps(PixMyLaser, 4);
  UnLoadPixmaps(PixItem, 1 * 2);
  UnLoadPixmaps(PixItemShot, 2);
  UnLoadPixmaps(PixItemLaser, 2);
  UnLoadPixmaps(PixZako, 9 * 2);
  UnLoadPixmaps(PixKuru, 12 * 2);
  UnLoadPixmaps(PixHoudai, 13);
  UnLoadPixmaps(PixUraUra, 5 * 2);
  UnLoadPixmaps(PixUturn, 7 * 2);
  UnLoadPixmaps(PixJiwa, 4 * 2);
  UnLoadPixmaps(PixChoro, 12 * 2);
  UnLoadPixmaps(PixMboss, 1 * 2);
  UnLoadPixmaps(PixEnemyShot1, 1);
  UnLoadPixmaps(PixEnemyShot2, 6);
  UnLoadPixmaps(PixEnemyShot3, 1);
  UnLoadPixmaps(PixEnemyShot4, 3);
  UnLoadPixmaps(PixEnemyShot5, 4);
  UnLoadPixmaps(PixBomb1, 13);
  UnLoadPixmaps(PixBomb2, 9);
  UnLoadPixmaps(PixFlash, 1);
  UnLoadPixmaps(PixBoss3Laser, 4);
  UnLoadPixmaps(PixBoss5Laser, 1);
  UnLoadPixmaps(PixWarning, 2);
}

/**********************
  ステージデータ読み込み
 **********************/
void LoadStageData(void)
{
  FILE *fp;
  Uint8 buff[256];
  Sint16 dat, i;
  Uint8 bossmax[] = {2, 3 * 2, 1 * 2, 1 * 2, 1 * 2, 1 * 2};
  Uint8 backmax[] = {7, 16, 20, 8, 18, 15};
  
  /* 背景ファイルを開く */
  sprintf(buff, DATA_PATH "/map%d.dat", Root->Stage + 1);
  if ((fp = fopen(buff,"r")) == NULL) {
    fprintf(stderr, "next stage not found\n");
    fprintf(stderr, "see you next version...\n");
    exit(1);
  }
  /* 背景データを読み込む */
  Root->MapMax = 0;
  while (fgets(buff, 255, fp)) {
    if (buff[0] == ';' || buff[0] == '\n')
      continue;
    /*領域を確保する*/
    if (!Root->MapMax)
      MapDatas = (MapData **)KXL_Malloc(sizeof(MapData *));
    else
      MapDatas = (MapData **)KXL_Realloc(MapDatas, sizeof(MapData *) * (Root->MapMax + 1));
    MapDatas[Root->MapMax] = (MapData *)KXL_Malloc(sizeof(MapData));
    /*データ取得*/
    for (i = 0; i < 9; i ++) {
      if (buff[i] >= '0' && buff[i] <= '9')
        dat = buff[i] - '0';
      else
        dat = 10 + buff[i] - 'a';
      MapDatas[Root->MapMax]->no[i] = dat;
    }
    Root->MapMax ++;
  }
  fclose(fp);
  
  /* 敵出現データファイルを読み込む */
  sprintf(buff, DATA_PATH "/stage%d.dat", Root->Stage + 1);
  if ((fp = fopen(buff, "r")) == NULL) {
    fprintf(stderr, "next stage not found\n");
    fprintf(stderr, "see you next version...\n");
    exit(1);
  }
  /* 敵出現データを読み込む */
  Root->StageMax = 0;
  while(fgets(buff, 255, fp)) {
    if (buff[0] == ';' || buff[0] == '\n')
      continue;
    if (!Root->StageMax)
      StageDatas = (StageData **)KXL_Malloc(sizeof(StageData *));
    else
      StageDatas = (StageData **)KXL_Realloc(StageDatas, sizeof(StageData *) * (Root->StageMax + 1));
    StageDatas[Root->StageMax] = (StageData *)KXL_Malloc(sizeof(StageData));
    sscanf(buff,"%d, %d, %d, %d",
           &(StageDatas[Root->StageMax]->Time),
           &(StageDatas[Root->StageMax]->CreateNo),
           &(StageDatas[Root->StageMax]->Max),
           &(StageDatas[Root->StageMax]->Step));
    StageDatas[Root->StageMax]->Flag = 0;
    StageDatas[Root->StageMax ++]->StepTime = 0;
  }
  fclose(fp);
  
  /* ボスキャラを読み込む */
  sprintf(buff, "boss%d", Root->Stage + 1);
  PixBoss = LoadPixmapSplit(buff, 0, bossmax[Root->Stage], 0.6);
  /* 背景キャラを読み込む */
  sprintf(buff, "back%d", Root->Stage + 1);
  PixBack = LoadPixmapSplit(buff, 255, backmax[Root->Stage], 0.0);
}

/**********************
  ステージデータ削除
 **********************/
void UnLoadStageData()
{
  Uint8 bossmax[] = {2, 3 * 2, 1 * 2, 1 * 2, 1 * 2, 1 * 2};
  Uint8 backmax[] = {7, 16, 20, 8, 18, 15};

  while (Root->StageMax)
    free(StageDatas[-- Root->StageMax]);
  free(StageDatas);
  while (Root->MapMax)
    free(MapDatas[-- Root->MapMax]);
  free(MapDatas);
  UnLoadPixmaps(PixBoss, bossmax[Root->Stage]);
  UnLoadPixmaps(PixBack, backmax[Root->Stage]);
}
