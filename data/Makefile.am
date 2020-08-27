datadir = $(prefix)/share/geki2/data
data_DATA = stage1.dat stage2.dat stage3.dat stage4.dat stage5.dat stage6.dat\
	    map1.dat map2.dat map3.dat map4.dat map5.dat map6.dat
EXTRA_DIST = stage1.dat stage2.dat stage3.dat stage4.dat stage5.dat stage6.dat\
	    map1.dat map2.dat map3.dat map4.dat map5.dat map6.dat .score

install:
	mkdir -p $(DESTDIR)$(DATA_PATH)
	$(INSTALL) -c -m 666 ./.score $(DESTDIR)$(DATA_PATH)
	$(INSTALL) $(data_DATA) $(DESTDIR)$(DATA_PATH)

