
DIRS = adpcm  arm  basicmath  bh bitcount  CRC32  dijkstra  em3d  FFT  hanoi  kmp  l2lat  patricia  qsort  sha  smatrix  susan sqlite

BROKEN = bisort mst bwmem

.PHONY: all install clean test $(addsuffix -install,$(DIRS)) $(addsuffix -clean,$(DIRS)) $(addsuffix -test,$(DIRS)) $(DIRS)

all: $(DIRS)

install: $(addsuffix -install,$(DIRS))

test: $(addsuffix -test,$(DIRS))

ftest: $(addsuffix -ftest,$(DIRS))

profile: $(addsuffix -profile,$(DIRS))

compare: $(addsuffix -compare,$(DIRS))

clean: $(addsuffix -clean,$(DIRS))

cleanall: $(addsuffix -cleanall,$(DIRS))

$(DIRS):
	@make -s -C $@ all

$(addsuffix -install,$(DIRS)):
	@make -s -C $(subst -install,,$@) install

$(addsuffix -clean,$(DIRS)):
	@make -s -C $(subst -clean,,$@) clean

$(addsuffix -cleanall,$(DIRS)):
	@make -s -C $(subst -cleanall,,$@) cleanall

$(addsuffix -test,$(DIRS)):
	@make -s -C $(subst -test,,$@) test

$(addsuffix -ftest,$(DIRS)):
	@make -s -C $(subst -ftest,,$@) ftest

$(addsuffix -compare,$(DIRS)):
	@make -s -C $(subst -compare,,$@) compare

$(addsuffix -profile,$(DIRS)):
	@make -s -C $(subst -profile,,$@) profile
