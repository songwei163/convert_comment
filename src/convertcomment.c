
#include "convertcomment.h"
#include "log.h"
//定义状态
typedef enum {
    no_comment_state,
    c_comment_state,
    cpp_comment_state,
    str_state,
    end_state
} enum_state;

//定义状态机
typedef struct {
    FILE *input;
    FILE *output;
    enum_state ulstate;
} state_machine;

//定义全局状态机
state_machine g_state;

#define PATH_SIZE 512

/////////////////////////////////////////////////////////
FILE *open_file (char *file, char *mode)
{
  FILE *fp = fopen (file, mode);
  if (NULL == fp)
    {
      sys_log (LOG_ERR, __FILE__, __LINE__, "open file : %s fail", file);
      exit (1);
    }
  return fp;
}
void close_file (FILE *fp)
{
  assert(fp != NULL);
  fclose (fp);
}
char read_ch (FILE *fp)
{
  return fgetc (fp);
}
void write_ch (char ch, FILE *fp)
{
  fputc (ch, fp);
}
void write_double_ch (char ch1, char ch2, FILE *fp)
{
  fputc (ch1, fp);
  fputc (ch2, fp);
}

/////////////////////////////////////////////////////////


int convertcomment (FILE *inputfile, FILE *outputfile)
{
  if (inputfile == NULL || outputfile == NULL)
    {
      sys_log (LOG_INFO, __FILE__, __LINE__, "invalid arugment inputfile or outputfile.\n");
      return -1;
    }

  /////////////////////////////////////////////////////////
  //初始化状态机
  g_state.input = inputfile;
  g_state.output = outputfile;
  g_state.ulstate = no_comment_state;

  char ch;
  while (g_state.ulstate != end_state)
    {
      ch = fgetc (g_state.input);
      eventpro (ch);
    }

  return 0;
}

void eventpro (char ch)
{
  switch (g_state.ulstate)
    {
      case no_comment_state:
        eventpro_no (ch);
      break;
      case c_comment_state:
        eventpro_c (ch);
      break;
      case cpp_comment_state:
        eventpro_cpp (ch);
      break;
      case str_state:
        eventpro_str (ch);
      break;
    }
}

void eventpro_no (char ch)
{
  char nextch;
  switch (ch)
    {
      case '/':
        nextch = fgetc (g_state.input);
      if (nextch == '/')   //C++
        {
          //fputc('/', g_state.output);
          //fputc('*', g_state.output);
          write_double_ch ('/', '*', g_state.output);
          g_state.ulstate = cpp_comment_state;
        }
      else if (nextch == '*')
        {
          fputc ('/', g_state.output);
          fputc ('*', g_state.output);
          g_state.ulstate = c_comment_state;
        }
      break;
      case '"':
        g_state.ulstate = str_state;
      fputc ('"', g_state.output);
      break;
      case EOF:
        g_state.ulstate = end_state;
      break;
      default:
        fputc (ch, g_state.output);
      break;
    }
}

void eventpro_cpp (char ch)
{
  char nextch;
  char thirdch;
  switch (ch)
    {
      case EOF:
        fputc ('*', g_state.output);
      fputc ('/', g_state.output);
      g_state.ulstate = end_state;
      break;
      case '\n':
        fputc ('*', g_state.output);
      fputc ('/', g_state.output);
      g_state.ulstate = no_comment_state;
      fputc ('\n', g_state.output);
      break;
      case '/':
        nextch = fgetc (g_state.input);

      if (nextch == '/' || nextch == '*')   //123 //456  or //123 /*456
        {
          if (nextch == '/')
            {
              thirdch = fgetc (g_state.input);
              if (thirdch == '/')
                {
                  while ((thirdch = fgetc (g_state.input)) == '/');
                  fputc (thirdch, g_state.output);
                }
              else
                {
                  fputc (' ', g_state.output);
                  fputc (' ', g_state.output);
                  fseek (g_state.input, -1, SEEK_CUR);
                }
            }
        }
      break;
      case '*':
        nextch = fgetc (g_state.input);
      if (nextch == '/')   // 123 */
        {
          fputc (' ', g_state.output);
          fputc (' ', g_state.output);
        }
      break;
      case '"':
        fputc ('"', g_state.output);
      g_state.ulstate = str_state;
      while (g_state.ulstate == str_state)
        {
          ch = fgetc (g_state.input);
          eventpro_str (ch);
        }
      g_state.ulstate = cpp_comment_state;
      break;
      default:
        fputc (ch, g_state.output);
      break;
    }
}

