#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void print_page(char buffer[]);
void read_page(FILE *file, char buffer[]);

char buffer[1000] = {0};
int count = 0;
int printed = 0;
int main(int argc,char *argv[])
{
	FILE* file = fopen(argv[1],"r");
	read_page(file, buffer);
	struct winsize ws;
	while(count < printed){
		char s = getchar();
		if(s == '\n'){
			print_page(buffer);
			printed++;
		}
		else if(s == 'b'){
			print_page(buffer-1);
			printed--;
		}
	}

	if (ioctl(0,TIOCGWINSZ,&ws)!=0) {
		fprintf(stderr,"TIOCGWINSZ:%s\n",strerror(errno));
		exit(-1);
	}
	printf("row=%d, col=%d, xpixel=%d, ypixel=%d\n",
	ws.ws_row,ws.ws_col,ws.ws_xpixel,ws.ws_ypixel);
	return 0;
}

void print_page(char buffer[]){
	for(int i = 0; i < 100; i++){
		printf("%s ", buffer[i]);
	}
	printf("\n");
}

void read_page(FILE *file, char buffer[]) {
	if(file == 0)return;
	while(file != 0){
		fgets(buffer, 1000, file);
		count++;
	}
	int i = 0;
	while ((i = read(0, buffer, sizeof(buffer))) != 0) {
		printf("%c\n", i);
		fflush(stdout);
		write(1, buffer, i);
	}
}
