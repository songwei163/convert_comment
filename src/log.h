#ifndef _LOG_H
#define _LOG_H

#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<stdarg.h>
#include<syslog.h>
#include<sys/types.h>
#include<sys/stat.h>

void set_loglevel (int level);
void sys_log (int log_level, const char *file_name, int line_num, const char *format, ...);

#endif // _LOG_H
