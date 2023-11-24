#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

bool do_system(const char *command);

bool do_exec(int count, ...);

bool do_exec_redirect(const char *outputfile, int count, ...);
