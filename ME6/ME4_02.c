/*
	ME4_Group2
	Celine Marcelo 2014-15480
	Jose Baquiran 2014-06321
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ME4.h"

#define MAXLEN 1000000
#define DATALEN 1000

int main2(void) {
	// variable declaration and fixed memory allocation
	char data[MAXLEN];					memset(data,MAXLEN,0);
	char textinput[DATALEN];		memset(textinput,DATALEN,0);
	char radiobutton[DATALEN];	memset(radiobutton,DATALEN,0);
	char checkbox[DATALEN];			memset(checkbox,DATALEN,0);
	char textarea[DATALEN];			memset(textarea,DATALEN,0);
	char select[DATALEN];				memset(select,DATALEN,0);
	char fileupload[DATALEN];		memset(fileupload,DATALEN,0);

	// get data
	get_data(get_length(), &data[0]);

	// initialize cgi
	cgi_init();
	start_html("ME4_Group2");

	// print data to screen
	debug_to_screen(get_length(), &data[0]);	

	// get value of each element
	get_element(&data[0],"textinput1", &textinput[0]);
	printf("    <br>textinput1=\"%s\"\n", textinput);
	get_element(&data[0],"radiobutton1", &radiobutton[0]);
	printf("    <br>radiobutton1=\"%s\"\n", radiobutton);
	get_element(&data[0],"checkbox1", &checkbox[0]);
	printf("    <br>checkbox1=\"%s\"\n", checkbox);
	get_element(&data[0],"textarea1", &textarea[0]);
	printf("    <br>textarea1=\"%s\"\n", textarea);
	get_element(&data[0],"select1", &select[0]);
	printf("    <br>select1=\"%s\"\n", select);
	int status = save_upload(&data[0],"fileupload1", &fileupload[0]);
	printf("    <br>fileupload1=\"%s\"\n", fileupload);
	if(status == 0) {
		printf("    <br>\"%s\" saved to cgi-bin\n", fileupload);
	}

	// end cgi
	end_html();
	return 0;
}

void cgi_init(void){
	printf("content-type:text/html\n\n");
}

void start_html(char *title){
	printf("<!DOCTYPE html>\n");
	printf("<html>\n<head>\n\t<title>%s", title);
    printf("</title>\n\t<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\" rel=\"stylesheet\">\n</head>");
	printf("\n\n<body>");
}

void end_html(void){
	printf("\n\t<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js\"></script>");
	printf("\n\t<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js/\"></script>");
	printf("\n</body>\n</html>");
}

long get_length(void){
	char method[5] = {};
	char data[5] = {};
	long size = 0;

	strcpy(method, getenv("REQUEST_METHOD"));

	if (!strcmp("GET", method)){
	//	printf("<p>GET</p>");
		strcpy(data, getenv("QUERY_STRING"));
		size = strlen(data);
	//	printf("<p>strlen(QUERY_STRING) = %lu</p>", size);	
	} else {
	//	printf("<p>POST</p>");
		strcpy(data, getenv("CONTENT_LENGTH"));
		size = atol(data);
	//	printf("<p>CONTENT_LENGTH = %lu</p>", size);
	}

	return size;
}

void get_data(long len, char *data){
	char method[5] = {};
	int i = 0;

	strcpy(method, getenv("REQUEST_METHOD"));

	if (!strcmp("GET", method)){
		strcpy(data, getenv("QUERY_STRING"));
	} else {
		for (i = 0; len; len--, i++){
			*(data + i) = fgetc(stdin);
		}
	}
}

void debug_to_screen(long len, char *data){
	char method[5] = {};

	strcpy(method, getenv("REQUEST_METHOD"));

	if (!strcmp("GET", method)){
		printf("<p>(QUERY_STRING) = %s</p>\n", data);
	} else {
		printf("<p>CONTENT = %s</p>\n", data);
	}
}

void get_element(char *data, char *name, char *value){
	char a[3] = {};
	char method[5] = {}; // GET or POST
	char ctrstr[100] = {}; // control string
	char line[100] = {};
	char *holder;
	char *end;
	
	// pointer of intermediate values of (char*) value
	char *temp; 
	char *temp2;
	char *temp3;
	char *dummy; // duplicate of data
	int i = 0, j = 0, k = 0; // counters

	holder = malloc(300);
	end = malloc(300);
	temp = malloc(300);
	temp2 = malloc(300);
	temp3 = malloc(300);
	dummy = malloc(MAXLEN);

	memcpy(dummy, data, MAXLEN);
	
	strcpy(method, getenv("REQUEST_METHOD"));

	if (!strcmp("GET", method)){
		holder = strstr(getenv("QUERY_STRING"), name);

		// if 
		if(holder){
			holder += strlen(name) + 1;

			for (i = 0, j = 0; *(holder + i) != '&'; i++, j++){
				if (*(holder + i) == '+'){ //parse space
					*(value + j) = ' ';
				} else if (*(holder + i) == '%'){ // parse hex values
					a[0] = *(holder + i + 1);
					a[1] = *(holder + i + 2);
					*(value + j) = strtol(a, NULL, 16);
					i += 2;
				} else { // parse content
					*(value + j) = *(holder + i);
				}
			}

			// For Checkboxes
			if(!strcmp(name, "checkbox1")){ 
				temp2 = strstr(holder, name);
				
				// Continue searching holder while checked checkboxes exist 
				while (temp2){

					if(k){
						temp2 = strstr(holder + strlen(value) + strlen(name) + 1, name);
					} else {
						temp2 = strstr(holder + strlen(value) + 1, name);
					}
					
					k++;
					if(temp2){
						temp2 += strlen(name) + 1;

						for (i = 0, j = 0; *(temp2 + i) != '&'; i++, j++){
							if (*(temp2 + i) == '+'){
								*(temp3 + j) = ' ';
							} else if (*(temp2 + i) == '%'){
								a[0] = *(temp2 + i + 1);
								a[1] = *(temp2 + i + 2);
								*(temp3 + j) = strtol(a, NULL, 16);
								i += 2;
							} else {
								*(temp3 + j) = *(temp2 + i);
							}
						}

						temp3[j + 1] = 0;
						strcat(value, ", ");
						strcat(value, temp3);
					}
				}
			}
		}


	} else {
		for (i = 0; *(data + i) != '\n'; i++){
			*(ctrstr + i) = *(data + i);

			if (*(data + i + 1) == '\n'){
				*(ctrstr + i + 1) = 0;
			}
		}

		// Formatting further control string
		strcat(line, ctrstr);
		strcat(line, "\n");
		strcat(line, "Content-Disposition: form-data; name=\"");
		strcat(line, name);
		strcat(line, "\"");
		temp = strstr(dummy, line);

		strcat(end, ctrstr);
		end[strlen(end) - 1] = 0;
		strcat(end, "--\r");

		if(temp){
			holder = strtok(temp, "\n");
			for (i = 3; i; i--){
				holder = strtok(NULL, "\n");
			}

			do {
				strcat(value, holder);
				value[strlen(value)] = '\n';
				value[strlen(value) + 1] = 0;
				holder = strtok(NULL, "\n");
			} while(strcmp(holder, ctrstr) && strcmp(holder, end));

			// For Checkbox/es
			if(!strcmp(name, "checkbox1")){
				memcpy(dummy, data, MAXLEN);
				memset(value, 0, strlen(value));
				
				while(temp){
					dummy = temp + strlen(line) + 4;
					
					temp = strstr(dummy, line);

					for (i = 0; *(dummy + i) != '\n'; i++){
						*(holder + i) = *(dummy + i);
						
						if(*(dummy + i + 1) == '\n'){
							*(holder + i + 1) = 0;
						}
					}
					
					if(j){
						strcat(value, ", ");
					}
					strcat(value, holder);
					j++;
				}
			}
		}
	}
}


int save_upload(char *data, char *name, char *value){
	char a[3] = {};
	char method[5] = {}; // GET or POST
	char ctrstr[100] = {}; // control string
	char end[100] = {};
	char line[100] = {};
	char *content;
	char *holder; 
	char *temp;
	char *dummy; // duplicate of data
	int i = 0, j = 0; // counters

	content = malloc(MAXLEN);
	holder = malloc(300);
	dummy = malloc(MAXLEN);
	temp = malloc(300);

	memcpy(dummy, data, MAXLEN);
	
	strcpy(method, getenv("REQUEST_METHOD"));

	if (!strcmp("GET", method)){
		holder = strstr(getenv("QUERY_STRING"), name);
		
		if(holder){
			temp = strtok(holder, "&");

			temp += strlen(name) + 1;

			for (i = 0, j = 0; i <= strlen(temp); i++, j++){
				if (*(temp + i) == '+'){
					*(value + j) = ' ';
				} else if (*(temp + i) == '%'){
					a[0] = *(temp + i + 1);
					a[1] = *(temp + i + 2);
					*(value + j) = strtol(a, NULL, 16);
					i += 2;
				} else {
					*(value + j) = *(temp + i);
				}
			}

			return -1;
		}
	} else {
		for (i = 0; *(data + i) != '\n'; i++){
			*(ctrstr + i) = *(data + i);

			if (*(data + i + 1) == '\n'){
				*(ctrstr + i + 1) = 0;
			}
		}

		strcat(line, ctrstr);
		strcat(line, "\n");
		strcat(line, "Content-Disposition: form-data; name=\"");
		strcat(line, name);
		strcat(line, "\"; filename=\"");
		temp = strstr(dummy, line);

		temp += strlen(line);

		for (i = 0; *(temp + i) != '\"'; i++){
			*(value + i) = *(temp + i);

			if (*(temp + i + 1) == '\"'){
				*(value + i + 1) = 0;
			}
		}
		//printf("<h1>value = %s</h1>\n", value);



		strcat(end, ctrstr);
		end[strlen(end) - 1] = 0;
		strcat(end, "--\r");
		//printf("<p>end = %s</p>\n", end);


		holder = strtok(temp, "\n");

		for (i = 3; i; i--){
			holder = strtok(NULL, "\n");
		}

		do{
			strcat(content, holder);
			content[strlen(content)] = '\n';
			content[strlen(content) + 1] = 0;
			holder = strtok(NULL, "\n");
		}while(strcmp(holder, end));


		printf("<p>content = %s</p>\n", content);
		//printf("<p>holder = %s</p>\n", holder);

		if(strcmp(value, "\0")){
			FILE *fp = fopen(value, "w+");
			if(fputs(content, fp) != EOF){
				fclose(fp);	
				return 0;
			} else {
				return -1;
			}
		}
	}

	return -1;
}