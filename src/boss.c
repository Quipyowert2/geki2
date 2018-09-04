#include "geki2.h"
#include "extern.h"

Uint8 no;

/****************************
  ボス死亡
 ****************************/
RcHitEnum MoveBossEnd(CharacterData *my)
{
  my->X += ((rand() % 3) - 1) * 2;
  my->Y += 4;
  if (my->Y > AREA_RY)
    return RcHitBoss;
  Cchr.Attr     = AttrNone;
  Cchr.Target   = AttrNone;
  Cchr.X        = my->X + (rand() % my->Spr[0]->Image->Width * 0.8);
  Cchr.Y        = my->Y + (rand() % my->Spr[0]->Image->Height
			   * 0.8);
  Cchr.FrameMax = 9;
  Cchr.Spr      = PixBomb2;
  CopyYourNew(MoveBomb, HitNone, DrawPixmapFrame);
  KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
  return RcHitNone;
}

/****************************
  ボス死亡設定
 ****************************/
void CreateBossEnd(void)
{
  int sc[] = {3000, 4000, 5000, 7000, 10000, 15000};

  Root->Your[BOSS_NO]->Chr.Attr     =
  Root->Your[BOSS_NO]->Chr.Target   = AttrNone;
  Root->Your[BOSS_NO]->Chr.Active   = True;
  Root->Your[BOSS_NO]->Chr.FrameNo  =
  Root->Your[BOSS_NO]->Chr.Cnt1     = 0;
  Root->Your[BOSS_NO]->Chr.Spr      = PixBoss;
  Root->Your[BOSS_NO]->Draw         = DrawPixmapFrame;
  Root->Your[BOSS_NO]->Hit          = HitNone;
  Root->Your[BOSS_NO]->Move         = MoveBossEnd;
  Root->Your[BOSS_NO]->Chr.Score    = sc[Root->Stage] + Root->StageLoop * 1000;
}

/****************************
  ボスの爆発
 ****************************/
RcHitEnum HitEnemyToLargeBomb(CharacterData *my, CharacterData *your)
{
  if (my->Cnt1 < 100)
    return RcHitNone;
  my->Hp -= your->Cnt3;
  if (my->Hp % 10 < 2 + rand() % 5) {
    Cchr.Attr     = AttrNone;
    Cchr.Target   = AttrNone;
    Cchr.X        = my->X + (rand() % my->Spr[0]->Image->Width * 0.8);
    Cchr.Y        = my->Y + (rand() % my->Spr[0]->Image->Height * 0.8);
    Cchr.FrameMax = 9;
    Cchr.Spr      = PixBomb2;
    CopyYourNew(MoveBomb, HitNone, DrawPixmapFrame);
    KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
  }
  if (my->Hp <= 0) {
    DeleteAll();
    CreateBossEnd();
  }
  return RcHitNone;
}

/****************************
  ボス設定
 ****************************/
void CreateBoss(void)
{
  RcHitEnum (*mv[])(CharacterData *my) = {
    MoveBoss1, MoveBoss2, MoveBoss3, MoveBoss4, MoveBoss5, MoveBoss6
  };
  Uint16 hp[] = {305, 505, 605, 805, 1205, 1505};
  Uint8 mx[] = {2, 3, 1, 1, 1, 1};
  Sint16 ax, ay;

  Root->Your[BOSS_NO]->Chr.Attr     = (AttrEnemy |
                                       (Root->Stage ? AttrShadow : 0));
  Root->Your[BOSS_NO]->Chr.Target   = AttrMy | AttrMShot;
  Root->Your[BOSS_NO]->Chr.Active   = True;
  Root->Your[BOSS_NO]->Chr.FrameNo  =
  Root->Your[BOSS_NO]->Chr.Cnt1     =
  Root->Your[BOSS_NO]->Chr.Cnt2     =
  Root->Your[BOSS_NO]->Chr.Cnt3     =
  Root->Your[BOSS_NO]->Chr.Cnt4     =
  Root->Your[BOSS_NO]->Chr.Cnt5     =
  Root->Your[BOSS_NO]->Chr.Etc      = 0;
  Root->Your[BOSS_NO]->Chr.FrameMax = mx[Root->Stage];
  Root->Your[BOSS_NO]->Chr.Spr      = PixBoss;
  Root->Your[BOSS_NO]->Chr.X        = AREA_LX + (DRAW_WIDTH - PixBoss[0]->Image->Width) / 2;
  Root->Your[BOSS_NO]->Chr.Y        = AREA_LY - PixBoss[0]->Image->Height;
  Root->Your[BOSS_NO]->Draw         = DrawPixmapFrame;
  Root->Your[BOSS_NO]->Hit          = HitEnemyToLargeBomb;
  Root->Your[BOSS_NO]->Move         = mv[Root->Stage];
  Root->Your[BOSS_NO]->Chr.Hp       = hp[Root->Stage] + Root->StageLoop * 100;
  KXL_PlaySound(SE_WARNING, KXL_SOUND_STOP);
  KXL_PlaySound(SE_BOSS, KXL_SOUND_PLAY_LOOP);
}

