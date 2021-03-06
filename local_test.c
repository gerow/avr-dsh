#include <stdio.h>

#include <dsh/dsh.h>
#include <dsh/dummy.h>

static char reader(void);
static void writer(char c);

int main(int argc, char **argv)
{
	struct dsh_shell shell;

	DDRB = 0x80;
	DDRC = 0xFF;

	dsh_init(&shell, reader, writer);
	int ret = dsh_run(&shell);

	printf("local_test: shell exited with return value %d\n", ret);

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
