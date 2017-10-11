
TARGETS=xelitetest src

all clean: $(TARGETS) 
$(TARGETS): 
	cd $@
	$(MAKE) -C $@ $(MAKECMDGOALS)

cleanall:
	cd src;make cleanall
	cd xelitetest;make cleanall

run:
	xelitetest <test/loup.xml

.PHONY: all clean cleanall $(TARGETS)