/****************************
  ワーニング
 ****************************/
RcHitEnum MoveWarning(CharacterData *my)
{
  if (my->Cnt1 & 1)
    my->FrameNo ++;
  my->FrameNo &= 1;
  my->Cnt1 ++;
  if (my->Cnt1 == 125) {
    CreateBoss();
    return RcHitNone;
  } else if (my->Cnt1 > 125) {
    my->Y += 16;
    if (my->Y > AREA_RY)
      return RcHitDel;
  }
  return RcHitNone;
}

/****************************
  ワーニング
 ****************************/
void CreateWarning(void)
{
  Cchr.Attr   = AttrNone;
  Cchr.Target = AttrNone;
  Cchr.X      = AREA_LX + (DRAW_WIDTH - PixWarning[0]->Image->Width) / 2;
  Cchr.Y      = AREA_LY + (DRAW_HEIGHT - PixWarning[0]->Image->Height) / 2;
  Cchr.Spr    = PixWarning;
  CopyYourNew(MoveWarning, HitNone, DrawPixmapFrame);
  KXL_PlaySound(0, KXL_SOUND_STOP_ALL);
  KXL_PlaySound(SE_WARNING, KXL_SOUND_PLAY_LOOP);
}

/****************************
  ステージ1のボス移動
 ****************************/
