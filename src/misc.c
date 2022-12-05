#include "geki2.h"
#include "extern.h"

/**********************
  方角によりフレーム番号取得
 **********************/
Uint16 GetFrameNo(Sint16 dir)
{
  Sint16 min[] = {15, 45, 75,105,135,165,195,225,255,285,315};
  Sint16 max[] = {44, 74,104,134,164,194,224,254,284,314,344};
  Sint16 i;

  for (i = 0; i < 11; i ++)
    if (dir >= min[i] && dir <= max[i])
      return i + 1;
  return 0;
}

/********************************
  イメージ描画(フレームあり)
********************************/
void DrawPixmapFrame(CharacterData *my)
{
  DrawPixmap(my->Spr[my->FrameNo], my->X, my->Y);
}

/********************************
  イメージ描画(フレームなし)
********************************/
void DrawPixmap(PixData *my, Sint16 x, Sint16 y)
{
  KXL_PutImage(my->Image, x, y);
}

/**********************
  当たったら砕けろ
 **********************/
RcHitEnum HitDelete(CharacterData *my, CharacterData *your)
{
  return RcHitDel;
}

/**********************
  当たってもへっちゃら
 **********************/
RcHitEnum HitNone(CharacterData *my, CharacterData *your)
{
  return RcHitNone;
}

/****************************
  基本移動（フレームあり）
 ****************************/
RcHitEnum MoveStandard(CharacterData *my)
{
  KXL_DirectionAdd(my->X, my->CntX, my->AddX);
  KXL_DirectionAdd(my->Y, my->CntY, my->AddY);
  if (++ my->FrameNo == my->FrameMax)
    my->FrameNo = 0;
  if (my->Y < AREA_LY - my->Spr[0]->Image->Height ||
      my->Y > AREA_RY ||
      my->X < AREA_LX - my->Spr[0]->Image->Width ||
      my->X > AREA_RX)
    return RcHitDel;
  return RcHitNone;
}

/****************************
  基本移動（フレームなし）
 ****************************/
RcHitEnum MoveStandardNoFrame(CharacterData *my)
{
  KXL_DirectionAdd(my->X, my->CntX, my->AddX);
  KXL_DirectionAdd(my->Y, my->CntY, my->AddY);
  if (my->Y < AREA_LY - my->Spr[0]->Image->Height ||
      my->Y > AREA_RY ||
      my->X < AREA_LX - my->Spr[0]->Image->Width ||
      my->X > AREA_RX)
    return RcHitDel;
  return RcHitNone;
}

/**********************
  相手の方角取得
 **********************/
Uint16 GetDirection(CharacterData *my, CharacterData *your)
{
  KXL_Rect src, dest;

  src.Left   = my->X;
  src.Top    = my->Y;
  src.Width  = my->Spr[my->FrameNo]->Image->Width;
  src.Height = my->Spr[my->FrameNo]->Image->Height;
  dest.Left   = your->X;
  dest.Top    = your->Y;
  dest.Width  = your->Spr[your->FrameNo]->Image->Width;
  dest.Height = your->Spr[your->FrameNo]->Image->Height;
  return KXL_GetDirection(src, dest);
}

/********************************
  角度による加算値取得
********************************/
void GetDirectionAdd(Uint16 direction, Sint16 *ax, Sint16 *ay, Uint16 speed)
{
  KXL_GetDirectionAdd(direction, ax, ay);
  *ax *= speed;
  *ay *= speed;
}

/********************************
  ステージクリア&ゲームオーバー
********************************/
void ClearAndGameOver(void)
{
  static Uint16 no;
  Uint16 y[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0};
  Uint16 i;
  char *str[3] = {"Clear", "GameOver", "Congratulations"};

  switch (Root->Cnt) {
  case 0:
    KXL_PlaySound(SE_BOMB, KXL_SOUND_STOP_ALL);
    if (Root->MainFlag == MainClear) {
      no = Root->Stage == 5 ? 2 : 0;
    } else {
      ScoreRanking();
      no = 1;
    }
    break;
  case 150:
    UnLoadStageData();
    Root->Cnt = -1;
    if (Root->MainFlag == MainClear) {
      Root->Stage ++;
      if (Root->Stage == 6) {
	Root->Stage = 0;
	Root->StageLoop ++;
      }
      for (i = 0; i < MAX_YOUR; i ++)
	Root->Your[i]->Chr.Active = False;
      for (i = 0; i < MAX_MY; i ++)
	Root->My[i]->Chr.Active = False;
      CreateMy();
      Root->MainFlag = MainGame;
      LoadStageData();
    } else {
      Root->MainFlag = MainOpening;
    }
    break;
  default:
    KXL_Font("-bitstream-charter-bold-*-normal-*-60-*-*-*-*-*-*-*", 0x00, 0x00, 0x00);
    KXL_PutText(AREA_LX + 2 + (DRAW_WIDTH - KXL_TextWidth(str[no])) / 2,
		AREA_LY + 250 + 2 + y[Root->Cnt % 18],
		str[no]);
    KXL_Font(NULL, 0xff, 0xff, 0xff);
    KXL_PutText(AREA_LX + (DRAW_WIDTH - KXL_TextWidth(str[no])) / 2,
		AREA_LY + 250 + y[Root->Cnt % 18],
		str[no]);
    KXL_Font( "-adobe-courier-bold-r-normal--14-*-*-*-*-*-iso8859-1", 0xff, 0xff, 0xff);
    break;
  }
  Root->Cnt ++;
}

/**********************
  インフォメーション
 **********************/
void Infomation(void)
{
  char text[30];
  KXL_Rect r;

  /** スコア **/
  sprintf(text, "Score %07lu", Root->Score);
  KXL_Font(NULL, 0xff, 0xff, 0xff);
  KXL_PutText(AREA_LX + 280, AREA_LY + 12, text);

  /** ハイスコア **/
  if (Root->Score > Root->HiScore)
    Root->HiScore = Root->Score;
  sprintf(text, "Hi-Score %07lu", Root->HiScore);
  KXL_PutText(AREA_LX + 60, AREA_LY + 12, text);

  /** ステージ **/
  sprintf(text, "Stage %d", Root->Stage + 1);
  KXL_PutText(AREA_LX + 180,AREA_RY - 4, text);

  /** ループ **/
  sprintf(text, "Loop %d", Root->StageLoop + 1);
  KXL_PutText(AREA_LX + 300,AREA_RY - 4, text);

  /** 弱機 **/
  sprintf(text, "Left %d", Root->MyMax);
  KXL_PutText(AREA_LX + 60, AREA_RY - 4, text);
}

