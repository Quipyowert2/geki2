#ifndef _EXTERN_H_
#define _EXTERN_H_

#include "opening.h"
#include "game.h"
#include "load.h"
#include "my.h"
#include "your.h"
#include "boss.h"
#include "ranking.h"
#include "misc.h"

#ifdef _EXTERN_DEF_
#define EXTERN 
#else
#define EXTERN extern
#endif

EXTERN CharacterData Cchr;          /**                **/
EXTERN RootData *Root;              /** root data **/
EXTERN RankingData **Ranking;        /** ranking data **/
EXTERN StageData **StageDatas;
EXTERN MapData **MapDatas;          /** map data **/
EXTERN PixData **PixMy;             /** my **/
EXTERN PixData **PixMyShot;         /** my shot **/
EXTERN PixData **PixItem;           /** item **/
EXTERN PixData **PixMyLaser;        /** my laser **/
EXTERN PixData **PixItemShot;       /** shot item **/
EXTERN PixData **PixItemLaser;      /** laser item **/
EXTERN PixData **PixZako;           /** zako    **/
EXTERN PixData **PixKuru;           /** kuru **/
EXTERN PixData **PixHoudai;         /** houdai **/
EXTERN PixData **PixUraUra;         /** uraura **/
EXTERN PixData **PixUturn;          /** uturn **/
EXTERN PixData **PixJiwa;           /** jiwa **/
EXTERN PixData **PixChoro;          /** choro **/
EXTERN PixData **PixMboss;          /** middle boss 8 **/
EXTERN PixData **PixEnemyShot1;     /** enemy shot 1 **/
EXTERN PixData **PixEnemyShot2;     /** enemy shot 2 **/
EXTERN PixData **PixEnemyShot3;     /** enemy shot 3 **/
EXTERN PixData **PixEnemyShot4;     /** enemy shot 4 **/
EXTERN PixData **PixEnemyShot5;     /** enemy shot 5 **/
EXTERN PixData **PixBomb1;          /** bomb 1 **/
EXTERN PixData **PixBomb2;          /** bomb 2 **/
EXTERN PixData **PixBack;           /** back pixmap **/
EXTERN PixData **PixBoss;           /** boss **/
EXTERN PixData **PixFlash;          /** flash **/
EXTERN PixData **PixBoss3Laser;     /** boss3 laser **/
EXTERN PixData **PixBoss5Laser;     /** boss5 laser **/
EXTERN PixData **PixWarning;        /** warning     **/
#endif
