#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "dot_matrix_font2.h"

#define CCHAR 0
#define MAX_BUTTON 9
#define LEFT 11
#define RIGHT 12
#define DOWN 13
#define ROTATE 14

unsigned char quit = 0;
unsigned char push_sw_buff[MAX_BUTTON];

void user_signal1(int sig){ quit = 1;}

void create();
void Initialize(int dev);
void draw(int dev);
void game_over(int dev, int dev3, int dev4);
void move_block(int a, int dev, int dev3, int dev4);
void displayScore(int dev3);
int detect_floor();
int detect_block();
int detect_left();
int detect_right();
int detect_top();
void erase_line(int dev, int dev3);
int delete();
int check_level(int dev, int level, int dev3);
void heap_block(int dev, int dev3);
void draw_clear(int dev);
int getch(void);
int key();
int R_turn(int dev2);

int buff_size;
int X = 2;
int Y = 0;
int rotate;
int INDEX;
int SCORE = 0;
int LEVEL = 1;
int score_num = 0;

// FND
char score[4];
unsigned char fnd_data[4];

int main(void)
{
				int input_key = 0;
				int rotate_key = 0;
				int i;
				int a, b;

   				int dev=open("/dev/csemad_dot_matrix", O_WRONLY);
				int dev2=open("/dev/csemad_push_switch", O_RDONLY);
				int dev3=open("/dev/csemad_fnd", O_RDWR);
				int dev4=open("/dev/csemad_led", O_RDWR);


				if(dev < 0 || dev2<0 || dev3<0 || dev4<0) 
				{
					printf("failed to open\n");
					return -1;
				}

				(void)signal(SIGINT, user_signal1);
				printf("Press <ctrl+c> to quit. \n");

				create();
				
				printf("Input the LEVEL!\n");
				scanf("%d", &a);

				if (a == 1)
				{
					LEVEL = 1;
					b = 128;
					write(dev4, &b, 1);
					score_num=1000;
					displayScore(dev3);
				}
				else if (a == 2)
				{
					LEVEL = 2;
					b = 192;
					write(dev4, &b, 1);
					score_num=2000;
					displayScore(dev3);

				}
				else if (a == 3)
				{
					LEVEL = 3;
					b = 224;
					write(dev4, &b, 1);
					score_num=3000;
					displayScore(dev3);

				}

				buff_size = sizeof(push_sw_buff);

				for(i=0; i<6; i++)
				{
					write(dev, dot_first[i], sizeof(dot_first[i]));
					usleep(300000);
					usleep(300000);
				}

				while (!quit)
				{
					usleep(300000);
					
					while(!quit)
					{

						draw(dev);
						move_block(DOWN, dev, dev3, dev4);
						
						input_key = R_turn(dev2);
						rotate_key = R_turn(dev2);
						
						if (11<= input_key && input_key<=13)
						{
								move_block(input_key, dev, dev3, dev4);
								draw(dev);
								usleep(10000);
						}

						if(rotate_key == ROTATE)
						{
								rotate = (rotate + 1) % 4;
						}

						if (LEVEL == 1)
						{
								usleep(800000);
						}
						else if (LEVEL == 2)
						{
								usleep(600000);
						}
						else if (LEVEL == 3)
						{
								usleep(400000);
						}
					}
				}
				close(dev);
				close(dev2);
				close(dev3);
				close(dev4);

				return 0;
}

void create()
{
   				int index;
   				srand(time(NULL));
   				index = rand() % 4;
   				INDEX = index;
   				X = 2;
   				Y = 0;
   				rotate = 0;
}

void Initialize(int dev)
{
   				int i = 0;
   				for (i = 0; i<10; i++)
   				{
      					matrix_display[i] = 0;

   				}
   				write(dev, matrix_display, sizeof(matrix_display));
}

void draw(int dev)
{

   				char temp_display[10] = { 0 };
   				int i;
   				int j = 0;
   				for (i = 0;i<10;i++)
   				{
      					temp_display[i] = matrix_display[i];
   				}

   				for (i = Y; i<Y + 3; i++)
  				{
      					temp_display[i] = matrix_display[i] | dot_tetris[INDEX][rotate][j++] << X;
  				}


   				write(dev, temp_display, sizeof(temp_display));
}

