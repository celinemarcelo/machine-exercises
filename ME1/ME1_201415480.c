#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main (void){												 
	char s[72] = {}, c;
	char *num, *uname;
	char uname_input[32] = {}, pw_input[32] = {}; 
	int i = 0, j = 0, id = 0, same = 0, cap = 0, not_alnum = 0; 
	FILE *fp = fopen("users.txt", "r+");						//opens users.txt with read/write privilege
	
	puts("| -- Create account -- |");
	
	while (1){
	
		printf("> Enter a username: ");
		fgets(uname_input, 30, stdin);
		uname_input[strlen(uname_input) - 1] = 0;				//eliminates newline character
		same = 0;		
		not_alnum = 0;										
		
		rewind(fp);												//re-initializes file pointer to beginning of file
		c = fgetc(fp);											//re-initializes c to beginning of file
		
		while (c != EOF){										//c checks for EOF character (will indicate when to stop reading from file)
			
			fseek(fp, -1, SEEK_CUR);							//moves fp one step backwards
			fgets(s, 40, fp);									//stores one line from users.txt into s
			
			num = strtok(s, ": ");								//conversion of string id into integer
			id = atoi(num);										
			
			uname = strtok(NULL, ": ");
			i = strcmp(uname_input, uname);	
			
			c = fgetc(fp);
			
			if(!i){												//checks if uname_input is unique
				puts("Username already exists, try again.");
				same = 1;
				continue;
			} else {											//checks if uname_input is alphanumeric
			
				for(j = 0; j < strlen(uname_input); j++){
					if (!isalnum(uname_input[j])){
						not_alnum++;
					}
				}
			}

		}
		
		if(same){
			continue;
		} else if(not_alnum){
			puts("Invalid username, try again.");
			continue;
		} else {
			break;
		}
	}
	
	
	while(1){
		printf("> Enter a password: ");
		fgets(pw_input, 30, stdin);
		pw_input[strlen(pw_input) - 1] = 0;
		
		if(strlen(pw_input) < 4 || strlen(pw_input) > 14){		//checks if length of pw_input is within the specified range (4 <= length >= 14)
			puts("Password is invalid, try again.");
			continue;
		} else {
			cap = 0;
			not_alnum = 0;
		
			for(i = 0; i < strlen(pw_input); i++){				//checks for at least two capital letters in pw_input			
				if(isupper(pw_input[i])){
					cap++;
				}
			
				if (!isalnum(pw_input[i])){						//checks if pw_input is alphanumeric
					not_alnum++;
				}
			}
			
			if(not_alnum || cap < 2){
				puts("Password is invalid, try again.");
				continue;
			}
			
		}
		
		break;
	}
		
	id++;														//increments existing id by 1
	
	fseek(fp, 0, SEEK_END);										//moves fp to end of file
	fprintf(fp, "\n%d : %s : %s", id, uname_input, pw_input);	//writes the correct data inputs unto users.txt
	
	fclose(fp);
		
	return 0;													//end of program
}
