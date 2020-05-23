SF2FLOAT_POBJS = sf2float.o
SFGAIN_POBJS = sfgain.o
SFPAN_POBJS = sfpan.o
ENVX_POBJS = envx.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -I ../include
LDFLAGS = -L../lib
LIBS = -lportsf

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	sf2float sfgain sfpan envx

clean:
	-rm -f sf2float; 
	rm -f sfgain;
	rm -f sfpan;
	rm -f envx;

veryclean:
	-rm -f $(SF2FLOAT_POBJS) 
	rm -f sf2float; 
	rm -f sfgain;
	rm -f sfpan;
	rm -f envx;

sf2float:		$(SF2FLOAT_POBJS)
	$(CC) -o sf2float $(SF2FLOAT_POBJS) $(LIBS) $(LDFLAGS);

sfgain:	$(SFGAIN_POBJS)
	$(CC) -o sfgain $(SFGAIN_POBJS) $(LIBS) $(LDFLAGS);


sfpan: $(SFPAN_POBJS)
	$(CC) -o sfpan $(SFPAN_POBJS) $(LIBS) $(LDFLAGS);

envx: $(ENVX_POBJS)
	$(CC) -o envx $(ENVX_POBJS) $(LIBS) $(LDFLAGS);
