#include <stdio.h>

#include <dsh/dsh.h>
#include <dsh/dummy.h>

static char reader(void);
static void writer(char c);

int main(int argc, char **argv)
{
	struct dsh_shell shell;

	DDRB = 0x80;

	dsh_init(&shell, reader, writer);
	dsh_run(&shell);

	return 0;
}

static char reader(void)
{
	return getc(stdin);
}

static void writer(char c)
{
	putc(c, stdout);
}
