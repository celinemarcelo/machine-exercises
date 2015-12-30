/*
	ME4_Group2
	Celine Marcelo 2014-15480
	Jose Baquiran 2014-06321
*/

#include <stdio.h>
#include "/opt/lampp/include/mysql.h"
#include "ME4.h"


#define MAXLEN 1000000
#define DATALEN 1000

/*
	You may declare strings (data storage) as global variables here
	or you may edit the given functions
*/


void initial_page(char *password, char *database) {
	puts("\t\t<div class=\"col-sm-1\"></div>");
	puts("\t\t<div class=\"col-sm-3\">");
	puts("\t\t\t<div class=\"panel panel-primary\">");
	puts("\t\t\t\t<div class=\"panel-heading\">");
	puts("\t\t\t\t\t<center>MySQL Web Client</center>");
	puts("\t\t\t\t</div>");
	puts("\t\t\t\t<div class=\"panel-body\">");
	puts("\t\t\t\t\t<form action=\"http://localhost/cgi-bin/ME6.cgi\" method=\"post\" enctype=\"multipart/form-data\" target=\"result\" role=\"form\">");
	puts("\t\t\t\t\t<label for=\"element1\">Database:</label>");
	printf("\t\t\t\t\t<input type=\"text\" name=\"database\" class=\"form-control\" id=\"element1\" value=\"%s\" readonly> <br>", database);
	puts("\t\t\t\t\t<label for=\"element2\">Password:</label>");
	printf("\t\t\t\t\t<input type=\"password\" name=\"password\" class=\"form-control\" id=\"element2\" value=\"%s\" readonly> <br>", password);
	puts("\t\t\t\t\t<label for=\"element3\">Query:</label>");
	puts("\t\t\t\t\t<input type=\"text\" name=\"query\" class=\"form-control\" id=\"element3\"> <br>");
	puts("\t\t\t\t\t<input type=\"submit\" name=\"button\" class=\"btn btn-default\" value=\"Send Query\">");
	puts("\t\t\t\t\t<input type=\"reset\" class=\"btn btn-default\" value=\"Reset\">");
	puts("\t\t\t\t\t<a href=\"http://localhost/ME6.html\" class=\"btn btn-default\">Log out</a>");
	puts("\t\t\t\t\t</form>");
	puts("\t\t\t\t</div>");
	puts("\t\t\t</div>");
	puts("\t\t</div>");
	puts("\t\t<div class=\"col-sm-6\">");
	puts("\t\t\t<div class=\"well\" style=\"height: 345px\">");
	puts("\t\t\t\t<iframe name=\"result\" style=\"height:100%%; width:100%%\"></iframe>");
	puts("\t\t\t</div>");
	puts("\t\t</dipv>");
	puts("\t\t<div class=\"col-sm-2\"></div>");
}

void result_page(char *password, char *database, char *query) {
	int connect = 0, i = 0, j = 0;
	unsigned int num_fields = 0, num_rows = 0;

	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;

//	printf("MySQL client version: %s\n", mysql_get_client_info());

	if(mysql_init(&mysql)){
		if(!mysql_real_connect(&mysql, "127.0.0.1", "root", password, database, 0, NULL, 0)){
			puts("<h1>Error in connecting database.</h1>");
		} 
			connect++;
	}

	if(connect){
		if (mysql_query(&mysql, query)){      
     		printf("<div class=\"container\"><div class=\"jumbotron\"><h1 style=\"text-align:center\">\
   					ERROR %u (%s): %s</h1></div></div>", mysql_errno(&mysql), mysql_sqlstate(&mysql), mysql_error(&mysql));
  		} else {
			res = mysql_store_result(&mysql);

			if(res){
				num_fields = mysql_num_fields(res);

				if(num_fields){
					row = mysql_fetch_row(res);

					puts("<table class=\"table-striped table table-responsive\" style=\"font-size:20px;\">");

					for(j = 0; j < num_fields; j++){
       					field = mysql_fetch_field_direct(res, j);
    					printf("<th>%s</th>", field->name);	
					}

					for(j = 0; j < mysql_num_rows(res); j++){
   						unsigned long *lengths;
   						lengths = mysql_fetch_lengths(res);
   					
   						puts("<tr>");
   						for(i = 0; i < num_fields; i++){
       						printf("<td>%.*s</td>", (int) lengths[i], row[i]);
   						}
   						puts("</tr>");

   						row = mysql_fetch_row(res);
   					}

   					puts("</table>");
				}

   			} else {
   				num_rows = mysql_affected_rows(&mysql);
   				printf("<div class=\"container\"><div class=\"jumbotron\"><h1 style=\"text-align:center\">\
   					Query OK, %u row/s affected</h1></div></div>", num_rows);
   			}
		}	
	}

	mysql_close(&mysql);

}

int main(void) {
	char data[MAXLEN];					memset(data,MAXLEN,0);
	char database[DATALEN];			memset(database,DATALEN,0);
	char password[DATALEN];			memset(password,DATALEN,0);
	char query[DATALEN];			memset(query,DATALEN,0);
	char button[DATALEN];			memset(button,DATALEN,0);

	/*
		You may want to parse the data from here
		or you may do it elsewhere if you want.
	*/

	cgi_init();
	start_html("MySQL Web Client");

	get_data(get_length(), &data[0]);

	get_element(&data[0],"database", &database[0]);
	database[strlen(database) - 2] = 0;
//	printf("    <br>database=\"%s\"", database);

	get_element(&data[0],"password", &password[0]);
	password[strlen(password) - 2] = 0;
//	printf("    <br>password=\"%s\"", password);

	get_element(&data[0],"query", &query[0]);
	query[strlen(query) - 2] = 0;

	get_element(&data[0],"button", &button[0]);
	button[strlen(button) - 2] = 0;
//	printf("    <br>button=\"%s\"", button);

	if(!strncmp(button,"Log in",6)) {
		initial_page(password, database);
	}
	
	if(!strncmp(button,"Send Query",10)) {
		result_page(password, database, query);
	}

	end_html();
	return 0;

}
