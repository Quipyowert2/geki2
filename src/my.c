#include "geki2.h"
#include "extern.h"

/****************************
  自分の武器移動
 ****************************/
RcHitEnum MoveMyShot(CharacterData *my)
{
  switch (my->Cnt1) {
  case 0:
    my->CntX += my->AddX;
    my->CntY += my->AddY;
    my->X = my->CntX >> 8;
    my->Y = my->CntY >> 8;
    if (my->Y < AREA_LY - my->Spr[my->FrameNo]->Image->Height || my->Y > AREA_RY
        || my->X < AREA_LX - my->Spr[my->FrameNo]->Image->Width || my->X > AREA_RX)
      return RcHitDel;
    return RcHitNone;
  case 2:
    my->FrameNo ++;
  case 1:
    my->Y -= 4;
    my->Cnt1 ++;
    return RcHitNone;
  case 3:
    return RcHitDel;
  }
  return RcHitNone;
}

/****************************
  自分の武器当たり
 ****************************/
RcHitEnum HitMyShot(CharacterData *my, CharacterData *your)
{
  if (my->Spr == PixMyLaser) /*レーザーは消える*/
    return RcHitDel;
  else if (my->Spr == PixMyShot) { /*ショットはアニメをして消える*/
    my->Attr = AttrNone;
    my->Target = AttrNone;
    my->FrameNo = 1;
    my->Cnt1 = 1;
  } /*フラッシュは無敵*/
  return RcHitNone;
}

/****************************
  自分ショット作成
 ****************************/
void CreateMyWeaponShot(Sint16 x, Sint16 y, Uint16 dir, Uint8 no)
{
  int rc;

  Cchr.Attr   = AttrMShot;
  Cchr.Target = AttrEnemy | AttrEBomb;
  Cchr.Cnt3   = 1;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), 28);
  Cchr.X      = x;
  Cchr.Y      = y;
  Cchr.Spr    = PixMyShot;
  if ((rc = CopyMyNew(MoveMyShot, HitMyShot, DrawPixmapFrame)) > 0)
    Root->My[rc]->Chr.FrameNo = no;
}

/****************************
  自分レーザー作成
 ****************************/ 
void CreateMyWeaponLaser(Sint16 x, Sint16 y, Uint16 dir, Uint8 no)
{
  int rc;

  Cchr.Attr   = AttrMShot;
  Cchr.Target = AttrEnemy | AttrEBomb;
  Cchr.Cnt3   = Root->MyPower + 2;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), PixMyLaser[3]->Image->Height);
  Cchr.X      = x;
  Cchr.Y      = y;
  Cchr.Spr    = PixMyLaser;
  if ((rc = CopyMyNew(MoveMyShot, HitMyShot, DrawPixmapFrame)) > 0)
    Root->My[rc]->Chr.FrameNo = no;
}

/****************************
  自分フラッシュ作成
 ****************************/ 
void CreateMyWeaponFlash(Sint16 x, Sint16 y, Uint16 dir)
{
  int rc;

  Cchr.Attr   = AttrMShot;
  Cchr.Target = AttrEnemy | AttrEBomb;
  Cchr.Cnt3   = 4;
  GetDirectionAdd(dir, &(Cchr.AddX), &(Cchr.AddY), 9);
  Cchr.X      = x;
  Cchr.Y      = y;
  Cchr.Spr    = PixFlash;
  if ((rc = CopyMyNew(MoveMyShot, HitMyShot, DrawPixmapFrame)) > 0)
    Root->My[rc]->Chr.FrameNo = 0;
}

/****************************
  自分移動
 ****************************/