void game_over(int dev, int dev3, int dev4)
{
   				score_num = 9999;
				displayScore(dev3);
   				int i = 0;
				int a, b;

   				for (i = 0; i < 20; i++)
  				{
      					write(dev, dot_matrix_full, sizeof(dot_matrix_full));
      					usleep(50000);
      					write(dev, dot_matrix_blank, sizeof(dot_matrix_blank));
      					usleep(50000);
   				}
				printf("Input the LEVEL!\n");
				scanf("%d", &a);

				if (a == 1)
				{
					LEVEL = 1;
					b = 128;
					write(dev4, &b, 1);
					score_num=1000;
					displayScore(dev3);
				}
				else if (a == 2)
				{
					LEVEL = 2;
					b = 192;
					write(dev4, &b, 1);
					score_num=2000;
					displayScore(dev3);

				}
				else if (a == 3)
				{
					LEVEL = 3;
					b = 224;
					write(dev4, &b, 1);
					score_num=3000;
					displayScore(dev3);

				}


  				for(i=0; i<6; i++)
				{
					write(dev, dot_first[i], sizeof(dot_first[i]));
					usleep(300000);
					usleep(300000);
				}

}

void move_block(int a, int dev, int dev3, int dev4)
{

   				switch (a)
   				{
   					case LEFT:
     						if (!detect_left())
      						{
         						X++;
      						}
         					break;
   					case RIGHT:
      						if (!detect_right())
      						{
         						X--;
						}
    						break;
  					 case DOWN:
      						if (detect_floor() || detect_block())
      						{
         						heap_block(dev, dev3);
         						if (detect_top())
         						{
            							game_over(dev, dev3, dev4);
           								Initialize(dev);
         						}
         						create();
      						}
      						else
						{
         						Y++;
						}
     	 					break;

   				   	default:
      						break;
   					}
}
int detect_floor()
{
   				if (Y == 7)
   				{
      					if ((dot_tetris[INDEX][rotate][2] << X) & 0x7f) 
						return 1;
   				}

  				if (Y == 8)
   				{
      					if ((dot_tetris[INDEX][rotate][1] << X) & 0x7f)
           					return 1;
  				}

   				if (Y == 9)
  				{

      					if ((dot_tetris[INDEX][rotate][0] << X) & 0x7f)
           					return 1;
  				}

  				return 0;
}

int detect_block()
{
   				int i = 0;
   				for (i = 0;i<3;i++)
   				{
      					if (matrix_display[i + Y + 1] && (dot_tetris[INDEX][rotate][i] << X))
         					return 1;
   				}

   				return 0;
}

void heap_block(int dev, int dev3)
{

   				int i = 0;
   				for (i = 0;i<3;i++)
   				{
     					matrix_display[i + Y] |= (dot_tetris[INDEX][rotate][i] << X);
   				}

   				draw(dev);
				score_num+=5;
				displayScore(dev3);

   				if (delete())
				{
      					erase_line(dev, dev3);
				}
}

int detect_left()
{
   				int i = 0;
   				for (i = 0;i<3;i++)
   				{
      					if (0x40 & (dot_tetris[INDEX][rotate][i] << X))
         					return 1;
   				}

   				return 0;
}

int detect_right()
{
   				int i = 0;
   				for (i = 0;i<3;i++)
   				{
      					if (0x1 & (dot_tetris[INDEX][rotate][i] << X))
         					return 1;
   				}

   				return 0;
}

int detect_top()
{
   				if (matrix_display[0] & 0x7f)
   				{
      					return 1;
   				}
   				return 0;
}

int delete()
{
   				int i = 0;
   				for (i = 0; i< 10; i++)
   				{
      					if (matrix_display[i] == 0x7f)
         					return 1;
   				}

   				return 0;
}

void erase_line(int dev, int dev3)
{

   				score_num += 10;
				displayScore(dev3);				
   				
   				int i = 0;
   				int j = 9;

   				while (j != 0)
   				{

      					if (matrix_display[j] == 0x7f)
      					{
         					for (i = 0;i<4;i++)
        					{
            						matrix_display[j] = 0x0;

            						usleep(80000);

            						write(dev, matrix_display, sizeof(matrix_display));
            						matrix_display[j] = 0x7f;

            						usleep(80000);

            						write(dev, matrix_display, sizeof(matrix_display));

         					}

         					for (i = 0;i<6;i++)
         					{
            						matrix_display[j] = matrix_display[j] ^ (0x1 << i);
           		 					write(dev, matrix_display, sizeof(matrix_display));

            						usleep(80000);
         					}

         					for (i = j; i >= 0; i--)
         					{
            						matrix_display[i] = matrix_display[i - 1];
         					}

         					SCORE++;
      					}

      					if (check_level(dev, LEVEL, dev3))
      					{
         					Initialize(dev);
         					create();
      					}

      					j--;

   				}

}

