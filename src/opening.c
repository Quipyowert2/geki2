#include "geki2.h"
#include "extern.h"

#define OP_BASE (AREA_LX+170)
#define CENTER(str) (AREA_LX + (DRAW_WIDTH - KXL_TextWidth((str))) / 2)
#define TO_MENU "push any key to menu"
Uint8 *title[]={
  " 000                000 ",
  "0   0       0    0 0   0",
  "0   0  000  0  0       0",
  "0     0   0 0 0  0    0 ",
  "0 000 00000 00   0   0  ",
  "0   0 0     0 0  0 0    ",
  " 0000  000  0  0 0 00000",
  ""
};
char *menu_str[]={
  "Game Start",
  "Key Operation",
  "Items",
  "Ranking",
  "Exit",
  " ",
  " ",
  " ",
  "move   - Cursor Up or Down",
  "select - z                ",
  ""
};
char *key_str[] = {
  "Key Operation",
  " ",
  "Up    - Cursor Up   ",
  "Down  - Cursor Down ",
  "Right - Cursor Right",
  "Left  - Cursor Left ",
  "Shot  - z           ",
  "Pause - s           ",
  ""
};
char *item_str[]={
  "An item is carried.",
  "Power up to shot.  ",
  "Power up to laser. ",
  ""
};
Uint8 menu = 0;
Uint8 sel = 0;

/**********************
  オープニング
 **********************/
Bool Opening(void)
{
  static Uint16 OldKey = KNone;
  Uint16 i, j;
  Uint8 buff[65];

  /* title */
  KXL_Rect rect = {AREA_LX, AREA_LY, DRAW_WIDTH, DRAW_HEIGHT};
  KXL_ClearFrame(rect);
  for (i = 0; title[i][0]; i ++)
    for (j = 0; j < 24; j ++)
      if (title[i][j] != ' ')
	KXL_PutImage(PixBomb1[Root->Cnt % 13]->Image,
		     AREA_LX + 24 + 16 * j,
		     AREA_LY + 8 + 14 * i);
  Root->Cnt ++;
  if (Root->Cnt == 13)
    Root->Cnt = 0;

  if (menu == 0) { /* menu */
    for (i = 0; menu_str[i][0]; i ++) {
      if (i == sel)
	KXL_Font(NULL, 0xff, 0x00, 0x00);
      else
	KXL_Font(NULL, 0xff, 0xff, 0xff);
      KXL_PutText(CENTER(menu_str[i]), AREA_LY + 180 + i * 16, menu_str[i]);
    }
  } else if (menu == 1) { /* key operation */
    for (i = 0; key_str[i][0]; i ++) {
      KXL_Font(NULL, 0xff, 0xff, i == 0 ? 0x00 : 0xff);
      KXL_PutText(CENTER(key_str[i]), AREA_LY + 148 + i * 16, key_str[i]);
    }
    KXL_Font(NULL, 0x00, 0xff, 0xff);
    KXL_PutText(CENTER(TO_MENU), AREA_LY + 320, TO_MENU);
  } else if (menu == 2) { /* items */
    KXL_Font(NULL, 0xff, 0xff, 0x00);
    KXL_PutText(CENTER("Items"), AREA_LY + 148, "Items");
    KXL_PutImage(PixItem[0]->Image, AREA_LX + 120, AREA_LY + 152);
    KXL_PutImage(PixItemShot[0]->Image, AREA_LX + 120, AREA_LY + 218);
    KXL_PutImage(PixItemLaser[0]->Image, AREA_LX + 120, AREA_LY + 258);
    KXL_Font(NULL, 0xff, 0xff, 0xff);
    KXL_PutText(AREA_LX + 172, AREA_LY + 184, item_str[0]);
    KXL_PutText(AREA_LX + 172, AREA_LY + 240, item_str[1]);
    KXL_PutText(AREA_LX + 172, AREA_LY + 280, item_str[2]);
    KXL_Font(NULL, 0x00, 0xff, 0xff);
    KXL_PutText(CENTER(TO_MENU), AREA_LY + 320, TO_MENU);
  } else if (menu == 3) { /* ranking */
    KXL_Font(NULL, 0xff, 0xff, 0x00);
    KXL_PutText(CENTER("Ranking Top 5"), AREA_LY + 148, "Ranking Top 5");
    KXL_Font(NULL, 0x00, 0xff, 0xff);
    sprintf(buff, "Top    Score     Stage   Loop   Name");
    KXL_PutText(CENTER(buff), AREA_LY + 168, buff);
    KXL_Font(NULL, 0xff, 0xff, 0xff);
    for (i = 0; i < 5; i ++) {
      sprintf(buff, "    %hu    %08lu     %hhu       %hhu    %-8s", i + 1,
	      Ranking[i]->Score, Ranking[i]->Stage + 1,
	      Ranking[i]->Loop + 1, Ranking[i]->Name);
      KXL_PutText(CENTER(buff), AREA_LY + 188 + i * 20, buff);
    }
    KXL_Font(NULL, 0x00, 0xff, 0xff);
    KXL_PutText(CENTER(TO_MENU), AREA_LY + 320, TO_MENU);
  }
  /* check key */
  if (OldKey != Root->Key) {
    if (OldKey == KNone && Root->Key) {
      if (menu == 0) {
	KXL_PlaySound(SE_POW, KXL_SOUND_PLAY);
	if (Root->Key & KUp) {
	  if (sel == 0)
	    sel = 4;
	  else
	    sel --;
	} else if (Root->Key & KDown) {
	  if (sel == 4)
	    sel = 0;
	  else
	    sel ++;
	}
	if (Root->Key & KShot){
	  menu = sel;
	  if (menu == 0) {
	    Root->Stage = 0;
	    Root->MainFlag = MainGame;
	    LoadStageData();
	    CreateMy();
	    Root->Cnt = 0;
	  } else if (menu == 4)
	    return True;
	}
      } else {
	menu = 0;
	Root->Key = KNone;
      }
    }
    OldKey = Root->Key;
  }
  return False;
}
