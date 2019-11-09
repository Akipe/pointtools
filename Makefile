PREFIX=	/usr/local

install:
	install -m755 bin/dir2point $(PREFIX)/bin/dir2point
	install -m755 bin/md2point  $(PREFIX)/bin/md2point
	install -m755 bin/point2pdf $(PREFIX)/bin/point2pdf

uninstall:
	rm -f $(PREFIX)/bin/dir2point
	rm -f $(PREFIX)/bin/md2point
	rm -f $(PREFIX)/bin/point2pdf
