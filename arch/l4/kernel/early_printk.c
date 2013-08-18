/*
 * Early_printk implementation, skeleton taken from x86_64 version.
 */
#include <linux/console.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

#include <l4/sys/kdebug.h>

static void early_kdb_write(struct console *con, const char *s, unsigned n)
{
	while (*s && n-- > 0) {
		outchar(*s);
		if (*s == '\n')
			outchar('\r');
		s++;
	}
}

static struct console early_kdb_console = {
	.name =		"earlykdb",
	.write =	early_kdb_write,
	.flags =	CON_PRINTBUFFER,
	.index =	-1,
};

int __init setup_early_printk(char *buf)
{
	if (!buf)
		return 0;

	if (early_console)
		return 0;

	if (strstr(buf, "keep"))
		early_kdb_console.flags &= ~CON_BOOT;
	else
		early_kdb_console.flags |= CON_BOOT;

	early_console = &early_kdb_console;
	register_console(early_console);
	return 0;
}

__setup("earlyprintk=", setup_early_printk);
