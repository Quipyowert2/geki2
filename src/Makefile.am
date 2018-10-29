bin_PROGRAMS = geki2
geki2_SOURCES = boss.c misc.c game.c main.c opening.c\
	        load.c my.c ranking.c your.c\
	        boss.h extern.h ranking.h your.h game.h\
	        geki2.h load.h opening.h my.h misc.h

AM_CFLAGS = @X_CFLAGS@ @KXL_CFLAGS@
LIBS = @X_LIBS@ -lX11 @KXL_LIBS@ -lKXL
