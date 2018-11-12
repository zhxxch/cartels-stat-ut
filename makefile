CFLAGS=/nologo /W4 /O2 /GL /arch:AVX2 /D_CRT_SECURE_NO_WARNINGS
SRCDIR=.
CC=cl

all:dist-analy.exe

dist-analy.exe: $(SRCDIR)/dist-analy.c
	$(CC) $** $(CFLAGS)

clean:
	del *.obj
