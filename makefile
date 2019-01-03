CFLAGS=/nologo /W4 /Ox /GL /MD /sdl
SRCDIR=.
CC=cl

all:dist-analy.exe simul-mp.exe weibull-estimate.exe \
		simul-weibull.exe

dist-analy.exe: $(SRCDIR)/dist-analy.c
	$(CC) $** $(CFLAGS)

simul-mp.exe: $(SRCDIR)/simul-mp.c
	$(CC) $** $(CFLAGS)

weibull-estimate.exe: $(SRCDIR)/weibull-estimate.c
	$(CC) $** $(CFLAGS)

simul-weibull.exe: $(SRCDIR)/simul-weibull.c
	$(CC) $** $(CFLAGS)

clean:
	del *.obj

clean-exe:
	del *.exe

test-sw: simul-weibull.exe
	simul-weibull.exe 0.008641642 1.205623998 1060.280257 600000 10
