SF2FLOAT_POBJS = sf2float.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -I ../include
LDFLAGS = -L../lib
LIBS = -lportsf

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	sf2float

clean:
	-rm -f sf2float; 

veryclean:
	-rm -f $(SF2FLOAT_POBJS) 
	rm -f sf2float; 

sf2float:		$(SF2FLOAT_POBJS)
	$(CC) -o sf2float $(SF2FLOAT_POBJS) $(LIBS) $(LDFLAGS);
