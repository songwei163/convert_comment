#ifndef _CONVERTCOMMENT_H
#define _CONVERTCOMMENT_H

#include "utili.h"
#include"log.h"
FILE *open_file (char *file, char *mode);
void close_file (FILE *fp);
char read_ch (FILE *fp);
void write_ch (char ch, FILE *fp);
void write_double (char ch1, char ch2, FILE *fp);
/////////////////////////////////////////////////////

int convertcomment (FILE *inputfile, FILE *outputfile);
void eventpro (char ch);
void eventpro_no (char ch);
void eventpro_cpp (char ch);
void eventpro_c (char ch);
void eventpro_str (char ch);
//////////////////////////////////////////////////////////

void dir_convert (char *src_dir, char *des_dir);
void file_convert (char *src_file, char *des_file);
#endif // _CONVERTCOMMENT_H
