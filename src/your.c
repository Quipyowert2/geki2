#include "geki2.h"
#include "extern.h"

/****************************
  爆発
 ****************************/
RcHitEnum MoveBomb(CharacterData *my)
{
  if (++ my->FrameNo != my->FrameMax)
    return RcHitNone;
  return RcHitDel;
}

/****************************
  HPが無くなったら爆発する
 ****************************/
RcHitEnum HitEnemyToBomb(CharacterData *my, CharacterData *your)
{
  my->Hp -= your->Cnt3;
  if (my->Hp <= 0) {
    KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
    /*爆発*/
    Cchr.Attr     = AttrNone;
    Cchr.Target   = AttrNone;
    Cchr.X        = (my->X + my->Spr[0]->Image->Width / 2)
      - (PixBomb1[0]->Image->Width / 2);
    Cchr.Y        = (my->Y + my->Spr[0]->Image->Height / 2)
      - (PixBomb1[0]->Image->Height / 2);
    Cchr.FrameMax = my->Etc == 0 ? 13 : 9;
    Cchr.Spr      = my->Etc == 0 ? PixBomb1 : PixBomb2;
    CopyYourNew(MoveBomb, HitNone, DrawPixmapFrame);
    return RcHitBomb;
  }
  return RcHitNone;
}

/****************************
  "アイテム"出現
 ****************************/
void CreateItem(Sint16 x, Sint16 y, PixData **pix)
{
  KXL_PlaySound(SE_BOMB,KXL_SOUND_PLAY);
  Cchr.Attr     = AttrItem | AttrShadow;
  Cchr.Target   = AttrMy;
  Cchr.Score    = 30;
  Cchr.Spr      = pix;
  Cchr.FrameMax = 1;
  Cchr.X        = x;
  Cchr.Y        = y;
  Cchr.Cnt3     = 0;
  Cchr.Etc      = 0;
  CopyYourNew(MoveItem, HitDelete, DrawPixmapFrame);
}

/****************************
  "アイテム"移動
 ****************************/
RcHitEnum MoveItem(CharacterData *my)
{
  PixData **pix[] = {PixItemShot, PixItemLaser};

  /*座標計算*/
  GetDirectionAdd(my->Cnt1, &(my->AddX), &(my->AddY), 4);
  KXL_DirectionAdd(my->X, my->CntX, my->AddX);
  KXL_DirectionAdd(my->Y, my->CntY, my->AddY);
  if (my->Y > PICT_HEIGHT) /*画面下で消える*/
    return RcHitDel;
  
  if (my->Cnt3 < 8) {
    my->Cnt3 ++;
  } else {
    my->Cnt1 += 6;
    if (my->Cnt1 == 360) { /*アイテムを切り替える*/
      my->Cnt2 = rand() % 2;
      my->Spr = pix[my->Cnt2];
      my->Cnt1 = 0;
      my->Cnt3 = 0;
    }
  }
  return RcHitNone;
}

/****************************
  HPが無くなったら爆発する
 ****************************/
RcHitEnum HitEnemyToItem(CharacterData *my, CharacterData *your)
{
  my->Hp -= your->Cnt3;
  if (my->Hp <= 0) {
    KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
    CreateItem(my->X, my->Y, rand() % 2 ? PixItemShot : PixItemLaser);
    /*爆発*/
    Cchr.Attr     = AttrNone;
    Cchr.Target   = AttrNone;
    Cchr.X        = (my->X + my->Spr[0]->Image->Width / 2)
      - (PixBomb1[0]->Image->Width / 2);
    Cchr.Y        = (my->Y + my->Spr[0]->Image->Height / 2)
      - (PixBomb1[0]->Image->Height / 2);
    Cchr.FrameMax = my->Etc == 0 ? 13 : 9;
    Cchr.Spr      = my->Etc == 0 ? PixBomb1 : PixBomb2;
    CopyYourNew(MoveBomb, HitNone, DrawPixmapFrame);
    return RcHitBomb;
  }
  return RcHitNone;
}

/****************************
  普通の弾発射
 ****************************/
void CreateEnemyShot1(Sint16 x, Sint16 y, Uint16 dir, Uint8 speed, Uint8 sel)
{
  Cchr.Attr   = AttrEShot;
  Cchr.Target = AttrMy;
  Cchr.X      = x;
  Cchr.Y      = y;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), speed);
  Cchr.Spr    = sel == 0 ? PixEnemyShot1 : PixEnemyShot3;
  CopyYourNew(MoveStandardNoFrame, HitDelete, DrawPixmapFrame);
}

