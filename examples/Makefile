SF2FLOAT_POBJS = sf2float.o
SFGAIN_POBJS = sfgain.o
SFPAN_POBJS = sfpan.o
ENVX_POBJS = envx.o
SFENV_POBJS = sfenv.o breakpoint.o
SIGGEN_POBJS = siggen.o wave.o
OSCGEN_POBJS = oscgen.o wave.o breakpoint.o
TABGEN_POBJS = tabgen.o gtable.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -g -I ../include
LDFLAGS = -L../lib
LIBS = -lportsf

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	sf2float sfgain sfpan envx sfenv siggen oscgen tabgen

clean:
	-rm -f sf2float; 
	rm -f sfgain;
	rm -f sfpan;
	rm -f envx;
	rm -f sfenv;
	rm -f siggen;
	rm -f oscgen;
	rm -f tabgen;

veryclean:
	-rm -f $(SF2FLOAT_POBJS) 
	rm -f sf2float; 
	rm -f sfgain;
	rm -f sfpan;
	rm -f envx;
	rm -f sfenv;
	rm -f siggen;
	rm -f oscgen;
	rm -f tabgen;

sf2float:		$(SF2FLOAT_POBJS)
	$(CC) -o sf2float $(SF2FLOAT_POBJS) $(LIBS) $(LDFLAGS);

sfgain:	$(SFGAIN_POBJS)
	$(CC) -o sfgain $(SFGAIN_POBJS) $(LIBS) $(LDFLAGS);


sfpan: $(SFPAN_POBJS)
	$(CC) -o sfpan $(SFPAN_POBJS) $(LIBS) $(LDFLAGS);

envx: $(ENVX_POBJS)
	$(CC) -o envx $(ENVX_POBJS) $(LIBS) $(LDFLAGS);

sfenv: $(SFENV_POBJS)
	$(CC) -o sfenv $(SFENV_POBJS) $(LIBS) $(LDFLAGS);

siggen: $(SIGGEN_POBJS)
	$(CC) -o siggen $(SIGGEN_POBJS) $(LIBS) $(LDFLAGS);

oscgen: $(OSCGEN_POBJS)
	$(CC) -o oscgen $(OSCGEN_POBJS) $(LIBS) $(LDFLAGS);

tabgen: $(TABGEN_POBJS)
	$(CC) -o tabgen $(TABGEN_POBJS) $(LIBS) $(LDFLAGS);



