/*
 * See LICENSE for license details.
 */
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* from git://bitreich.org/utf8expr */
size_t
utf8strlen(const char *s)
{
	size_t i;

	for (i = 0; *s; s++) {
		if ((*s & 0xc0) != 0x80)
			i++;
	}

	return i;
}

void
fprintunderline(FILE *fp, const char *str)
{
	size_t i;

	fprintf(fp, "\n  %s\n  ", str);
	for (i = 0; i <= utf8strlen(str); ++i)
		fputs("=", fp);
	fputs("\n\n", fp);
}

void
escapechars(char *s)
{
	for (; *s; s++) {
		switch (*s) {
		case '#':
		case ' ':
		case '\t':
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
}

void
fprintesc(FILE *fp, const char *s)
{
	int intext = 0;

	fputs("  ", fp);
	for (; *s; s++) {
		switch (*s) {
		case ' ':
			fputc(' ', fp);
			break;
		case '\t':
			fprintf(fp, "        ");
			break;
		case '*':
			if (intext) {
				fputc(*s, fp);
			} else {
				fputc('o', fp);
				intext = 1;
			}
			break;
		default:
			intext = 1;
			fputc(*s, fp);
			break;
		}
	}
	fputs("\n", fp);
}

void
mkfilename(char *fname, char *str, size_t bufsiz, int i)
{
	char buf[PATH_MAX];

	strlcpy(buf, str, sizeof(buf));
	escapechars(buf);
	snprintf(fname, bufsiz, "%.4d-%s.txt", i, buf);
}

void
copyfile(char *dst, char *src)
{
	int c;
	FILE *fsrc, *fdst;

	if (src[0] == '\0' || dst[0] == '\0' ||
		!(fsrc = fopen(src, "r")) || !(fdst = fopen(dst, "w")))
		err(1, "copyfile: %s -> %s", src, dst);

	while ((c = fgetc(fsrc)) != EOF)
		fputc(c, fdst);

	fclose(fsrc);
	fclose(fdst);
}

int
main(void)
{
	size_t i = 0;
	char *line = NULL;
	size_t linesize = 0;
	ssize_t linelen;
	char title[PATH_MAX] = "", fname[PATH_MAX] = "", fname_old[PATH_MAX] = "";
	FILE *fp = NULL;

	while ((linelen = getline(&line, &linesize, stdin)) > 0) {
		if (line[linelen - 1] == '\n')
			line[--linelen] = '\0';

		if (line[0] == '%')
			continue;

		if (line[0] == '#' && line[1] == '#') {
			if (fp) {
				fclose(fp);
				fp = NULL;
			}
			strlcpy(title, line + 2, sizeof(title));
			mkfilename(fname, title, sizeof(fname), i++);
			if (!(fp = fopen(fname, "w")))
				err(1, "fopen: %s", fname);
			if (line[2] == '\0')
				fputs("\n", fp);
			else
				fprintunderline(fp, title);
		} else if (linelen > 5 && !strncmp(line, "#pause", linelen)) {
			if (fp) {
				fclose(fp);
				fp = NULL;
			}
			strlcpy(fname_old, fname, sizeof(fname_old));
			mkfilename(fname, title, sizeof(fname), i++);
			copyfile(fname, fname_old);
			if (fname[0] != '\0' && !(fp = fopen(fname, "a")))
				err(1, "fopen: %s", fname);
		} else {
			/* ignore text before first header */
			if (fp)
				fprintesc(fp, line);
		}
	}

	free(line);

	return 0;
}
