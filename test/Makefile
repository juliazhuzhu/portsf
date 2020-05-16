POBJS = test.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -I ../include
LDFLAGS = -L../lib
LIBS = -lportsf

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	audio_test

clean:
	-rm -f audio_test; 

veryclean:
	-rm -f $(POBJS) 
	rm -f audio_test; 

audio_test:		$(POBJS)
	$(CC) -o audio_test $(POBJS) $(LIBS) $(LDFLAGS);
