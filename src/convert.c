#include "utili.h"
#include "convertcomment.h"
#include "log.h"
// ./cct -s /home/xxx -d /home/xxxx

int main (int argc, char *argv[])
{
  int rec = 0;
  char src_path[512];
  char des_path[512];
  int is_dir = 0;
  while ((rec = getopt (argc, argv, "s:d:r")) != -1)
    {
      switch (rec)
        {
          case 's':
            strcpy (src_path, optarg);
          break;
          case 'd':
            strcpy (des_path, optarg);
          break;
          case 'r':
            is_dir = 1;
          break;
        }
    }
  if (is_dir == 1)
    {
      dir_convert (src_path, des_path);
    }
  else
    {
      file_convert (src_path, des_path);
    }
  sys_log (LOG_INFO, __FILE__, __LINE__, "%s Convert Completion.......\n", des_path);
  return 0;
}
