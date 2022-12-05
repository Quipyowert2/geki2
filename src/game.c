#include "geki2.h"
#include "extern.h"

/****************************
  自分データ削除
 ****************************/
Bool DeleteMyData(int m)
{
  if (Root->My[m]->Chr.Active == True) {
    Root->My[m]->Chr.Active = False;
    Root->MyNo --;
    return True;
  }
  return False;
}

/****************************
  敵データ削除
 ****************************/
Bool DeleteYourData(int y)
{
  if (Root->Your[y]->Chr.Active == True) {
    Root->Your[y]->Chr.Active = False;
    Root->YourNo --;
    return True;
  }
  return False;
}

/****************************
  自分データ作成
 ****************************/
Sint16 CopyMyNew(RcHitEnum (*act)(CharacterData *my),
              RcHitEnum (*hit)(CharacterData *my, CharacterData *your),
              void (*re)(CharacterData *my))
{
  Uint16 i;
  
  if (Root->MyNo >= MAX_MY)
    return -1;
  for (i = 1; i < MAX_MY; i ++) {
    if (Root->My[i]->Chr.Active == False) {
      Root->My[i]->Chr         = Cchr;
      Root->My[i]->Chr.CntX    = Root->My[i]->Chr.X << 8;
      Root->My[i]->Chr.CntY    = Root->My[i]->Chr.Y << 8;
      Root->My[i]->Move        = act;
      Root->My[i]->Hit         = hit;
      Root->My[i]->Draw        = re;
      Root->My[i]->Chr.Active  = True;
      Root->My[i]->Chr.FrameNo =
      Root->My[i]->Chr.Cnt1    =
      Root->My[i]->Chr.Cnt2    = 0;
      Root->MyNo ++;
      return i;
    }
  }
  return -1;
}

/****************************
  敵データ作成
 ****************************/
Sint16 CopyYourNew(RcHitEnum (*act)(CharacterData *my),
                RcHitEnum (*hit)(CharacterData *my, CharacterData *your),
                void (*re)(CharacterData *my))
{
  Uint16 i;
  Uint16 st = 41;
  Uint16 ed = 71;
  
  if (Root->YourNo >= MAX_YOUR)
    return -1;
  if (Cchr.Attr == AttrEnemy) {
    /** 地上の敵 **/
    st = 0;
    ed = st + 10;
  } else if (Cchr.Attr & AttrShadow) {
    /** 空中の敵 **/
    st = 11;
    ed = st + 30;
  } else if (Cchr.Attr & AttrEShot) {
    /** 爆発 **/
    st = 71;
    ed = MAX_YOUR;
  }
  for (i = st; i < ed; i ++) {
    if (Root->Your[i]->Chr.Active == False) {
      Root->Your[i]->Chr         = Cchr;
      Root->Your[i]->Chr.CntX    = Root->Your[i]->Chr.X << 8;
      Root->Your[i]->Chr.CntY    = Root->Your[i]->Chr.Y << 8;
      Root->Your[i]->Move        = act;
      Root->Your[i]->Hit         = hit;
      Root->Your[i]->Draw        = re;
      Root->Your[i]->Chr.Active  = True;
      Root->Your[i]->Chr.FrameNo =
      Root->Your[i]->Chr.Cnt1    =
      Root->Your[i]->Chr.Cnt2    = 0;
      Root->YourNo ++;
      return i;
    }
  }
  return -1;
}

/****************************
  敵データ削除
 ****************************/
void DeleteAll(void)
{
  Uint16 i;

  for (i = 0; i < MAX_YOUR; i ++) {
    if (Root->Your[i]->Chr.Active == False || i == BOSS_NO)
      continue;
    Root->Your[i]->Chr.Attr    =
    Root->Your[i]->Chr.Target  = AttrNone;
    Root->Your[i]->Chr.Spr     = PixBomb1;
    Root->Your[i]->Chr.Cnt1    =
    Root->Your[i]->Chr.FrameNo = 0;
    Root->Your[i]->Move        = MoveBomb;
    Root->Your[i]->Hit         = HitNone;
    Root->Your[i]->Draw        = DrawPixmapFrame;
  }
}

