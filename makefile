SUBDIRS := eventloop #2028  GreedySnake Tetris tests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	$(MAKE) -C $(SUBDIRS) clean
