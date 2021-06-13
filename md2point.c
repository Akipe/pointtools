/*
 * See LICENSE for license details.
 */
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef NEED_STRLCPY /* OpenBSD implementation */
size_t
strlcpy(char *dst, const char *src, size_t dsize) {
	const char *osrc = src;
	size_t nleft = dsize;

	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++= *src++) == '\0')
				break;
		}
	}

	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';
		while (*src++)
			;
	}

	return(src - osrc - 1);
}
#endif /* NEED_STRLCPY */

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
	size_t i, len;

	fprintf(fp, "\n  %s\n  ", str);
	len = utf8strlen(str);
	for (i = 0; i <= len; ++i)
		fputs("=", fp);
	fputs("\n\n", fp);
}

void
escapechars(char *s)
{
	for (; *s; s++) {
		if (*s == '\n') {
			*s = '\0';
			return;
		}

		/*
		 * Only allow ASCII printable a-zA-Z0-9 for simplicity.
		 */
		if ((*s >= 'a' && *s <= 'z')
				|| (*s >= 'A' && *s <= 'Z')
				|| (*s >= '0' && *s <= '9')) {
			continue;
		}
		*s = '_';
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

		if (line[0] == '\\') {
			if (line[1] == '\0')
				continue;

			/* ignore text before first header */
			if (fp)
				fprintesc(fp, line+1);
		} else if (line[0] == '#' && line[1] == '#') {
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
