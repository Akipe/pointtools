
VERSION="0.3"

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I. -I/usr/include

# BSD
#LIBS = -L/usr/lib -lc
# Linux
LIBS = -L/usr/lib -lc -lbsd

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS += -g -std=c99 -pedantic -Wall -Wvariadic-macros -Os ${INCS} ${CPPFLAGS}
LDFLAGS += -g ${LIBS}

# compiler and linker
# CC = cc