/****************************
  当り判定
 ****************************/
Bool Check(CharacterData *my, CharacterData *your)
{
  PixData *m = my->Spr[my->FrameNo];
  PixData *y = your->Spr[your->FrameNo];
  KXL_Rect mm, yy;
  
  mm.Left   = my->X + m->r.Left;
  mm.Top    = my->Y + m->r.Top;
  mm.Width  = m->r.Width;
  mm.Height = m->r.Height;
  yy.Left   = your->X + y->r.Left;
  yy.Top    = your->Y + y->r.Top;
  yy.Width  = y->r.Width - 1;
  yy.Height = y->r.Height - 1;
  return KXL_RectIntersect(mm, yy);
}

/****************************
  ゲームメイン
 ****************************/
void Game(void)
{
  Sint16 i, j, m, y;
  RcHitEnum rc;
  
  /** 背景スクロール **/
  for (i = 0; i < 13; i++)
    for (j = 0; j < 9; j++)
      DrawPixmap(PixBack[MapDatas[Root->MapNo+i]->no[j]],
                 AREA_LX + j * 50,
                 AREA_RY - 50 - i * 50 + Root->ScrollCnt);
  if (Root->MapNo < Root->MapMax - 14) {
    if (++ Root->ScrollCnt == 50) {
      Root->MapNo ++;
      Root->ScrollCnt = 0;
    }
  }
  /** 敵等発生 **/
  for (i = 0; i < Root->StageMax; i ++) {
    if (Root->EnemyCnt == StageDatas[i]->Time && !StageDatas[i]->Flag) {
      StageDatas[i]->Flag = 1;
    }
  }
  for (i = 0; i < Root->StageMax; i ++) {
    if (StageDatas[i]->Flag) {
      if (StageDatas[i]->Max) {
	if (!StageDatas[i]->StepTime) {
	  StageDatas[i]->Max --;
	  StageDatas[i]->StepTime = StageDatas[i]->Step;
	  switch (StageDatas[i]->CreateNo) {
	  case 0: /* ワーニング出現 */
            CreateWarning();
            break;
	  case 1: /* ザコ 左上から右下へ */
	    CreateZako(AREA_LX,
                       30,
                       8 + Root->StageLoop);
	    break;
	  case 2: /* ザコ 右上から左下 */
	    CreateZako(AREA_RX - PixZako[0]->Image->Width,
                       330,
                       8 + Root->StageLoop);
	    break;
	  case 3: /* ザコ 上から下へ */
	    CreateZako(AREA_LX + 50 + rand() % 300,
                       0,
                       12 + Root->StageLoop+(rand() % 4));
	    break;
	  case 4: /* ユラユラ 左上から左下へ */
	    CreateUraura(AREA_LX + rand() % 150,
                         0);
	    break;
	  case 5: /* ユラユラ 右上から右下へ */
	    CreateUraura(AREA_RX - 50 - rand() % 150,
                         1);
	    break;
	  case 6: /* ジワ 左右から */
	    for (j = 0; j < 5; j ++) {
	      CreateJiwa(AREA_LX + j * PixJiwa[0]->Image->Width,
                         AREA_LY + j * PixJiwa[0]->Image->Height,
                         10);
	      CreateJiwa(AREA_RX - (j + 1) * PixJiwa[0]->Image->Width,
                         AREA_LY + j * PixJiwa[0]->Image->Height,
                         -10);
	    }
	    break;
	  case 7: /* 砲台 */
	    CreateHoudai(AREA_LX + 30 + (rand() % 4) * 90,
                         0,
                         1 + Root->StageLoop + (rand() % 1));
	    break;
	  case 8: /* Uターン */
	    CreateUturn(AREA_LX + 50 + rand() % 300);
	    break;
	  case 9: /* くるくる */
	    CreateKuru();
	    break;
	  case 10: /* 中ボス */
	    CreateMiddleBoss();
	    break;
	  case 11: /* ちょろ */
	    CreateChoro(AREA_LX + 50 + rand() % 300);
	    break;
	  case 90: /* アイテム */
	    CreateTank();
	    break;
	  }
	} else StageDatas[i]->StepTime --;
      } else StageDatas[i]->Flag = 0;
    }
  }
  Root->EnemyCnt ++;
  /** 自分移動 **/
  for (m = 0; m < MAX_MY; m ++)
    if (Root->My[m]->Chr.Active == True)
      if ((rc = Root->My[m]->Move(&(Root->My[m]->Chr))) == RcHitDel)
        DeleteMyData(m);
  /** 敵移動 **/
  for (y = 0; y < MAX_YOUR; y ++) {
    if (Root->Your[y]->Chr.Active == False)
      continue;
    rc = Root->Your[y]->Move(&(Root->Your[y]->Chr));
    switch (rc) {
    case RcHitDel:
      DeleteYourData(y);
      break;
    case RcHitBoss:
      DeleteYourData(y);
      Root->Cnt = 0;
      Root->MainFlag = MainClear;
      return;
      break;
    }
  }
  /** 当り判定 **/
  for (m = 0; m < MAX_MY; m ++) {
    if (Root->My[m]->Chr.Active == False)
      continue;
    for (y = 0; y < MAX_YOUR; y ++) {
      if (Root->Your[y]->Chr.Active == False)
        continue;
      if (Root->My[m]->Chr.Target & Root->Your[y]->Chr.Attr) {
        if (Check(&(Root->My[m]->Chr), &(Root->Your[y]->Chr)) == False)
          continue;
        if (Root->My[m]->Hit(&(Root->My[m]->Chr), &(Root->Your[y]->Chr)) == RcHitDel)
          DeleteMyData(m);
        rc = Root->Your[y]->Hit(&(Root->Your[y]->Chr), &(Root->My[m]->Chr));
        switch (rc) {
        case RcHitBomb:
          Root->Score += Root->Your[y]->Chr.Score;
        case RcHitDel:
          DeleteYourData(y);
          break;
        default:
          break;
        }
      }
    }
  }
  /** 相手の影移動&描画 **/
  for (y = 0; y < MAX_YOUR; y ++)
   if (Root->Your[y]->Chr.Active == True && Root->Your[y]->Chr.Attr & AttrShadow)
     DrawPixmap(Root->Your[y]->Chr.Spr[Root->Your[y]->Chr.FrameMax + Root->Your[y]->Chr.FrameNo],
                Root->Your[y]->Chr.X + Root->Your[y]->Chr.Spr[0]->Image->Width,
                Root->Your[y]->Chr.Y + Root->Your[y]->Chr.Spr[0]->Image->Height / 2);
  /** 自分の影移動&描画 **/
  if (Root->My[0]->Chr.Attr & AttrShadow)
    DrawPixmap(Root->My[0]->Chr.Spr[Root->My[0]->Chr.FrameMax + Root->My[0]->Chr.FrameNo],
               Root->My[0]->Chr.X + Root->My[0]->Chr.Spr[0]->Image->Width,
               Root->My[0]->Chr.Y + Root->My[0]->Chr.Spr[0]->Image->Height / 2);
  /** 相手描画 **/
  for (y = 0; y < MAX_YOUR; y ++)
   if (Root->Your[y]->Chr.Active == True)
     Root->Your[y]->Draw(&(Root->Your[y]->Chr));
  /** 自分描画 **/
  for (m = MAX_MY - 1; m >= 0; m --)
    if (Root->My[m]->Chr.Active == True)
      Root->My[m]->Draw(&(Root->My[m]->Chr));
  /* スコアチェック */
  if (Root->Score >= Root->OneUp) {
    Root->MyMax ++;
    Root->OneUp += 10000;
  }
  /*インフォメーション描画*/
  Infomation();
}
