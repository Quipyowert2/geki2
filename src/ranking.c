#include <pwd.h>
#include "geki2.h"
#include "extern.h"

/**********************
  スコアランキング
 **********************/
int ScoreRanking(void)
{
  Uint16 i, j, k;
  struct passwd *pw;
	
  for (i = 0; i < 5; i ++)
    if (Ranking[i]->Score < Root->Score)
      break;
  if (i == 5)
    return;
  if (i != 4) {
    for (j = 4; j > i; j --)
      memcpy(Ranking[j], Ranking[j - 1], sizeof(RankingData));
  }
  Ranking[i]->Score = Root->Score;
  Ranking[i]->Stage = Root->Stage;
  Ranking[i]->Loop  = Root->StageLoop;
  if (!(pw = getpwuid(getuid())))
    sprintf(Ranking[i]->Name, "%d", getuid());
  else
    strcpy(Ranking[i]->Name, pw->pw_name);
  WriteScore();
}

/**********************
  スコア読み込み
 **********************/
void ReadScore(void)
{
  FILE *fp;
  Uint16 i;

  if ((fp = fopen(DATA_PATH "/.score", "r"))) {
    fscanf(fp, "%d", &(Root->HiScore));
    for (i = 0; i < 5; i ++)
      fscanf(fp, "%d %d %d %s",
	     &(Ranking[i]->Score),
	     &(Ranking[i]->Stage),
	     &(Ranking[i]->Loop),
	     Ranking[i]->Name);
    fclose(fp);
  } else {
    Root->HiScore = 10000;
    for (i = 0; i < 4; i ++) {
      Ranking[i]->Score = 10000 - i * 1000;
      Ranking[i]->Stage = 1;
      Ranking[i]->Loop = 0;
      sprintf(Ranking[i]->Name, "hoge%d", i + 1);
    }
    Ranking[4]->Score = 3000;
    Ranking[4]->Stage = 0;
    Ranking[4]->Loop  = 0;
    sprintf(Ranking[4]->Name, "hoge5");
  }
}

/**********************
  スコア書き込み
 **********************/
void WriteScore(void)
{
  FILE *fp;
  Uint16 i;

  if ((fp = fopen(DATA_PATH "/.score", "w"))) {
    fprintf(fp, "%d\n", Root->HiScore);
    for (i = 0; i < 5; i ++)
      fprintf(fp, "%d %d %d %s\n",
	      Ranking[i]->Score,
	      Ranking[i]->Stage,
	      Ranking[i]->Loop,
	      Ranking[i]->Name);
    fclose(fp);
  }
}
