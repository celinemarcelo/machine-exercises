#ifndef ME4_H_
#define ME4_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cgi_init(void);
void start_html(char *title);
void end_html(void);
long get_length(void);
void get_data(long len, char *data);
void debug_to_screen(long len, char *data);
void get_element(char *data, char *name, char *value);
int save_upload(char *data, char *name, char *value);

#endif /* ME4_H_ */
