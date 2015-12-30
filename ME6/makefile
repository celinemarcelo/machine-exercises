all:
	gcc -Wall -c -o ME4.o ME4_02.c
	gcc -Wall -c -o ME6.o ME6.c
	gcc -Wall -o ME6.cgi ME4.o ME6.o -L/opt/lampp/lib -lmysqlclient
	sudo cp ME6.html /opt/lampp/htdocs/
	sudo cp ME6.cgi /opt/lampp/cgi-bin/