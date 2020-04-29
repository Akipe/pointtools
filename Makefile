# See LICENSE for license details.

include config.mk

all: options bin/md2point

options:
	@echo pointtools build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

bin/md2point: md2point.o
	${CC} -o $@ md2point.o ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 bin/dir2point $(DESTDIR)$(PREFIX)/bin/dir2point
	install -m 755 bin/md2point  $(DESTDIR)$(PREFIX)/bin/md2point
	install -m 755 bin/point2pdf $(DESTDIR)$(PREFIX)/bin/point2pdf
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	install -m 644 md2point.1    $(DESTDIR)$(MANPREFIX)/man1/md2point.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dir2point
	rm -f $(DESTDIR)$(PREFIX)/bin/md2point
	rm -f $(DESTDIR)$(PREFIX)/bin/point2pdf
	rm -f $(DESTDIR)$(MANPREFIX)/man1/md2point.1

clean:
	rm -f bin/md2point
	rm -f md2point.o

.PHONY: all options install uninstall clean