void eventpro_c (char ch)
{
  char nextch;
  char thirdch;
  switch (ch)
    {
      case '*':
        nextch = fgetc (g_state.input);
      if (nextch == '/')    /*  123456 */
        {
          fputc ('*', g_state.output);
          fputc ('/', g_state.output);
          g_state.ulstate = no_comment_state;
        }
      else if (nextch == '*')
        {
          fputc ('*', g_state.output);
          fseek (g_state.input, -1, SEEK_CUR);
        }
      break;
      case '/':
        nextch = fgetc (g_state.input);
      if (nextch == '/')   //  /*123 //456 */
        {
          fputc (' ', g_state.output);
          fputc (' ', g_state.output);
        }
      break;
      case '"':
        fputc ('"', g_state.output);
      g_state.ulstate = str_state;
      while (g_state.ulstate == str_state)
        {
          ch = fgetc (g_state.input);
          eventpro_str (ch);
        }
      g_state.ulstate = c_comment_state;
      break;
      default:
        fputc (ch, g_state.output);
      break;
    }
}

void eventpro_str (char ch)
{
  char nextch;
  switch (ch)
    {
      case '"':
        fputc ('"', g_state.output);
      g_state.ulstate = no_comment_state;
      break;
      case '*':
        nextch = fgetc (g_state.input);
      if (nextch == '/')
        {
          fputc ('*', g_state.output);
          fputc (' ', g_state.output);
          fputc ('/', g_state.output);
        }
      else
        {
          fputc ('*', g_state.output);
        }
      break;
      case '/':
        nextch = fgetc (g_state.input);
      if (nextch == '*')
        {
          fputc ('/', g_state.output);
          fputc (' ', g_state.output);
          fputc ('*', g_state.output);
        }
      else
        {
          fputc ('/', g_state.output);
          fputc (nextch, g_state.output);
        }
      break;
      default:
        fputc (ch, g_state.output);
      break;
    }
}

///////////////////////////////////////////////////////////////////////////

void dir_convert (char *src_dir, char *des_dir)
{
  DIR *src_dp = opendir (src_dir);
  if (src_dp == NULL)
    {
      sys_log (LOG_ERR, __FILE__, __LINE__, "connot open dir : %s", src_dir);
    }
  DIR *des_dp = opendir (des_dir);
  if (des_dp == NULL)
    {
      mkdir (des_dir, 0755);
    }

  char src_pre_path[PATH_SIZE];
  char src_cur_path[PATH_SIZE];
  char des_pre_path[PATH_SIZE];
  char des_cur_path[PATH_SIZE];

  strcpy (src_pre_path, src_dir);
  strcpy (src_cur_path, src_dir);
  strcpy (des_pre_path, des_dir);
  strcpy (des_cur_path, des_dir);

  struct dirent *ptr;
  while ((ptr = readdir (src_dp)) != NULL)
    {
      if (strncmp (ptr->d_name, ".", 1) == 0)
        {
          continue;
        }
      sprintf (src_cur_path, "%s/%s", src_pre_path, ptr->d_name);
      sprintf (des_cur_path, "%s/%s", des_pre_path, ptr->d_name);

      struct stat info;
      if (stat (src_cur_path, &info) < 0)
        {
          sys_log (LOG_ERR, __FILE__, __LINE__, "stat err");
          continue;
        }
      if (S_ISDIR(info.st_mode))
        {
          dir_convert (src_cur_path, des_cur_path);
        }
      else
        {
          file_convert (src_cur_path, des_cur_path);
        }

      strcpy (src_cur_path, src_pre_path);
      strcpy (des_cur_path, des_pre_path);

    }
}

void file_convert (char *src_file, char *des_file)
{
  FILE *sfp = open_file (src_file, "r");
  FILE *dfp = open_file (des_file, "w");

  convertcomment (sfp, dfp);

  close_file (sfp);
  close_file (dfp);
}
