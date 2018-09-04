/*
**  Geki2
**  Copyright (C) 1999-2001, kacchan
**  E-M@il address   fc3srx@mwnet.or.jp
**  Homepage address http://www2.mwnet.or.jp/~fc3srx7
*/
#ifndef _GEKI2_H_
#define _GEKI2_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <KXL.h>

#define FRAME_RATE 40

#define KeyUp    KXL_KEY_Up
#define KeyDown  KXL_KEY_Down
#define KeyRight KXL_KEY_Right
#define KeyLeft  KXL_KEY_Left
#define KeyShot  KXL_KEY_z
#define KeyPause KXL_KEY_s

/** キーマスク **/
#define KNone      0
#define KUp        (1L << 0)
#define KDown      (1L << 1)
#define KRight     (1L << 2)
#define KLeft      (1L << 3)
#define KShot      (1L << 4)
#define KPause     (1L << 5)

#define KUpMask    ~KUp
#define KDownMask  ~KDown
#define KRightMask ~KRight
#define KLeftMask  ~KLeft
#define KShotMask  ~KShot
#define KPauseMask ~KPause

/** キャラクタの識別子**/
#define AttrNone     0
#define AttrMy       (1L << 0)
#define AttrMShot    (1L << 1)
#define AttrEnemy    (1L << 2)
#define AttrEShot    (1L << 3)
#define AttrItem     (1L << 4)
#define AttrShadow   (1L << 5)
#define AttrEBomb    (1L << 6)

/** ウィンドウサイズ **/
#define DRAW_WIDTH   450
#define DRAW_HEIGHT  600
#define FREE_AREA    60
#define PICT_WIDTH   (DRAW_WIDTH + FREE_AREA * 2)
#define PICT_HEIGHT  (DRAW_HEIGHT + FREE_AREA * 2)
#define AREA_LX      FREE_AREA
#define AREA_LY      FREE_AREA
#define AREA_RX      (PICT_WIDTH - FREE_AREA)
#define AREA_RY      (PICT_HEIGHT - FREE_AREA)
#define CHECK_PER    0.7

/** 各種最大値 **/
#define MAX_YOUR    148 /** 敵&敵弾 **/
#define MAX_MY      48  /** プレイヤー&プレイヤー弾 **/
#define BOSS_NO     10

/** サウンド **/
#define SE_BGM1     0
#define SE_BGM2     1
#define SE_BGM3     2
#define SE_BGM4     3
#define SE_BGM5     4
#define SE_BGM6     5
#define SE_BOMB     6
#define SE_WARNING  7
#define SE_BOSS     8
#define SE_OPEN     9
#define SE_LASER    10
#define SE_POW      11
#define SE_GO       12
#define SE_LASER5   13

/** メイン動作 **/
typedef enum {
  MainOpening,
  MainGame,
  MainClear,
  MainGameOver,
  MainEnding,
  MainPause
} MainEnum;

/*武器種別*/
typedef enum {
  WeaponShot,
  WeaponLaser
} WeaponEnum;

/*移動後の戻り値*/
typedef enum {
  RcHitNone,
  RcHitDel,
  RcHitBomb,
  RcHitBoss
} RcHitEnum;

/*ゲームモード*/
typedef enum {
  ModeEasy,
  ModeNormal,
  ModeHard
} ModeEnum;

/** ピックス情報 **/
typedef struct {
  KXL_Image *Image;
  KXL_Rect r;
} PixData;

/** キャラクタデータ **/
typedef struct {
  Bool   Active;                  /** 表示可? **/
  Uint16 Attr;                    /** 識別子 **/
  Uint16 Target;                  /** ターゲット **/
  Sint16 Cnt1;                    /**  **/
  Sint16 Cnt2;                    /**  **/
  Sint16 Cnt3;                    /**  **/
  Sint16 Cnt4;                    /**  **/
  Sint16 Cnt5;                    /**  **/
  Sint16 Etc;
  Sint16 Hp;                      /**  **/
  Uint32 Score;                   /** 点数 **/
  Sint16 X, Y;                    /** スプライト座標 **/
  Uint32 CntX, CntY;              /** スプライト移動量 **/
  Sint16 AddX, AddY;              /** スプライト移動量加算値 **/
  Uint8  FrameNo, FrameMax;       /** スプライトフレーム数 **/
  PixData **Spr;
} CharacterData;

/** 汎用キャラクタデータ **/
typedef struct {
  CharacterData Chr;
  void (*Draw)(CharacterData *my);
  RcHitEnum (*Move)(CharacterData *my);
  RcHitEnum (*Hit)(CharacterData *my, CharacterData *your);
} CharacterObject;

/** 管理用 **/
typedef struct {
  Uint8 StageMax;
  Uint16 ScrollCnt;
  Uint16 MapMax;
  Uint8 MapNo;
  Uint16 EnemyCnt;
  MainEnum MainFlag;
  Uint16 Key;
  Uint8 MyNo;
  Uint8 YourNo;
  Uint8 Stage;
  Uint8 StageLoop;
  Uint32 Score;
  Uint32   HiScore;     /*ハイスコア*/
  Uint32 OneUp;
  Sint8 MyMax;
  Uint8 MyPower;
  Bool MyFlash;
  WeaponEnum MyWeapon;
  Sint16 Cnt;
  Bool WaitFlag;
  ModeEnum GameMode;
  CharacterObject **My;
  CharacterObject **Your;
} RootData;

/*ランキング*/
typedef struct {
  Uint32 Score;
  Uint8 Stage;
  Uint8 Loop;
  Uint8 Name[16];
} RankingData;

/*ステージデータ*/
typedef struct {
  Uint16 Time;         /* 敵出現時間 */ 
  Uint8  CreateNo;     /* 敵No. */
  Uint8  Max;          /* 敵出現回数 */
  Uint16 Step;         /* 敵出現間隔 */
  Uint16 StepTime;
  Bool Flag;
} StageData;

typedef struct {
  Uint8 no[9];
} MapData;

#endif
