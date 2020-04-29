/*
 * See LICENSE for license details.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define PATH_MAX 128

char buf[PATH_MAX];

/* from git://bitreich.org/utf8expr */
size_t
utf8strlen(char *s)
{
	size_t i;

	i = 0;
	for (; s[0]; s++) {
		if ((s[0] & 0xc0) != 0x80)
			i++;
	}

	return i;
}

void
fprintunderline(FILE *fp, char *str, size_t linelen)
{
	size_t i;
	fprintf(fp, "\n  %s\n  ", str);
	for (i=0; i<=utf8strlen(str); ++i)
		fputs("=", fp);
	fputs("\n\n", fp);
}

void
escapechars(char *s, size_t linelen)
{
	size_t i;
	for (i=0; i<linelen && *s != '\0'; (void)*s++, i++)
		switch (*s) {
			case '#':
			case ' ':
			case '	':
			case ':':
			case '.':
			case '(':
			case ')':
			case '/':
				*s = '_';
				break;
			case '\n':
				*s = '\0';
				return;
			default:
				break;
		}
}

void
fprintesc(FILE *fp, char *s, ssize_t len)
{
	ssize_t i;
	int intext;

	intext = 0;
	fputs("  ", fp);
	for (i=0; i<len && s[i] != '\0'; i++)
		switch (s[i]) {
			case ' ':
				fputc(' ', fp);
				break;
			case '\t':
				fprintf(fp, "        ");
				break;
			case '*':
				if (intext) {
					fputc(s[i], fp);
				} else {
					fputc('o', fp);
					intext = 1;
				}
				break;
			default:
				intext = 1;
				fputc(s[i], fp);
				break;
		}
	fputs("\n", fp);
}

void
mkfilename(char *fname, char *str, size_t len, int i)
{
	strlcpy(buf, str, len);
	escapechars(buf, len);
	snprintf(fname, len, "%.4d-%s.txt", i, buf);
}

void
copyfile(char *dst, char *src)
{
	int c;
	FILE *fsrc, *fdst;

	if (strlen(src) < 1 || strlen(dst) < 1 ||
		!(fsrc = fopen(src, "r")) || !(fdst = fopen(dst, "w")))
		err(1, "copyfile: %s -> %s", src, dst);

	while ((c = fgetc(fsrc)) != EOF)
		fputc(c, fdst);

	fclose(fsrc);
	fclose(fdst);
}

int
main(int argc, char* argv[])
{
	int i;
	static char *line;
	static size_t linesize;
	ssize_t linelen;
	char title[PATH_MAX], fname[PATH_MAX], fname_old[PATH_MAX];
	FILE *fp;

	fp = NULL;
	title[0] = fname[0] = fname_old[0] = '\0';
	i = 0;
	while ((linelen = getline(&line, &linesize, stdin)) > 0) {

		if (line[linelen-1] == '\n')
			line[--linelen] = '\0';

		if (linelen > 1 && line[0] == '#' && line[1] == '#') {
			if (fp)
				fclose(fp);
			strlcpy(title, line+2, PATH_MAX);
			mkfilename(fname, title, PATH_MAX, i++);
			if (!(fp = fopen(fname, "w")))
				err(1, "fopen: %s", fname);
			if (linelen == 2)
				fputs("\n", fp);
			else
				fprintunderline(fp, title, linelen);

		} else if (linelen > 0 && line[0] == '%') {
			continue;

		} else if (linelen > 5 && !strncmp(line, "#pause", linelen)) {
			if (fp)
				fclose(fp);
			strlcpy(fname_old, fname, PATH_MAX);
			mkfilename(fname, title, PATH_MAX, i++);
	 		copyfile(fname, fname_old);
			if (strlen(fname) > 0 && !(fp = fopen(fname, "a")))
				err(1, "fopen: %s", fname);

		} else {
			/* ignore text before first header */
			if (fp)
				fprintesc(fp, line, linelen);
		}
	}

	free(line);
	return 0;
}
