#include <dsh/dsh.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
	char line[1024];
	int res;

	for(;;) {
		/* print the prompt */
		dsh_write_str(shell->writer, "[dsh]$ ");
		/* get a line */
		res = dsh_get_line(shell->reader, line, sizeof(line));
		if (res < 0) {
			dsh_write_str(shell->writer, "ERR: input line too long\r\n");
		}

		dsh_printf(shell->writer, "You said %s\r\n", line);
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
	int res = 0;

	va_start(varargs, format_string);
	res = vasprintf(&string, format_string, varargs);
	va_end(varargs);

	if (res < 0) {
		goto free_string;
	}

	dsh_write_str(writer, string);

free_string:
	if (string) {
		free(string);
	}

	return res;
}

static void dsh_write_str(dsh_char_writer writer, const char *string)
{
	while (*string != '\0') {
		writer(*string);
		string++;
	}
}
