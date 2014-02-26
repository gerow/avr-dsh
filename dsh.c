#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dsh/dsh.h>
#if 1
#include <dsh/dummy.h>
#endif /* 1 */

static int dsh_get_line(dsh_char_reader reader, char *line, size_t size);
static int dsh_printf(dsh_char_writer writer, const char *format_string, ...);
static void dsh_write_str(dsh_char_writer writer, const char *string);

void dsh_init(struct dsh_shell *shell,
	      dsh_char_reader reader,
	      dsh_char_writer writer)
{
	shell->reader = reader;
	shell->writer = writer;
}

int dsh_run(struct dsh_shell *shell)
{
	char line[256];
	int res;

	for(;;) {
		/* print the prompt */
		dsh_write_str(shell->writer, "[dsh]$ ");
		/* get a line */
		res = dsh_get_line(shell->reader, line, sizeof(line));
		if (res < 0) {
			dsh_write_str(shell->writer, "dsh: input line too long\r\n");
			continue;
		}

		if (strncmp(line, "echo", 4) == 0) {
			char *arg = line + 4;
			dsh_printf(shell->writer, "%s\r\n", "woaaaaaah");
			continue;
		} 

		dsh_printf(shell->writer, "dsh: %s: command not found\r\n", line);
	}
}

/* Static functions */

/*
 * Read a from reader until we see a new line and then save the
 * result into line. The resulting value at line is garaunteed
 * to be a null terminated string.
 *
 * If we failed ran out of space on the reader string then we return
 * a value of -1 (but only after we see a newline). Otherwise the
 * return value is 0
 */
static int dsh_get_line(dsh_char_reader reader, char *line, size_t size)
{
	size_t n = 0;
	char c;
	int overrun = 0;

	while ( (c = reader()) != '\n') {
		if (n < (size - 1)) {
			line[n] = c;
			n++;
		} else {
			overrun = 1;
		}
	}

	line[n] = '\0';

	if (overrun) {
		return -1;
	}

	return 0;
}

static int dsh_printf(dsh_char_writer writer, const char *format_string, ...)
{
	va_list varargs;
	char *string = NULL;
	int size;

	va_start(varargs, format_string);
	size = vsnprintf(string, 0, format_string, varargs);
	string = malloc(size + 1);
	if (!string) {
		return -1;
	}
	vsnprintf(string, size + 1, format_string, varargs);
	va_end(varargs);

	dsh_write_str(writer, string);
	free(string);

	return 0;
}

static void dsh_write_str(dsh_char_writer writer, const char *string)
{
	while (*string != '\0') {
		writer(*string);
		string++;
	}
}
