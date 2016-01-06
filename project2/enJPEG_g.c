#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include"dct.h"
#include"tables.h"

#define NAME_SIZE 256
#define BUFF_SIZE 1000000

struct AC_list{
	int run;	// -1 means EOB
	int val;
	struct AC_list *next;
};

struct blockInfo{
    int dc;
	struct AC_list *ac;
};


int width, height;
int bNums;
int bit_num = 0;
unsigned char oneByte = 0x00;
const unsigned char forOne = 0x01;
const unsigned char forZero = 0xFE;

void readImg(float ***img);
void cutBlock(float **img, float ****blocks);
void printMatrix(float **input, int width, int height); //for debug
void quanBlock(float **block);
void int2bin(int input, int counts, char *result);
void enDC(int *prevDC, int currDC, char *buff);
void enAC(struct AC_list *ac_list, char *buff);
void zz(float **block, struct blockInfo *bInfo);
void writeByte(char bit, FILE **output);
void finishByte(FILE **output);

int main(int argc, char **argv)
{
	float **img, ***blocks;
	int index, prevDC = 0;
	FILE *output;
	char *buff = (char*)malloc(sizeof(char)*BUFF_SIZE);
	memset(buff, 0, sizeof(char)*BUFF_SIZE);
	struct blockInfo *bInfo = (struct blockInfo*)malloc(sizeof(struct blockInfo));
	memset(bInfo, 0, sizeof(struct blockInfo));

	readImg(&img);
	cutBlock(img, &blocks);
	for( index=0; index<bNums; index++ ){
		dct2(blocks[index], 8);
		quanBlock(blocks[index]);
		zz(blocks[index], bInfo);
		enDC(&prevDC, bInfo->dc, buff);
		enAC(bInfo->ac, buff);
	}

	output = fopen("output", "wb");
	if( output == NULL ){
		puts("Cannot write output file!");
		exit(-1);
	}

	for( index=0; buff[index]!='\0'; index++ )
		writeByte(buff[index], &output);
	finishByte(&output);

	fclose(output);
	return 0;
}


void readImg(float ***img)
{
	FILE *imgFile;
	char filename[NAME_SIZE];

	printf("Enter image name: ");
	if( fgets(filename, NAME_SIZE, stdin) == NULL){
		puts("Bad image name!\n");
		exit(-1);
	}
	filename[strlen(filename)-1] = '\0';
	
	printf("Enter image width: ");
	if( scanf("%d", &width) != 1 ){
		puts("Bad image width!\n");
		exit(-1);
	}
	
	printf("Enter image height: ");
	if( scanf("%d", &height) != 1){
		puts("Bad image height!\n");
		exit(-1);
	}
	
	if( width%8 !=0 || height%8 != 0){
		puts("Bad image size!");
		exit(-1);
	}

	imgFile = fopen(filename, "rb");
	if( imgFile == NULL ){
		printf("Cannot open %s !\n", filename);
		exit(-1);
	}
	
	*img = (float **)malloc(sizeof(float *)*height);
	int i, j;
	unsigned char byte;
	for( i=0; i<height; i++ ){
		(*img)[i] = (float *)malloc(sizeof(float)*width);
		for( j=0; j<width; j++ ){
			if( fread( &byte, 1, 1, imgFile) == 0 ){
				puts("Reading image error!");
				exit(-1);
			}
			(*img)[i][j] = (float)byte;
		}
	}
	
	fclose(imgFile);
}

void cutBlock(float **img, float ****blocks)
{
	int wNums, hNums;
	wNums = width/8;
	hNums = height/8;
	bNums = wNums*hNums;
	*blocks = (float***)malloc(sizeof(float **)*bNums);
	int i, j;
	for( i=0; i<bNums; i++ ){
		(*blocks)[i] = (float**)malloc(sizeof(float *)*8);
		for( j=0; j<8; j++)
			(*blocks)[i][j] = (float*)malloc(sizeof(float)*8);
	}

	int row, col;	//OMG.... 4 nested loop... QQ
	for( row=0; row<hNums; row++ )
		for( col=0; col<wNums; col++ )
			for( i=0; i<8; i++ )
				for( j=0; j<8; j++ )
					(*blocks)[row*hNums+col][i][j] = img[row*8+i][col*8+j];
}

void printMatrix(float **input, int width, int height)
{
	int i, j;
	for( i=0; i<height; i++){
		for( j=0; j<width; j++)
			printf("%7.2f ", input[i][j]);
		puts("");
	}
}

void quanBlock(float **block)
{
	int i,j;
	for( i=0; i<8; i++)
		for( j=0; j<8; j++){
			block[i][j] = round(block[i][j]);
			block[i][j] = block[i][j]/Qtable[i][j];
			block[i][j] = round(block[i][j]);
		}
}

void int2bin(int input, int counts, char *result)
{
	int bit;
	char one = '1', zero = '0';
	if( input < 0 ){
		input *= -1;
		one = '0';
		zero = '1';
	}

	for( counts-=1; counts>=0; counts--){
		bit = input>>counts;
		if( bit & 1 )
			sprintf(result + strlen(result), "%c", one);
		else
			sprintf(result + strlen(result), "%c", zero);
	}
}

