# pointtools
# See LICENSE for license details.
PREFIX=	/usr/local

bin/md2point: md2point.o
	${CC} -o $@ md2point.o ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

install:
	install -m755 bin/dir2point $(PREFIX)/bin/dir2point
	install -m755 bin/md2point  $(PREFIX)/bin/md2point
	install -m755 bin/point2pdf $(PREFIX)/bin/point2pdf
	install -m644 md2point.1    $(PREFIX)/share/man/man1/md2point.1

uninstall:
	rm -f $(PREFIX)/bin/dir2point
	rm -f $(PREFIX)/bin/md2point
	rm -f $(PREFIX)/bin/point2pdf
	rm -f $(PREFIX)/share/man/man1/md2point.1

clean:
	rm -f bin/md2point
	rm -f md2point.o

.PHONY: install uninstall clean
