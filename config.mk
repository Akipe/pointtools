VERSION="0.3"

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I. -I/usr/include

# BSD
#LIBS = -L/usr/lib -lc

# flags
# Linux
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_POSIX_C_SOURCE=200809L -DNEED_STRLCPY
# BSD
#CPPFLAGS = -DVERSION=\"${VERSION}\"

CFLAGS += -std=c99 ${INCS} ${CPPFLAGS}
LDFLAGS += ${LIBS}

# compiler and linker
# CC = cc