/****************************
  回転する弾発射
 ****************************/
void CreateEnemyShot2(Sint16 x, Sint16 y, PixData **spr, Uint8 max, Uint16 dir, Uint8 speed)
{
  Cchr.Attr     = AttrEShot;
  Cchr.Target   = AttrMy;
  Cchr.X        = x;
  Cchr.Y        = y;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), speed);
  Cchr.Spr      = spr;
  Cchr.FrameMax = max;
  CopyYourNew(MoveStandard, HitDelete, DrawPixmapFrame);
}

/****************************
  "ザコ"出現
 ****************************/
void CreateZako(Sint16 x, Sint16 dir, Uint8 speed)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 2 + Root->StageLoop;
  Cchr.Score    = 10;
  Cchr.Etc      = 0;
  Cchr.Spr      = PixZako;
  Cchr.FrameMax = 9;
  Cchr.X        = x;
  Cchr.Y        = AREA_LY - PixZako[0]->Image->Height;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), speed);
  CopyYourNew(MoveZako, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "ザコ"移動
 ****************************/
RcHitEnum MoveZako(CharacterData *my)
{
  /*座標計算*/
  KXL_DirectionAdd(my->X, my->CntX, my->AddX);
  KXL_DirectionAdd(my->Y, my->CntY, my->AddY);
  if (my->Y < 0 || my->Y > AREA_RY || my->X < 0 || my->X > AREA_RX)
    return RcHitDel;
  /*アニメーション*/
  if (++ my->FrameNo == my->FrameMax)
    my->FrameNo = 0;
  /*攻撃*/
  if (my->Cnt2 == 0) {
    if (my->Y > AREA_LY && my->Y < AREA_RY - 100)
      if (rand() % (12 - Root->Stage - Root->StageLoop) == 0)
        CreateEnemyShot1(my->X + my->Spr[0]->Image->Width / 2,
                         my->Y + my->Spr[0]->Image->Height / 2,
                         GetDirection(my, &(Root->My[0]->Chr)) + (rand() % 20 - 10),
                         8 + Root->StageLoop, 0);
    my->Cnt2 = 15;
  } else
    my->Cnt2 --;
  return RcHitNone;
}

/****************************
  "タンク"出現
 ****************************/
void CreateTank(void)
{
  Sint16 ax, ay;

  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 5 + Root->StageLoop;
  Cchr.Score    = 30;
  Cchr.Etc      = 0;
  Cchr.Spr      = PixItem;
  Cchr.FrameMax = 1;
  Cchr.X        = AREA_LX + 100 + (rand() % 200);
  Cchr.Y        = AREA_LY - PixItem[0]->Image->Height;
  Cchr.AddY     = 4;
  CopyYourNew(MoveTank, HitEnemyToItem, DrawPixmapFrame);
}

/****************************
  "タンク"移動
 ****************************/
RcHitEnum MoveTank(CharacterData *my)
{
  /*座標計算*/
  my->Y += my->AddY;
  if (my->Y < 0 || my->Y > AREA_RY)
    return RcHitDel;
  
  if (my->Cnt1 == 0) { /*ある程度下にきたら上下どちらかに移動*/
    if (my->Y > AREA_RY - 200) {
      my->AddY = rand() % 2 ? 1 : -1;
      my->Cnt1 = 1;
    }
  } else { /*攻撃*/
    if (my->Cnt2 == 0) {
      if (rand() % 3 == 0)
        CreateEnemyShot1(my->X + 10, my->Y + 50, 0, 4, 0);
      my->Cnt2 = 20;
    } else
      my->Cnt2 --;
  }
  return RcHitNone;
}

/****************************
  "ユラユラ"出現
 ****************************/
void CreateUraura(Sint16 x, Uint8 sel)
{
  Cchr.Attr     = AttrEnemy|AttrShadow;
  Cchr.Target   = AttrMy|AttrMShot;
  Cchr.Hp       = 5 + Root->StageLoop;
  Cchr.Score    = 50;
  Cchr.Etc      = 0;
  Cchr.Spr      = PixUraUra;
  Cchr.FrameMax = 5;
  Cchr.X        = x;
  Cchr.Y        = AREA_LY - PixUraUra[0]->Image->Height;
  Cchr.Cnt3     = sel;
  CopyYourNew(MoveUraura, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "ユラユラ"移動
 ****************************/
RcHitEnum MoveUraura(CharacterData *my)
{
  int i;

  /*座標計算*/
  if (my->Cnt3 == 0) {
    my->Cnt1 ++;
    if (my->Cnt1 >= 8)
      my->Cnt3 = 1;
  } else {
    my->Cnt1 --;
    if (my->Cnt1 <= -8)
      my->Cnt3 = 0;
  }
  my->X += my->Cnt1;
  my->Y += 4;
  if (my->Y > AREA_RY)
    return RcHitDel;
  /*アニメーション*/
  my->FrameNo = (my->Cnt1 > 4 ? 0 :
		 my->Cnt1 > 0 ? 1 :
		 my->Cnt1 < -4 ? 4 :
		 my->Cnt1 < 0 ? 3 :
		 2);
  /*攻撃*/
  if (my->Cnt2 == 0) {
    if (rand() % 3 == 0)
      if (my->Y > AREA_LY && my->Y < AREA_RY - 50)
        for (i = 0; i <= 30; i += 30)
          CreateEnemyShot1(my->X + i,
                           my->Y + my->Spr[0]->Image->Height / 2,
                           0,
                           8, 0);
    my->Cnt2 = 20 - Root->StageLoop * 2;
  } else my->Cnt2 --;
  return RcHitNone;
}

/****************************
  "ジワ"出現
 ****************************/
void CreateJiwa(Sint16 x, Sint16 y, Uint16 dir)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 2 + Root->StageLoop;
  Cchr.Score    = 80;
  Cchr.Etc      = 0;
  Cchr.Spr      = PixJiwa;
  Cchr.FrameMax = 4;
  Cchr.X        = x;
  Cchr.Y        = y;
  Cchr.Cnt3     = dir;
  CopyYourNew(MoveJiwa, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "ジワ"移動
 ****************************/
RcHitEnum MoveJiwa(CharacterData *my)
{
  int no[] = {0,1,0,1,2,1,0,1,2,3};
  
  my->FrameNo = no[my->Cnt1];
  if (my->Cnt1 < 9) /*アニメーション*/
    my->Cnt1 ++;
  else {
    /*座標計算*/
    my->X += my->Cnt3;
    if (my->X < 0 || my->X > AREA_RX)
      return RcHitDel;
    /*攻撃*/
    if (my->Cnt2 == 0) {
      if (my->X > AREA_LX && my->X < AREA_RX - my->Spr[0]->Image->Width)
        CreateEnemyShot1(my->X + my->Spr[0]->Image->Width / 2,
                         my->Y + my->Spr[0]->Image->Height / 2,
                         GetDirection(my , &(Root->My[0]->Chr)) + (rand() % 90 - 45),
                         6 + (rand() % 3), 1);
      my->Cnt2 = 16 - Root->StageLoop * 2;
    } else my->Cnt2 --;
  }
  return RcHitNone;
}

/****************************
  HPが無くなったら爆発する
 ****************************/
RcHitEnum HitEnemyToHoudai(CharacterData *my, CharacterData *your)
{
  my->Hp -= your->Cnt3;
  if (my->Hp <= 0) {
    Root->Score += my->Score;
    my->Attr = AttrNone;
    my->Target = AttrNone;
    my->FrameNo = my->FrameMax - 1;
    KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
    /*爆発*/
    Cchr.Attr     = AttrNone;
    Cchr.Target   = AttrNone;
    Cchr.X        = (my->X + my->Spr[0]->Image->Width / 2)
      - (PixBomb1[0]->Image->Width / 2);
    Cchr.Y        = (my->Y + my->Spr[0]->Image->Height / 2)
      - (PixBomb1[0]->Image->Height / 2);
    Cchr.FrameMax = my->Etc == 0 ? 13 : 9;
    Cchr.Spr      = my->Etc == 0 ? PixBomb1 : PixBomb2;
    CopyYourNew(MoveBomb, HitNone, DrawPixmapFrame);
  }
  return RcHitNone;
}

/****************************
  "砲台"出現
 ****************************/
void CreateHoudai(Sint16 x, Uint16 dir, Uint8 speed)
{
  Cchr.Attr     = AttrEnemy;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 25 + Root->StageLoop * 2;
  Cchr.Score    = 80;
  Cchr.Etc      = 1;
  Cchr.Spr      = PixHoudai;
  Cchr.FrameMax = 13;
  Cchr.X        = x;
  Cchr.Y        = AREA_LY - PixHoudai[0]->Image->Height;
  CopyYourNew(MoveHoudai, HitEnemyToHoudai, DrawPixmapFrame);
}

/****************************
  "砲台"移動
 ****************************/
RcHitEnum MoveHoudai(CharacterData *my)
{
  int i;

  /*座標計算*/
  if (++ my->Y > AREA_RY)
    return RcHitDel;
  
  if (my->Attr == AttrNone)
    return RcHitNone;

  /*アニメーション*/
  my->FrameNo = GetFrameNo(GetDirection(my , &(Root->My[0]->Chr)));
  /*攻撃*/
  if (my->Cnt2 == 0) {
    if (my->Y > AREA_LY && my->Y < AREA_RY - 50)
      if (rand() % (5 - Root->StageLoop) == 0)
        for (i =- 10; i < 20; i += 10)
          CreateEnemyShot1(my->X + my->Spr[0]->Image->Width / 2,
                           my->Y + my->Spr[0]->Image->Height / 2,
                           my->FrameNo * 30 + i,
                           10, 1);
    my->Cnt2 = 15 - Root->StageLoop * 3;
  } else
    my->Cnt2 --;
  return RcHitNone;
}

/****************************
  "Uターン"出現
 ****************************/
void CreateUturn(Sint16 x)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 5 + Root->StageLoop;
  Cchr.Score    = 50;
  Cchr.Etc      = 1;
  Cchr.Spr      = PixUturn;
  Cchr.FrameMax = 7;
  Cchr.X        = x;
  Cchr.Y        = AREA_LY - PixUturn[0]->Image->Height;
  CopyYourNew(MoveUturn, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "Uターン"移動
 ****************************/
RcHitEnum MoveUturn(CharacterData *my)
{
  int i;
  int spd[] = {24,24,24,24,24,24,24,24,24,24,24,24,24,24,16,16,8,0,-8,-8,-16,-16,-24};
  int no[] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3, 4, 5, 6, 6, 6};

  /*座標計算*/
  my->Y += spd[my->Cnt1];
  if (my->Y > AREA_RY || my->Y < 0)
    return RcHitDel;

  /*アニメーション*/
  my->FrameNo = no[my->Cnt1];
  if (my->Cnt1 < 22)
    my->Cnt1 ++;
  /*攻撃*/
  if (my->Cnt1 >= 16 && my->Cnt1 <= 18) {
    if (rand() % 2 > 0)
      if (my->Y > AREA_LY && my->Y < AREA_RY - 50)
        for (i =- 45; i <= 45; i += 45)
          CreateEnemyShot2(my->X + my->Spr[0]->Image->Width / 2,
                           my->Y + my->Spr[0]->Image->Height / 2,
                           PixEnemyShot2,
                           6,
                           i + (rand() % 10 - 5),
                           8 + rand() % 2);
  }
  return RcHitNone;
}

/****************************
  "くるくる"出現
 ****************************/
void CreateKuru(void)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 5 + Root->StageLoop;
  Cchr.Score    = 30;
  Cchr.Etc      = 1;
  Cchr.Spr      = PixKuru;
  Cchr.FrameMax = 12;
  Cchr.X        = 
    AREA_LX + (rand() % (DRAW_WIDTH / PixKuru[0]->Image->Width)) * PixKuru[0]->Image->Width;
  Cchr.Y        = AREA_LY - PixKuru[0]->Image->Height;
  Cchr.Cnt3     = (rand() % 3) * 32;
  Cchr.AddX     = 0;
  Cchr.AddY     = 24;
  CopyYourNew(MoveKuru, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "くるくる"移動
 ****************************/
RcHitEnum MoveKuru(CharacterData *my)
{
  /*座標計算*/
  my->X += my->AddX;
  my->Y += my->AddY;
  if (my->Y < 0 || my->Y > AREA_RY || my->X < 0 || my->X > AREA_RX)
    return RcHitDel;
  
  if (my->Cnt1 == 0) { /*画面下まで移動*/
    if (my->Y > AREA_RY - 200 - my->Cnt3) {
      my->AddX = ((rand() % 3) - 1) * 2;
      my->AddY = -4;
      my->Cnt1 = 1;
    }
  } else { /*アニメーション*/
    my->FrameNo = GetFrameNo(GetDirection(my, &(Root->My[0]->Chr)));
    /*攻撃*/
    if (my->Cnt2 == 0) {
      if (rand() % 30 > (20 + Root->StageLoop))
        CreateEnemyShot2(my->X + my->Spr[0]->Image->Width / 2,
                         my->Y + my->Spr[0]->Image->Height / 2,
                         PixEnemyShot2,
                         6,
                         GetDirection(my, &(Root->My[0]->Chr)),
                         8);
      my->Cnt2 = 20 - Root->StageLoop * 2;
    } else
      my->Cnt2 --;
  }
  return RcHitNone;
}

/****************************
  "中ボス"出現
 ****************************/
void CreateMiddleBoss(void)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 50 + Root->StageLoop * 3;
  Cchr.Score    = 500;
  Cchr.Etc      = 1;
  Cchr.Spr      = PixMboss;
  Cchr.FrameMax = 1;
  Cchr.X        = AREA_LX;
  Cchr.Y        = AREA_LY - PixMboss[0]->Image->Height;
  Cchr.AddX     = 2;
  Cchr.AddY     = 8;
  CopyYourNew(MoveMiddleBoss, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "中ボス"移動
 ****************************/
RcHitEnum MoveMiddleBoss(CharacterData *my)
{
  int i;

  /*座標計算*/
  my->X += my->AddX;
  my->Y += my->AddY;
  if (my->Cnt1 == 0)
    if (my->Y < AREA_LY + 20)
      return RcHitNone;
    else {
      my->Cnt1 = 1;
      my->AddY = -1;
    }
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);
  if (my->Y > AREA_LY + 100 || my->Y < AREA_LY)
    my->AddY = -(my->AddY);

  /*攻撃*/
  my->Cnt2 ++;
  if (my->Cnt2 % 10 == 0) {
    if (rand() % 2) {
      for (i = 0; i < 2; i ++)
	CreateEnemyShot2(my->X + 34 + i * 38, my->Y + 82,
			 PixEnemyShot2,
			 6,
			 GetDirection(my, &(Root->My[0]->Chr)) + (rand() % 10 - 5),
			 10);
    }    
  }
  return RcHitNone;
}

/****************************
  "ちょろ"出現
 ****************************/
void CreateChoro(Sint16 x)
{
  Cchr.Attr     = AttrEnemy | AttrShadow;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.Hp       = 20 + Root->StageLoop * 3;
  Cchr.Score    = 100;
  Cchr.Etc      = 1;
  Cchr.Spr      = PixChoro;
  Cchr.FrameMax = 12;
  Cchr.X        = x;
  Cchr.Y        = AREA_LY - PixChoro[0]->Image->Height;
  Cchr.Cnt3     = 0;
  Cchr.Cnt4     = x < AREA_LX + DRAW_WIDTH / 2 ? 0 : 1;
  Cchr.Cnt5     = Cchr.Cnt4 == 0 ? 0 : 360;
  CopyYourNew(MoveChoro, HitEnemyToBomb, DrawPixmapFrame);
}

/****************************
  "ちょろ"移動
 ****************************/
RcHitEnum MoveChoro(CharacterData *my)
{
  /*座標計算*/
  GetDirectionAdd(my->Cnt5, &(my->AddX), &(my->AddY), 12);
  KXL_DirectionAdd(my->X, my->CntX, my->AddX);
  KXL_DirectionAdd(my->Y, my->CntY, my->AddY);
  if (my->Cnt1 < 25) {
    my->Cnt1 ++;
    return RcHitNone;
  }
  if (my->Y < 0 || my->Y > AREA_RY || my->X < 0 || my->X > AREA_RX)
    return RcHitDel;

  if (my->Cnt2 == 0) {
    if (my->Cnt4 == 0) {
      my->Cnt5 += 9;
      if (my->Cnt5 >= 360) {
	my->Cnt2 = 1;
	my->Cnt5 = GetDirection(my, &(Root->My[0]->Chr));
      }
    } else {
      my->Cnt5 -= 9;
      if (my->Cnt5 <= 0) {
	my->Cnt2 = 1;
	my->Cnt5 = GetDirection(my, &(Root->My[0]->Chr));
      }
    }
  }
  my->FrameNo = GetFrameNo(my->Cnt5);
  if (my->Cnt3 == 0) {
    if (rand() % (10 - Root->Stage - Root->StageLoop) == 0)
      CreateEnemyShot1(my->X + my->Spr[0]->Image->Width / 2,
		       my->Y + my->Spr[0]->Image->Height / 2,
		       GetDirection(my, &(Root->My[0]->Chr)) + (rand() % 10 - 5),
		       10, 1);
    my->Cnt3 = (30 - Root->Stage - Root->StageLoop);
  } else
    my->Cnt3 --;

  return RcHitNone;
}

