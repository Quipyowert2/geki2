/***********************************************************************
   Filename    :  utu.c
   Description :  
 ***********************************************************************/
#include "geki2.h"
#define _EXTERN_DEF_
#include "extern.h"

/****************************
  メインループ(タイマ割込み)
 ****************************/
void MainLoop(void)
{
  KXL_Rect rect={AREA_LX, AREA_LY, DRAW_WIDTH, DRAW_HEIGHT};

  while (1) {
    /*タイマ割込み待ち*/
    if (KXL_GetTimer() == False)
      continue;
    KXL_ResetTimer();
    /*Ｘイベント処理*/
    while (KXL_CheckEvents()) {
      switch (KXL_GetEvents()) {
      case KXL_EVENT_KEY_PRESS: /*キー押した*/
        switch (KXL_GetKey()) {
        case KeyShot:  Root->Key |= KShot;  break;
        case KeyLeft:  Root->Key |= KLeft;  break;
        case KeyRight: Root->Key |= KRight; break;
        case KeyUp:    Root->Key |= KUp;    break;
        case KeyDown:  Root->Key |= KDown;  break;
        case KeyPause: Root->Key |= KPause; break;
        default:                            break;
        }
        break;
      case KXL_EVENT_KEY_RELEASE: /*キー離した*/
        switch (KXL_GetKey()) {
        case KeyShot:  Root->Key &= KShotMask;  break;
        case KeyLeft:  Root->Key &= KLeftMask;  break;
        case KeyRight: Root->Key &= KRightMask; break;
        case KeyUp:    Root->Key &= KUpMask;    break;
        case KeyDown:  Root->Key &= KDownMask;  break;
        case KeyPause: Root->Key &= KPauseMask; break;
        default:                                break;
        }
        break;
      default:
        break;
      }
    }
    /** メインフラグチェック **/
    switch (Root->MainFlag) {
    case MainPause: /*ポーズ*/
      Root->MainFlag = (Root->Key & KShot) ? MainGame : MainPause;
      break;
    case MainOpening: /*オープニング*/
      if (Opening() == True)
        return;
      break;
    case MainGame: /*プレイ*/
      Game();
      break;
    case MainClear: /*クリア*/
      Game();
      ClearAndGameOver();
      break;
    case MainGameOver: /*ゲームオーバー*/
      Game();
      ClearAndGameOver();
      break;
    case MainEnding:
      break;
    }
    KXL_UpDate(rect);
  }
}

/****************************
  ルート領域確保&初期値設定
 ****************************/
void CreateRoot(void)
{
  Uint16 i;

  /** ルート領域確保 **/
  Root = (RootData *)KXL_Malloc(sizeof(RootData));
  Root->My = (CharacterObject **)KXL_Malloc(sizeof(CharacterObject *) * MAX_MY);
  for (i = 0; i < MAX_MY; i ++)
    Root->My[i] = (CharacterObject *)KXL_Malloc(sizeof(CharacterObject));
  Root->Your = (CharacterObject **)KXL_Malloc(sizeof(CharacterObject *) * MAX_YOUR);
  for (i = 0; i < MAX_YOUR; i ++)
    Root->Your[i] = (CharacterObject *)KXL_Malloc(sizeof(CharacterObject));
  Ranking = (RankingData **)KXL_Malloc(sizeof(RankingData *) * 5);
  for (i = 0; i < 5; i ++)
    Ranking[i] = (RankingData *)KXL_Malloc(sizeof(RankingData));
  /** ウィンドウ作成 **/
  KXL_CreateWindow(DRAW_WIDTH,
		   DRAW_HEIGHT,
		   TITLE,
		   KXL_EVENT_KEY_PRESS_MASK |
		   KXL_EVENT_KEY_RELEASE_MASK);
  KXL_ReSizeFrame(PICT_WIDTH, PICT_HEIGHT);
  /** ルート初期値設定 **/
  Root->MainFlag = MainOpening;
  ReadScore();
  Root->Key = KNone;
  Root->Stage =
  Root->Cnt = 0;
  Root->WaitFlag = True;
}

/****************************
  ルート領域解放
 ****************************/
void DeleteRoot(void)
{
  Uint16 i;
  
  KXL_DeleteWindow();
  for (i = 0; i < MAX_MY; i ++)
    free(Root->My[i]);
  for (i = 0; i < MAX_YOUR; i ++)
    free(Root->Your[i]);
  free(Root);
  for (i = 0; i < 5; i ++)
    KXL_Free(Ranking[i]);
  KXL_Free(Ranking);
}

/****************************
      オプション
 ****************************/
void options(int argc, char **argv)
{
  Uint16 i;

  fprintf(stderr, "url   : http://www2.mwnet.or.jp/~fc3srx7\n");
  fprintf(stderr, "email : fc3srx7@mwnet.or.jp\n\n");
  for (i = 1; i < argc; i ++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      fprintf(stderr, "\nUsage: geki2 [--display display] [--version] [--help]\n");
      fprintf(stderr, "--display display : Use display as the target display\n");
      fprintf(stderr, "--version         : Show version number and exit\n");
      fprintf(stderr, "--help            : Show this message\n");
      exit(1);
    }
  }
  fprintf(stderr, "\"geki2 --help\" for command line parameters\n");
  for (i = 1; i < argc; i ++) {
    if (!strcmp(argv[i], "--display")) {
      fprintf(stderr, "Using display: %s\n", argv[i + 1]);
      KXL_DisplayName(argv[i + 1]);
    }
    if (!strcmp(argv[i], "--version")) {
      fprintf(stderr, "Version is " TITLE "\n");
      exit(1);
    }
  }
}

/****************************
      メインルーチン
 ****************************/
int main(int argc, char **argv)
{
  Uint8 *sname[]={
    "bgm1",
    "bgm2",
    "bgm3",
    "bgm4",
    "bgm5",
    "bgm6",
    "bomb",
    "warning",
    "boss",
    "open",
    "laser",
    "pow",
    "go",
    "laser5",
    ""};

  options(argc, argv);
  srand(time(NULL));
  CreateRoot();
  CreatePixmap();
  KXL_InitSound(WAV_PATH, sname);
  KXL_Timer(FRAME_RATE);
  MainLoop();
  KXL_EndSound();
  DeletePixmap();
  DeleteRoot();
  return 0;
}
