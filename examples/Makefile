SF2FLOAT_POBJS = sf2float.o
SFGAIN_POBJS = sfgain.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -I ../include
LDFLAGS = -L../lib
LIBS = -lportsf

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	sf2float sfgain

clean:
	-rm -f sf2float; 
	rm -f sfgain;

veryclean:
	-rm -f $(SF2FLOAT_POBJS) 
	rm -f sf2float; 
	rm -f sfgain;

sf2float:		$(SF2FLOAT_POBJS)
	$(CC) -o sf2float $(SF2FLOAT_POBJS) $(LIBS) $(LDFLAGS);

sfgain:	$(SFGAIN_POBJS)
	$(CC) -o sfgain $(SFGAIN_POBJS) $(LIBS) $(LDFLAGS);

