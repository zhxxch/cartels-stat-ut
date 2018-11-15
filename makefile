CFLAGS=/nologo /W4 /O2 /GL /arch:AVX2 /D_CRT_SECURE_NO_WARNINGS
SRCDIR=.
CC=cl

all:dist-analy.exe simul-mp.exe weibull-estimate.exe

dist-analy.exe: $(SRCDIR)/dist-analy.c
	$(CC) $** $(CFLAGS)

simul-mp.exe: $(SRCDIR)/simul-mp.c
	$(CC) $** $(CFLAGS)

weibull-estimate.exe: $(SRCDIR)/weibull-estimate.c
	$(CC) $** $(CFLAGS)

clean:
	del *.obj
