LINK_LIBS=-lpthread
ping: $(LINK_LIBS)

.PHONY: debug
debug: ping.c
	$(CC) $(LINK_LIBS) -g $^ -o ping.dbg
.PHONY: all
all: ping debug

.PHONY: clean
clean:
	rm ping  ping.dbg