RcHitEnum MoveBoss1(CharacterData *my)
{
  Uint16 i;

  /*座標計算*/
  if (my->Cnt1 < 100) {
    my->Y += 2;
    my->Cnt1 ++;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 0;
    my->AddY = 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  my->Y += my->AddY;

  /*アニメーション*/
  my->FrameNo = rand() % my->FrameMax;
  if (my->Cnt2 == 0) { /*画面内まで移動後、右に移動*/
    if (my->Y > AREA_LY) {
      my->AddX = 4;
      my->AddY = 0;
      my->Cnt2 ++;
    }
  } else if (my->Cnt2 == 1) { /*画面右まで移動後、下に移動*/
    if (my->X > AREA_RX - PixBoss[0]->Image->Width - 4) {
      my->AddX = 0;
      my->AddY = 4;
      my->Cnt2 ++;
    }
  } else if (my->Cnt2 == 2) { /*画面下まで移動後、左に移動*/
    if (my->Y > AREA_LY + 160) {
      my->AddX = -4;
      my->AddY = 0;
      my->Cnt2 ++;
    }
  } else if (my->Cnt2 == 3) { /*画面左まで移動後、上に移動*/
    if (my->X < AREA_LX + 4) {
      my->AddX = 0;
      my->AddY = -4;
      my->Cnt2 ++;
    }
  } else { /*画面上まで移動後、右に移動*/
    if (my->Y < AREA_LY + 4) {
      my->AddX = 4;
      my->AddY = 0;
      my->Cnt2 = 1;
    }
  }
  /*攻撃*/
  if (my->Cnt3 == 0) {
    if (rand() % (5 - Root->StageLoop) == 0) {
      for (i = 0; i < 2; i ++) {
	CreateEnemyShot1(my->X + 62 + i * 20,
			 my->Y + 225 - i * 3,
			 15, 16, 1);
	CreateEnemyShot1(my->X + 152 + i * 20,
			 my->Y + 222 + i * 3,
			 345, 16, 1);
      }
    }
    if (rand() % (7 - Root->StageLoop) == 0) {
      for (i = 0; i < 2; i ++) {
	CreateEnemyShot1(my->X + 15 + i * 10,
			 my->Y + 195,
			 350, 16, 1);
	CreateEnemyShot1(my->X + 212 + i * 10,
			 my->Y + 195,
			 10, 16, 1);
      }
    }
    if (rand() % (9 - Root->StageLoop * 2) == 0) {
      for (i = 0; i < 3; i ++)
	CreateEnemyShot2(my->X + 75 + i * 35,
			 my->Y + 65,
			 PixEnemyShot4,
			 3,
			 0,
			 16);
    }
    my->Cnt3 = 8 - Root->StageLoop;
  } else my->Cnt3 --;
  return RcHitNone;
}

/****************************
  ステージ2のボス移動
  左右に移動する
  時々変形して弾を巻き散らす
 ****************************/
RcHitEnum MoveBoss2(CharacterData *my)
{
  /*座標計算*/
  if (my->Cnt1 < 100) {
    my->Cnt1 ++;
    my->Y += 2;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 4 + rand() % 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);

  my->Cnt2 ++;
  if (my->Cnt2 % 10 == 0) {
    if (rand() % (my->Cnt3 ? 4 : 2) == 0) {
      CreateEnemyShot2(my->X + 70,
		       my->Y + 125,
		       PixEnemyShot2,
		       6,
		       GetDirection(my, &(Root->My[0]->Chr)),
		       16);
      CreateEnemyShot2(my->X + 125,
		       my->Y + 125,
		       PixEnemyShot2,
		       6,
		       GetDirection(my, &(Root->My[0]->Chr)),
		       16);
    }
  }
  if (my->Cnt2 % (80 - Root->StageLoop * 10) == 0 && rand() % 2) {
    my->Cnt3 = 1;
    my->Cnt4 = 0;
    my->AddX = 0;
    KXL_PlaySound(SE_OPEN, KXL_SOUND_PLAY);
  }
  if (my->Cnt3) {
    if (my->Cnt4 < 4) { /*砲台出現アニメーション*/
      my->FrameNo += (my->Cnt4 & 1);
      my->Cnt4 ++;
    } else if (my->Cnt4 == 4) {
      if (++ my->Cnt3 < 25) {
	CreateEnemyShot1(my->X + 45,
			 my->Y + 135,
			 90-(my->Cnt3 * 15),
			 6, 0);
	CreateEnemyShot1(my->X + 150,
			 my->Y + 135,
			 270+(my->Cnt3 * 15),
			 6, 0);
      } else
	my->Cnt4 ++;
    } else { /*砲台を隠すアニメーション*/
      if (my->Cnt4 < 8) {
	my->FrameNo -= (my->Cnt4 & 1);
	my->Cnt4 ++;
      } else {
	my->Cnt3 = 0;
	my->AddX = (rand() % 4) + (my->X < AREA_LX + DRAW_WIDTH / 2 ?
				   4 : -4);
      }
    }
  }
  return RcHitNone;
}

/****************************
  レーザー作成
 ****************************/
void CreateBoss3Laser(int x,int y)
{
  Cchr.Attr     = AttrEShot;
  Cchr.Target   = AttrMy;
  Cchr.Spr      = PixBoss3Laser;
  Cchr.FrameMax = 4;
  Cchr.X        = x - PixBoss3Laser[0]->Image->Width / 2;
  Cchr.Y        = y - PixBoss3Laser[0]->Image->Height / 2;
  Cchr.AddY     = 0;
  CopyYourNew(MoveBoss3Laser, HitNone, DrawPixmapFrame);
}

/****************************
  レーザー移動
 ****************************/
RcHitEnum MoveBoss3Laser(CharacterData *my)
{
  /*座標計算*/
  my->Y += my->AddY;
  if (my->Y > AREA_RY)
    return RcHitDel;
  /*アニメーション*/
  if (++ my->FrameNo == my->FrameMax)
    my->FrameNo = 0;
  if (++ my->Cnt1 == 50) /*画面下まで移動*/
    my->AddY = 32;
  return RcHitNone;
}

/****************************
  ステージ3のボス移動
 ****************************/
RcHitEnum MoveBoss3(CharacterData *my)
{
  Sint16 i;

  /*座標計算*/
  if (my->Cnt1 < 100) {
    my->Cnt1 ++;
    my->Y += 2;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 4 + rand() % 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);
  my->Cnt2 ++;
  if (my->Cnt2 % 20 == 0) {
    if (rand() % (my->Cnt3 ? 5 : 3) == 0) {
      for (i = -45; i <= 45; i += 15) {
	CreateEnemyShot2(my->X + 92,
			 my->Y + 36,
			 PixEnemyShot2,
			 6,
			 i,
			 12);
	CreateEnemyShot2(my->X + 209,
			 my->Y + 36,
			 PixEnemyShot2,
			 6,
			 i,
			 12);
      }
    }
  }
  if (my->Cnt3 == 0 && my->Cnt2 % 40 == 0) {
    if (rand() % 2) {
      my->Cnt3 = 1;
      my->Cnt4 = 0;
      my->AddX = 0;
      CreateBoss3Laser(my->X + 150,
		       my->Y + 115);
      for (i = 1; i < 3; i ++) {
	CreateBoss3Laser(my->X + 150 - i * 112,
			 my->Y + 115);
	CreateBoss3Laser(my->X + 150 + i * 112,
			 my->Y + 115);
      }
      KXL_PlaySound(SE_LASER, KXL_SOUND_PLAY);
    }
  }
  if (my->Cnt3) {
    if (++ my->Cnt4 > 80) {
      my->Cnt3 = 0;
      my->AddX = (rand() % 4) + (my->X < AREA_LX + DRAW_WIDTH / 2 ?
				 4 : -4);
    }
  }
  return RcHitNone;
}

/****************************
  ステージ4のボス移動
  時々突っ込む
 ****************************/
RcHitEnum MoveBoss4(CharacterData *my)
{
  Sint16 i;

  /*座標計算*/
  if (my->Cnt1 < 100) {
    my->Cnt1 ++;
    my->Y += 2;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 4 + rand() % 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  my->Y += my->AddY;
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);

  my->Cnt2 ++;
  /*攻撃*/
  if (rand() % (20 - Root->StageLoop * 2) == 0) {
    for (i = -1; i < 2; i ++) {
      CreateEnemyShot2(my->X + 5 + i * 8,
		       my->Y + 184,
		       PixEnemyShot2,
		       6,
		       0 + i * 8,
		       10);
      CreateEnemyShot2(my->X + 198 + i * 8,
		       my->Y + 184,
		       PixEnemyShot2,
		       6,
		       0 + i * 8,
		       10);
    }
  }
  if (my->Cnt3 == 0 && my->Cnt2 % 40 == 0) {
    if (rand() % 2) {
      KXL_PlaySound(SE_LASER, KXL_SOUND_PLAY);
      for (i = -40; i <= 40; i += 20) {
	CreateEnemyShot2(my->X + 74,
			 my->Y + 214,
			 PixEnemyShot5,
			 4,
			 i,
			 8);
	CreateEnemyShot2(my->X + 142,
			 my->Y + 214,
			 PixEnemyShot5,
			 4,
			 i,
			 8);
      }
      my->Cnt3 = 1;
      my->Cnt4 = 0;
      my->Cnt5 = 0;
      my->AddX = 0;
      my->AddY = 0;
    }
  }
  if (my->Cnt3) {
    if (my->Cnt5 == 0 && my->Cnt4 > 20) {
      KXL_PlaySound(SE_GO, KXL_SOUND_PLAY);
      my->AddY = 32;
      my->Cnt5 = 1;
    } else
      my->Cnt4 ++;
    if (my->Cnt5 == 1) {
      if (my->Y >= AREA_RY - PixBoss[0]->Image->Height - 50) {
	my->AddY = -24;
	my->Cnt5 = 2;
      }
    } else if (my->Cnt5 == 2) {
      if (my->Y < AREA_LY + 11) {
	my->Cnt3 = 0;
	my->AddX = (rand() % 4) + (my->X < AREA_LX + DRAW_WIDTH / 2 ?
				   4 : -4);
	my->AddY = 0;
      }
    }
  }
  return RcHitNone;
}

/****************************
  当たったら弾をまきちらす
 ****************************/
RcHitEnum HitEnemyShotBomb(CharacterData *my, CharacterData *your)
{
  Uint16 i;

  for (i = 0; i < 8; i ++)
    CreateEnemyShot1(my->X + my->Spr[0]->Image->Width / 2,
		     my->Y + my->Spr[0]->Image->Height / 2,
		     i * 45,
		     8, 0);
  KXL_PlaySound(SE_BOMB, KXL_SOUND_PLAY);
  return RcHitDel;
}

/****************************
  爆裂弾
 ****************************/
void CreateEnemyShotBomb(int x, int y, int direction, int speed)
{
  Sint16 ax, ay;

  Cchr.Attr     = AttrEBomb;
  Cchr.Target   = AttrMy | AttrMShot;
  Cchr.X        = x;
  Cchr.Y        = y;
  KXL_GetDirectionAdd(direction, &ax, &ay);
  Cchr.AddX     = ax * speed;
  Cchr.AddY     = ay * speed;
  Cchr.Spr      = PixEnemyShot5;
  Cchr.FrameMax = 4;
  CopyYourNew(MoveStandard, HitEnemyShotBomb, DrawPixmapFrame);
}

/****************************
  レーザー移動
 ****************************/
RcHitEnum MoveBoss5Laser(CharacterData *my)
{
  my->X = Root->Your[BOSS_NO]->Chr.X + 82;
  my->Y += 24;
  if (my->Y > PICT_HEIGHT)
    return RcHitDel;
  return RcHitNone;
}

/****************************
  レーザー作成
 ****************************/ 
void CreateBoss5Laser(void)
{
  Cchr.Attr     = AttrEShot;
  Cchr.Target   = AttrMy;
  Cchr.X        = Root->Your[BOSS_NO]->Chr.X + 82;
  Cchr.Y        = Root->Your[BOSS_NO]->Chr.Y + 182;
  Cchr.Spr      = PixBoss5Laser;
  CopyYourNew(MoveBoss5Laser, HitNone, DrawPixmapFrame);
  KXL_PlaySound(SE_LASER5, KXL_SOUND_PLAY);
}

/****************************
  ステージ5のボス移動
 ****************************/
RcHitEnum MoveBoss5(CharacterData *my)
{
  if (my->Cnt1 < 100) {
    my->Cnt1 ++;
    my->Y += 3;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 4;
    my->AddY = 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  my->Y += my->AddY;
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);
  if (my->Y < AREA_LY || my->Y > AREA_RY - 300)
    my->AddY = -(my->AddY);

  my->Cnt2 ++;
  /*攻撃*/
  if (rand() % 20 == 0) { /*爆裂弾発射*/
    CreateEnemyShotBomb(my->X + 130,
			my->Y + 215,
			(rand() % 360) - 90,
			4);
  }
  if (my->Cnt3 == 0 && my->Cnt2 % 100 == 0) { /* マジ攻撃*/
    if (my->Y < AREA_LY + 100) {
      my->Cnt3 = 1;
      my->Cnt4 = 0;
      my->Cnt5 = rand() % 2;
    }
  }
  
  if (my->Cnt3) {
    if (my->Cnt5 == 0) {
      CreateBoss5Laser();
    } else {
      CreateEnemyShot2(my->X + 43,
		       my->Y + 215,
		       PixEnemyShot2,
		       6,
		       my->Cnt4 * 15,
		       9);
      CreateEnemyShot2(my->X + 217,
		       my->Y + 215,
		       PixEnemyShot2,
		       6,
		       -my->Cnt4 * 15,
		       9);
    }
    if (++ my->Cnt4 > 24)
      my->Cnt3 = 0;
  }
    
  return RcHitNone;
}

/****************************
  ステージ6のボス移動
 ****************************/
RcHitEnum MoveBoss6(CharacterData *my)
{
  if (my->Cnt1 < 100) {
    my->Cnt1 ++;
    my->Y += 3;
    return RcHitNone;
  } else if (my->Cnt1 == 100) {
    my->AddX = 4;
    my->AddY = 4;
    my->Cnt1 ++;
  }
  my->X += my->AddX;
  my->Y += my->AddY;
  if (my->X < AREA_LX || my->X > AREA_RX - my->Spr[0]->Image->Width)
    my->AddX = -(my->AddX);
  if (my->Y < AREA_LY || my->Y > AREA_RY - 300)
    my->AddY = -(my->AddY);

  my->Cnt2 ++;
  /*攻撃*/
  if (rand() % 15 == 0) { /*爆裂弾発射*/
    CreateEnemyShotBomb(my->X + 130,
			my->Y + 215,
			(rand() % 360) - 90,
			4);
  }
  if (my->Cnt3 == 0 && my->Cnt2 % 80 == 0) { /* マジ攻撃*/
    if (my->Y < AREA_LY + 100) {
      my->Cnt3 = 1;
      my->Cnt4 = 0;
      my->Cnt5 = rand() % 2;
    }
  }
  
  if (my->Cnt3) {
    if (my->Cnt5 == 0) {
      CreateBoss5Laser();
    } else {
      CreateEnemyShot2(my->X + 43,
		       my->Y + 215,
		       PixEnemyShot2,
		       6,
		       my->Cnt4 * 15,
		       9);
      CreateEnemyShot2(my->X + 217,
		       my->Y + 215,
		       PixEnemyShot2,
		       6,
		       -my->Cnt4 * 15,
		       9);
    }
    if (++ my->Cnt4 > 24)
      my->Cnt3 = 0;
  }
    
  return RcHitNone;
}