void enDC(int *prevDC, int currDC, char *buff)
{
	int ssss;
	int diff = currDC - *prevDC;
	
	if( diff == 0 )
		ssss = 0;
	else if( diff>=-1 && diff<=1 )
		ssss = 1;
	else if( diff>=-3 && diff<=3 )
		ssss = 2;
	else if( diff>=-7 && diff<=7 )
		ssss = 3;
	else if( diff>=-15 && diff<=15 )
		ssss = 4;	
	else if( diff>=-31 && diff<=31 )
		ssss = 5;
	else if( diff>=-63 && diff<=63 )
		ssss = 6;
	else if( diff>=-127 && diff<=127 )
		ssss = 7;
	else if( diff>=-255 && diff<=255 )
		ssss = 8;
	else if( diff>=-511 && diff<=511 )
		ssss = 9;
	else if( diff>=-1023 && diff<=1023 )
		ssss = 10;
	else if( diff>=-2047 && diff<=2047 )
		ssss = 11;
	else{
		puts("Encode DC error!");
		exit(-1);
	}

	sprintf(buff + strlen(buff), "%s", DCtable[ssss]);
	int2bin(diff, ssss, buff);
	*prevDC = currDC;

}

void enAC(struct AC_list *ac_curr, char *buff)
{
	int size;
	int val;
	
	while( ac_curr != NULL ){
		if( ac_curr->run < 0 ){
			sprintf(buff + strlen(buff), "1010");
			break;
		}

		val = ac_curr->val;
		if( val == 0 )
			size = 0;
		else if( val>=-1 && val<=1 )
        	size = 1;
    	else if( val>=-3 && val<=3 )
        	size = 2;
	    else if( val>=-7 && val<=7 )
    	    size = 3;
    	else if( val>=-15 && val<=15 )
        	size = 4;
    	else if( val>=-31 && val<=31 )
        	size = 5;
    	else if( val>=-63 && val<=63 )
        	size = 6;
    	else if( val>=-127 && val<=127 )
        	size = 7;
    	else if( val>=-255 && val<=255 )
        	size = 8;
    	else if( val>=-511 && val<=511 )
        	size = 9;
    	else if( val>=-1023 && val<=1023 )
        	size = 10;
    	else if( val>=-2047 && val<=2047 )
        	size = 11;
    	else{
        	puts("Encode AC error!");
        	exit(-1);
    	}

		sprintf(buff+ strlen(buff), "%s", ACtable[ac_curr->run][size]);
		int2bin(val, size, buff);
		
		ac_curr = ac_curr->next;
    }
}

void zz(float **block, struct blockInfo *bInfo)
{
	int val = 0;
	int zeros = 0;
	int index, i, j;
	int write = 0;
	struct AC_list *ac_curr;	

	bInfo->dc = (int)block[0][0];
	if( bInfo->ac == NULL ){
		bInfo->ac = (struct AC_list*)malloc(sizeof(struct AC_list));
		memset(bInfo->ac, 0, sizeof(struct AC_list));
	}
	ac_curr = bInfo->ac;
	
	for( index=1; index<64; index++ ){
		j = ZigZagSequenceGray64[index][0];
		i = ZigZagSequenceGray64[index][1];
		
		if( zeros == 15 ){
			val = (int)block[i][j];
			write = 1;
			if( val == 0 ){
				int findEOB, m, n;
				int isEOB = 1;
				for( findEOB=index+1; findEOB<64; findEOB++ ){
					n = ZigZagSequenceGray64[index][0];
					m = ZigZagSequenceGray64[index][1];
					if( (int)block[m][n] != 0 ){
						isEOB = 0;
						break;
					}
				}
				if( isEOB )
					zeros = -1;
			}
		}
		else if( (int)block[i][j] == 0 ){
			if( index == 63 ){
				zeros = -1;
				write = 1;
			}
			zeros++;
		}
		else{
			val = (int)block[i][j];
			write = 1;
		}
		
		if( write ){
			write = 0;
			ac_curr->run = zeros;
			ac_curr->val = val;
			if( zeros < 0 )
				break;
			if( ac_curr->next == NULL ){
				ac_curr->next = (struct AC_list*)malloc(sizeof(struct AC_list));
				memset(ac_curr->next, 0, sizeof(struct AC_list));
			}
			zeros = 0;
			ac_curr = ac_curr->next;
		}
	}
}

void writeByte(char bit, FILE **output)
{
	oneByte <<= 1;
	if( bit == '1' )
		oneByte |= forOne;
	else if( bit == '0' )
		oneByte &= forZero;
	else{
		puts("Write file error!");
		exit(-1);
	}
	bit_num++;
	if( bit_num == 8 ){
		bit_num = 0;
		fwrite(&oneByte, sizeof(unsigned char), 1, *output);
	}
}

void finishByte(FILE **output)
{
	if( bit_num != 0 ){
		for( bit_num; bit_num<8; bit_num++ ){
			oneByte <<= 1;
			oneByte &= forZero;
		}
		fwrite(&oneByte, sizeof(unsigned char), 1, *output);
	}
}



