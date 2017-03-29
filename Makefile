LINK_LIBS=-lpthread
ping: $(LINK_LIBS)

debug: ping.c
	gcc $(LINK_LIBS) -g ping.c -o ping.dbg
