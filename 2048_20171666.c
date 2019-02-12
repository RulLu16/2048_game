#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
}


void rotate(int b[4][4]){
	int arr[4][4];
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			arr[i][j]=b[i][j];
		}
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			b[i][j]=arr[j][3-i];
		}
	}
}

int set_board(int dir, int b[4][4])//dir=command 
{

	int i,x,y,j;
	int check=0;
	int c[4];
	int temp[4][4];
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			temp[i][j]=b[i][j];
		}
	}
	for(i=4-dir;i>0;i--)
	{
		rotate(b);
	}

	for(x=0;x<4;x++)
	{
		for(y=3;y>=0;y--)
		{
			c[y]=0;
		}
		j=3;
		for(y=3;y>=0;y--)
		{
			if(b[x][y]!=0)
			{
				c[j]=b[x][y];
				j--;
			}
		}
		for(y=3;y>0;y--)
		{
			if(c[y]==c[y-1])
			{
				c[y]=c[y]*2;
				c[y-1]=0;
			}
		}
		for(y=3;y>=0;y--)
		{
			b[x][y]=0;
		}
		j=3;
		for(y=3;y>=0;y--)
		{
			if(c[y]!=0)
			{
				b[x][j]=c[y];
				j--;
			}
		}	

	}
	for(i=dir;i>0;i--)
	{
		rotate(b);
	}

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(temp[i][j]==b[i][j]) check++;
		}
	}
	if(check==16) return 0;
	else return 1;
}

int is_game_over(){
	int temp[4][4];
	int i,j,check=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			temp[i][j]=b[i][j];
		}
	}
	for(i=0;i<4;i++)
	{
		if(set_board(i,temp)==1) check++;
	}
	if(check==0) return 1;
	else return 0;


}

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score;
	int sum=0,n;

	/* console clear */
	system("clear");

	score = 0;
	for(i=0;i<4;i++)
	{
		j=0;
		if(score<b[i][j]) score=b[i][j];

		for(j=0;j<3;j++)
		{
			if(score<b[i][j+1]) score=b[i][j+1];
		}
	}

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			n=b[i][j];
			sum=sum+n;
		}
	}
	if(sum==tot)
	{

		printf("    Score : %d\n", score);

		fprintf(fp, "%d %d\n", score, command);

		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				c[i][j] = 32;
				for(k=0; k<50; k++){
					if( b[i][j] == (1<<k) ){
						c[i][j] = 32 + (k%6);
						break;
					}
				}

			}
		}
		for(i=0; i<4; i++){
			for(j=0; j<4; j++)
				fprintf(fp, "%d ", b[i][j]);
			fprintf(fp, "\n");
		}

		for(i=0; i<4; i++)
		{
			puts("");
			puts("");
			for(j=0; j<4; j++)
				printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
			puts("");
			puts("");
		}
	}

}

int make_two_or_four(){
	int i,j,v=0;//v=빈공간
	int k,h;
	int num,check=0;
	h=rand()%3;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(b[i][j]==0) v++;
		}
	}
	if(v==0) return 0;
	else 
	{
		if(h==0||h==1) num=2;
		else num=4;
		k=rand()%v;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(b[i][j]==0) check++;
				if(check==k+1) break;
			}
			if(check==k+1) break;
		}
		b[i][j]=num;
		return num;
	}

}


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
		case 'd':
		case 'D': return 0;
		case 'w':
		case 'W': return 1;
		case 'a':
		case 'A': return 2;
		case 's':
		case 'S': return 3;
		default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	int over=0,num;
	int set;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));
	int tot = 0;

	num=make_two_or_four();
	tot=tot+num;
	num=make_two_or_four();
	tot=tot+num;
	draw_board(tot, -1);


	do {
		command = GetCommand();

		if (command != -1) {

			if( set_board(command,b) ){
				num=make_two_or_four();
				over=is_game_over();
				tot=tot+num;
				draw_board(tot, command);
			}


		}

	} while (over!=1);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}
