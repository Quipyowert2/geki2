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

/** �����ޥ��� **/
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

/** ����饯���μ��̻�**/
#define AttrNone     0
#define AttrMy       (1L << 0)
#define AttrMShot    (1L << 1)
#define AttrEnemy    (1L << 2)
#define AttrEShot    (1L << 3)
#define AttrItem     (1L << 4)
#define AttrShadow   (1L << 5)
#define AttrEBomb    (1L << 6)

/** ������ɥ������� **/
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

/** �Ƽ������ **/
#define MAX_YOUR    148 /** Ũ&Ũ�� **/
#define MAX_MY      48  /** �ץ쥤�䡼&�ץ쥤�䡼�� **/
#define BOSS_NO     10

/** ������� **/
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

/** �ᥤ��ư�� **/
typedef enum {
  MainOpening,
  MainGame,
  MainClear,
  MainGameOver,
  MainEnding,
  MainPause
} MainEnum;

/*������*/
typedef enum {
  WeaponShot,
  WeaponLaser
} WeaponEnum;

/*��ư��������*/
typedef enum {
  RcHitNone,
  RcHitDel,
  RcHitBomb,
  RcHitBoss
} RcHitEnum;

/*������⡼��*/
typedef enum {
  ModeEasy,
  ModeNormal,
  ModeHard
} ModeEnum;

/** �ԥå������� **/
typedef struct {
  KXL_Image *Image;
  KXL_Rect r;
} PixData;

/** ����饯���ǡ��� **/
typedef struct {
  Bool   Active;                  /** ɽ����? **/
  Uint16 Attr;                    /** ���̻� **/
  Uint16 Target;                  /** �������å� **/
  Sint16 Cnt1;                    /**  **/
  Sint16 Cnt2;                    /**  **/
  Sint16 Cnt3;                    /**  **/
  Sint16 Cnt4;                    /**  **/
  Sint16 Cnt5;                    /**  **/
  Sint16 Etc;
  Sint16 Hp;                      /**  **/
  Uint32 Score;                   /** ���� **/
  Sint16 X, Y;                    /** ���ץ饤�Ⱥ�ɸ **/
  Uint32 CntX, CntY;              /** ���ץ饤�Ȱ�ư�� **/
  Sint16 AddX, AddY;              /** ���ץ饤�Ȱ�ư�̲û��� **/
  Uint8  FrameNo, FrameMax;       /** ���ץ饤�ȥե졼��� **/
  PixData **Spr;
} CharacterData;

/** ���ѥ���饯���ǡ��� **/
typedef struct {
  CharacterData Chr;
  void (*Draw)(CharacterData *my);
  RcHitEnum (*Move)(CharacterData *my);
  RcHitEnum (*Hit)(CharacterData *my, CharacterData *your);
} CharacterObject;

/** ������ **/
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
  Uint32   HiScore;     /*�ϥ�������*/
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

/*��󥭥�*/
typedef struct {
  Uint32 Score;
  Uint8 Stage;
  Uint8 Loop;
  Uint8 Name[16];
} RankingData;

/*���ơ����ǡ���*/
typedef struct {
  Uint16 Time;         /* Ũ�и����� */ 
  Uint8  CreateNo;     /* ŨNo. */
  Uint8  Max;          /* Ũ�и���� */
  Uint16 Step;         /* Ũ�и��ֳ� */
  Uint16 StepTime;
  Bool Flag;
} StageData;

typedef struct {
  Uint8 no[9];
} MapData;

#endif
