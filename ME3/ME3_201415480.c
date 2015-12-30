#include <stdio.h>															
#include <stdlib.h>
#include <string.h>

void get_method(void);									//function prototypes
void post_method(void);
void parse(char* input);

int main(void){
	char *method;										//method points to the REQUEST_METHOD environment variable

	method = malloc(5);												

	printf("content-type:text/html\n\n");				//tell the server to expect an HTML file

	printf("<html><body>");								//start of HTML file

	if (method){
		method = getenv("REQUEST_METHOD");				//method can either be "GET" or "POST"
	}

	if (method){													
		if (!strcmp("GET", method)){					//if REQUEST_METHOD is "GET", execute the get_method function
			get_method();											
		} else {
			post_method();								//execute the post_method function otherwise
		}

	}

	printf("</body></html>");							//end of HTML file

	free(method);

	return 0;
}

void get_method(void){
	char *data;											//data points to the QUERY_STRING environment variable

	data = malloc(300);

	if (data){
		data = getenv("QUERY_STRING");					//QUERY_STRING is "text_get=%s", where %s is the input from the GET textarea
	}

	if (data){										
		parse(data + 9);								//move the pointer to exclude "text_get=" and execute the parse function
	}

	free(data);

	return;
}

void post_method(void){
	char *data;											//data points to the CONTENT_LENGTH environment variable
	char *content;										//content points to the input from the textarea
	int size = 0;										//size is the integer equivalent of CONTENT_LENGTH

	content = malloc(300);
	data = malloc(10);

	if (data){
		data = getenv("CONTENT_LENGTH");				//CONTENT_LENGTH is contains the string length
	}														//of "text_post=%s",where %s is the input from the POST textarea

	if (data){
		size = atoi(data);								//conversion of CONTENT_LENGTH from a string to an integer

		if (content){
			fgets(content, size + 1, stdin);			//reading data from stdin (GET and POST methods differ in input retrieval)
			parse(content + 10);						//move the pointer to exclude "text_post=" and execute the parse function
		}
	}

	free(content);
	free(data);

	return;
}

void parse(char* input){
	int i = 0;											//i is a counter
	char a[3] = {};										//a will hold the two hex digits representing a special character

	for (i = 0; i <= strlen(input); i++){				//this loop will "sweep" through the input, parsing special characters
		if (*(input + i) == '+'){						//if statement handling spaces
			putchar(' ');
		} else if (*(input + i) == '%'){				//if statement handling other special characters, which are preceded by '%'
			a[0] = *(input + i + 1);					//assign the first hex digit after the '%' to a[0]
			a[1] = *(input + i + 2);					//assign the second hex digit after the '%' to a[1] (a[2] = '/0')
			putchar(strtol(a, NULL, 16));				//conversion of hex string to integer
			i += 2;										//move the "sweeper" after the second hex digit of the special character
		} else {
			putchar(*(input + i));						//for normal characters
		}
	}
}