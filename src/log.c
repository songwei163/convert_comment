#include"log.h"
#include<fcntl.h>
#include<dirent.h>

#define LOG_BUFFER_SIZE 256

static int loglevel = LOG_INFO;

static char *loglevels[] =
    {
        "LOG_EMERG!", "LOG_ALERT", "LOG_CRIT", "LOG_ERR", "LOG_WARNING", "LOG_NOTICE", "LOG_INFO", "LOG_DEBUG"
    };

void set_loglevel (int level)
{
  loglevel = level;
}

//sys_log(LOG_INFO, __FILE__, __LINE__, "%s %d","fjlalfjal",a);

void sys_log (int log_level, const char *file_name, int line_num, const char *format, ...)
{
#ifdef LOG_FILE
  DIR *log_dir = opendir("log");
  if (log_dir == NULL)
  {
      mkdir("log", 0755);
  }
  int fd = open("log/log.txt", O_WRONLY | O_CREAT | O_APPEND, 0755);
  close(STDOUT_FILENO);
  int fd1 = dup2(fd, STDOUT_FILENO);
#endif

  if (log_level > loglevel)
    return;

  time_t te;
  time (&te);
  struct tm *cur_time = localtime (&te);

  char buffer[LOG_BUFFER_SIZE];
  memset (buffer, 0, LOG_BUFFER_SIZE);
  strftime (buffer, LOG_BUFFER_SIZE - 1, " [%x %X ] ", cur_time);
  printf ("%s", buffer);//时间戳
  printf ("%s:%04d ", file_name, line_num);
  printf ("<%s> ", loglevels[log_level - LOG_EMERG]);

  va_list arg_list;
  va_start(arg_list, format);
  memset (buffer, 0, LOG_BUFFER_SIZE);
  vsnprintf (buffer, LOG_BUFFER_SIZE - 1, format, arg_list);
  printf ("%s\n", buffer);
  va_end(arg_list);
  fflush (stdout);
}