RcHitEnum MoveMy(CharacterData *my)
{
  static int mcnt = 0;
  Sint16 sx, sy, i;

  /** ポーズチェック **/
  if (Root->Key & KPause) {
    if (Root->MainFlag == MainGame) {
      Root->MainFlag = MainPause;
      return RcHitNone;
    }
  }
  /** 右に移動 **/
  if (Root->Key & KRight)
    my->X += my->AddX;
  /** 左に移動 **/
  if (Root->Key & KLeft)
    my->X -= my->AddX;
  /** 上に移動 **/
  if (Root->Key & KUp)
    my->Y -= my->AddY;
  /** 下に移動 **/
  if (Root->Key & KDown)
    my->Y += my->AddY;
  /** 移動範囲チェック **/
  if (my->X < AREA_LX)
    my->X = AREA_LX;
  else if (my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->X = AREA_RX - my->Spr[0]->Image->Width;
  if (my->Y < AREA_LY + 20)
    my->Y = AREA_LY + 20;
  else if (my->Y > AREA_RY - my->Spr[0]->Image->Height - 20)
    my->Y = AREA_RY - my->Spr[0]->Image->Height - 20;
  /** フレームチェック **/
  my->FrameNo = 0;

  if (my->Cnt1 > 0) {
    if (my->Cnt1 > 30) {
      /** 速い点滅 **/
      if (my->Cnt1 % 2) {
	my->FrameNo = 1;
      }
    } else {
      /** 遅い点滅 **/
      if (my->Cnt1 % 4 == 0) {
	my->FrameNo = 1;
      }
    }
    /** 無敵終了チェック **/
    if (-- my->Cnt1 == 0)
      Root->My[0]->Chr.Target = AttrEnemy | AttrEShot | AttrItem;
  }

  /** フラッシュ **/
  if (Root->MyFlash == True) {
    sx = my->X - 1 + my->Spr[my->FrameNo]->Image->Width / 2;
    sy = my->Y - 1 + my->Spr[my->FrameNo]->Image->Height / 2;
    for (i = 0; i < 360; i += 30)
      CreateMyWeaponFlash(sx, sy, i);
    Root->MyFlash = False;
  }
  /** 武器発射 **/
  if (Root->Key & KShot) {
    if (my->Cnt2 == 0) {
      sx = my->X - 1 + my->Spr[my->FrameNo]->Image->Width / 2;
      sy = my->Y + 16;
      if (Root->MyWeapon == WeaponShot) { /*ショット*/
        if (!Root->MyPower)
          CreateMyWeaponShot(sx - PixMyShot[0]->Image->Width / 2, sy, 180, 0);
        sy += 6;
        if (Root->MyPower > 0) {
          CreateMyWeaponShot(sx - 25, sy, 180,      0);
          CreateMyWeaponShot(sx + 15, sy, 180,      0);
        }
        if (Root->MyPower > 1) {
          CreateMyWeaponShot(sx - 15, sy, 180 + 20, 0);
          CreateMyWeaponShot(sx +  5, sy, 180 - 20, 0);
        }
        if (Root->MyPower > 2) {
          CreateMyWeaponShot(sx - 15, sy, 180 + 45, 0);
          CreateMyWeaponShot(sx +  5, sy, 180 - 45, 0);
        }
        my->Cnt2 = 3;
      } else { /*レーザー*/
        CreateMyWeaponLaser(sx - PixMyLaser[Root->MyPower]->Image->Width / 2, sy, 180, Root->MyPower);
        my->Cnt2 = 2;
      }
    }
  }
  if (my->Cnt2 > 0)
    my->Cnt2 --;
  return RcHitNone;
}

/****************************
  自分作成
 ****************************/
void ReCreateMy(void) {
  Root->My[0]->Chr.Attr    = AttrMy | AttrShadow;
  Root->My[0]->Chr.Target  = AttrItem;
  Root->My[0]->Chr.Cnt1    = 3000 / FRAME_RATE;
  Root->My[0]->Chr.Spr     = PixMy;
  Root->My[0]->Chr.FrameNo = 0;
  Root->My[0]->Chr.X       = PICT_WIDTH / 2 - Root->My[0]->Chr.Spr[0]->Image->Width / 2;
  Root->My[0]->Chr.Y       = AREA_RY - Root->My[0]->Chr.Spr[0]->Image->Height - 20;
  Root->My[0]->Move        = MoveMy;
  Root->My[0]->Draw        = DrawPixmapFrame;
  Root->My[0]->Hit         = HitMy;
}

/****************************
  自分爆発
 ****************************/
RcHitEnum MoveMyBomb(CharacterData *my)
{
  Sint8 no[] = {0,1,2,3,4,3,4,5,4,5,4,3,2,1,2,1,0,1,0,0,-1};

  if (no[my->Cnt1] != -1) {
    my->FrameNo = no[my->Cnt1];
    my->Cnt1 ++;
    return RcHitNone;
  } else {
    if (-- Root->MyMax >= 0) { /*残機あり*/
      if (Root->MyPower) /*武器のパワーダウン*/
        Root->MyPower --;
      ReCreateMy();
    } else { /*残機なし*/
      /*ゲームオーバー*/
      Root->MyMax = 0;
      Root->Cnt = 0;
      Root->MainFlag = MainGameOver;
      return RcHitDel;
    }
  }
  return RcHitNone;
}

/****************************
  自分当り
 ****************************/
RcHitEnum HitMy(CharacterData *my, CharacterData *your)
{
  if (your->Attr & AttrItem) { /*アイテムに当たった*/
    KXL_PlaySound(SE_POW, KXL_SOUND_PLAY);
    if (your->Spr == PixItemLaser) { /*レーザーに当たった*/
      if (Root->MyWeapon == WeaponLaser) { /*現在の武器はレーザー?*/
        if (Root->MyPower < 3) /*パワーアップ*/
          Root->MyPower ++;
        else                   /*フラッシュ*/
          Root->MyFlash = True;
      } else /*ショットに変更*/
        Root->MyWeapon = WeaponLaser;
    } else {
      if (Root->MyWeapon == WeaponShot) { /*現在の武器はショット?*/
        if (Root->MyPower < 3) /*パワーアップ*/
          Root->MyPower ++;
        else                   /*フラッシュ*/
          Root->MyFlash = True;
      } else /*レーザーに変更*/
        Root->MyWeapon = WeaponShot;
    }
  } else if (my->Cnt1 == 0) { /*無敵以外は爆発準備*/
    Root->My[0]->Chr.Attr    = AttrNone;
    Root->My[0]->Chr.Target  = AttrNone;
    Root->My[0]->Chr.Spr     = PixBomb1;
    Root->My[0]->Chr.FrameNo = 0;
    Root->My[0]->Move        = MoveMyBomb;
    KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
  }
  return RcHitNone;
}

/****************************
  自分作成
 ****************************/
void CreateMy(void) {
  Uint16 i;

  for (i = 0; i < MAX_YOUR; i ++)
    Root->Your[i]->Chr.Active = False;
  for (i = 0; i < MAX_MY; i ++)
    Root->My[i]->Chr.Active = False;

  Root->MyNo = 1;
  Root->YourNo = 1;
  if (Root->MainFlag == MainGame) {
    Root->MyWeapon = WeaponShot;
    Root->MyMax    = 2;
    Root->Score    = 0;
    Root->MyPower  = 0;
    Root->OneUp    = 10000;
  }
  Root->MyFlash   = False;
  Root->ScrollCnt =
  Root->EnemyCnt  =
  Root->MapNo     = 0;
  
  Root->My[0]->Chr.Attr     = AttrMy | AttrShadow;
  Root->My[0]->Chr.Target   = AttrItem;
  Root->My[0]->Chr.Cnt1     = 3000 / FRAME_RATE;
  Root->My[0]->Chr.Spr      = PixMy;
  Root->My[0]->Chr.X        = PICT_WIDTH / 2 - Root->My[0]->Chr.Spr[0]->Image->Width / 2;
  Root->My[0]->Chr.Y        = AREA_RY - Root->My[0]->Chr.Spr[0]->Image->Height - 20;
  Root->My[0]->Chr.AddX     = 8;
  Root->My[0]->Chr.AddY     = 8;
  Root->My[0]->Chr.Active   = True;
  Root->My[0]->Chr.Cnt2     = 0;
  Root->My[0]->Chr.FrameNo  = 0;
  Root->My[0]->Chr.FrameMax = 2;
  Root->My[0]->Move         = MoveMy;
  Root->My[0]->Draw         = DrawPixmapFrame;
  Root->My[0]->Hit          = HitMy;
  KXL_PlaySound(Root->Stage, KXL_SOUND_PLAY_LOOP);
}