int check_level(int dev, int level, int dev3) //LEVEL1 score exist-> LEVEL UP
{

   				if (level == 1)
				{
      					if (SCORE == 1) 
					{
         					LEVEL = 2;
         					SCORE = 0;
         					score_num = 2000;
         					
         					write(dev, dot_matrix_blank, sizeof(dot_matrix_blank));
							draw_clear(dev);

         					usleep(500000);

         					write(dev, dot_matrix_font[2], sizeof(dot_matrix_font[2]));
         					usleep(500000);

         					displayScore(dev3);

         					return 1;
      					}
   				}
   				else if (level == 2)
   				{
      					if (SCORE == 2)
      					{
         					LEVEL = 3;
         					SCORE = 0;
         					score_num = 3000;
         					
         					write(dev, dot_matrix_blank, sizeof(dot_matrix_blank));
         					draw_clear(dev);

         					usleep(500000);

         					write(dev, dot_matrix_font[3], sizeof(dot_matrix_font[3]));

         					usleep(500000);

         					displayScore(dev3);

         					return 1;
      					}
   				}
   				else if (level == 3)
   				{
      					if (SCORE == 3)
      					{

					         int i = 0;

					         for (i = 0;i<10;i++)
					         {
					            	score_num = 0;
					            	write(dev, dot_matrix_blank, sizeof(dot_matrix_blank));
					            	write(dev, dot_matrix_full, sizeof(dot_matrix_full));
					         }
			
					         return 1;
					}
					
				}

   				return 0;
}

void draw_clear(int dev)
{

			   	unsigned char levelup_matrix[10] = { 0 };
			   	int i = 0;

			   	for (i = 9;i>4;i--)
			   	{
			      		levelup_matrix[i] = (0x1 << 3);
			      		write(dev, levelup_matrix, sizeof(levelup_matrix));

			      		usleep(100000);
			   	}
			
			   	for (i = 0;i<3;i++)
			   	{
			      		write(dev, dot_matrix_levelup[i], sizeof(dot_matrix_levelup[i]));

			      		usleep(500000);
			   	}
			
			   	for (i = 0;i<10;i++)
			   	{
			      		write(dev, dot_matrix_levelup[0], sizeof(dot_matrix_levelup[0]));
			      		usleep(70000);

			      		write(dev, dot_matrix_levelup[1], sizeof(dot_matrix_levelup[1]));
			      		usleep(70000);

			      		write(dev, dot_matrix_levelup[2], sizeof(dot_matrix_levelup[2]));
			      		usleep(70000);

			      		write(dev, dot_matrix_levelup[1], sizeof(dot_matrix_levelup[1]));
			      		usleep(70000);
			   	}
}


/*int getch(void)
{
  				char ch;
				int error;
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
				Ntty.c_cc[VMIN] = CCHAR;
				Ntty.c_cc[VTIME] = CTIME;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

				if(0 == (error = tcsetattr(0, FLAG, &Ntty)))
				{
					error = read(0, &ch, 1);
					error += tcsetattr(0, FLAG, &Otty);
				}

				if(error == 1)
				{
					return (int)ch;
				}
				else
					return -1;
}

int key()
{		
				char ch;
				int input=0;
				
				printf("Input the key!\n");
				ch=getch();

				switch(ch)
				{
							case 97: 
								input=LEFT;
								break;
							case 100: 
								input=RIGHT;
								break;
							case 115:
								input=DOWN;
								break;
							default:
								break;
				};
				
				return input;
}*/

void displayScore(int dev3)
{
				sprintf(score, "%d", score_num);
				int i;
				unsigned char ret;
					
				for(i=0; i<4; i++)
				{
								fnd_data[i]=score[i]-0x30;
				}
		
				ret=write(dev3, &fnd_data, 4);
	
				if(ret<0)
				{
								printf("Error\n");
				}

}

int R_turn(int dev2)
{
				
				buff_size = sizeof(push_sw_buff);
				
				read(dev2, push_sw_buff, buff_size);

				if(push_sw_buff[4]==1) return ROTATE;
				else if(push_sw_buff[6]==1) return LEFT;
				else if(push_sw_buff[7]==1) return DOWN;
				else if(push_sw_buff[8]==1) return RIGHT;

				return -1;
}
