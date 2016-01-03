#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include"dct.h"

#define NAME_SIZE 256
#define BUFF_SIZE 1000000
const float Qtable[8][8] = {
	{ 16, 11, 10, 16, 24, 40, 51, 61 },
	{ 12, 12, 14, 19, 26, 58, 60, 55 }, 
	{ 14, 13, 16, 24, 40, 57, 69, 56 },
	{ 14, 17, 22, 29, 51, 87, 80, 62 },
	{ 18, 22, 37, 56, 68,109,103, 77 },
	{ 24, 36, 55, 64, 81,104,113, 92 },
	{ 49, 64, 78, 87,103,121,120,101 },
	{ 72, 92, 95, 98,112,100,103, 99 }
};

const char DCtable[12][10] = {
	"00",
	"010",
	"011",
	"100",
	"101",
	"110",
	"1110",
	"11110",
	"111110",
	"1111110",
	"11111110",
	"111111110"
};

const char ACtable[16][11][17] = {
	{// 0-0
		"1010",	//EOB
		"00",
		"01",
		"100",
		"1011",
		"11010",
		"1111000",
		"11111000",
		"1111110110",
		"1111111110000010",
		"1111111110000011"
	},
	{// 1-0
		"none",
		"1100",
		"11011",
		"1111001",
		"111110110",
		"11111110110",
		"1111111110000100",
		"1111111110000101",
		"1111111110000110",
		"1111111110000111",
		"1111111110001000"
	},
	{// 2-0
		"none",
		"11100",
		"11111001",
		"1111110111",
		"111111110100",
		"1111111110001001",
		"1111111110001010",
		"1111111110001011",
		"1111111110001100",
		"1111111110001101",
		"1111111110001110"
	},
	{// 3-0
		"none",
		"111010",
		"111110111",
		"111111110101",
		"1111111110001111",
		"1111111110010000",
		"1111111110010001",
		"1111111110010010",
		"1111111110010011",
		"1111111110010100",
		"1111111110010101"
	},
	{// 4-0
		"none",
		"111011",
		"1111111000",
		"1111111110010110",
		"1111111110010111",
		"1111111110011000",
		"1111111110011001",
		"1111111110011010",
		"1111111110011011",
		"1111111110011100",
		"1111111110011101"
	},
	{// 5-0
		"none",
		"1111010",
		"11111110111",
		"1111111110011110",
		"1111111110011111",
		"1111111110100000",
		"1111111110100001",
		"1111111110100010",
		"1111111110100011",
		"1111111110100100",
		"1111111110100101"
	},
	{// 6-0
		"none",
		"1111011",
		"111111110110",
		"1111111110100110",
		"1111111110100111",
		"1111111110101000",
		"1111111110101001",
		"1111111110101010",
		"1111111110101011",
		"1111111110101100",
		"1111111110101101"
	},
	{// 7-0
		"none",
		"11111010",
		"111111110111",
		"1111111110101110",
		"1111111110101111",
		"1111111110110000",
		"1111111110110001",
		"1111111110110010",
		"1111111110110011",
		"1111111110110100",
		"1111111110110101"
	},
	{// 8-0
		"none",
		"111111000",
		"111111111000000",
		"1111111110110110",
		"1111111110110111",
		"1111111110111000",
		"1111111110111001",
		"1111111110111010",
		"1111111110111011",
		"1111111110111100",
		"1111111110111101"
	},
	{// 9-0
		"none",
		"111111001",
		"1111111110111110",
		"1111111110111111",
		"1111111111000000",
		"1111111111000001",
		"1111111111000010",
		"1111111111000011",
		"1111111111000100",
		"1111111111000101",
		"1111111111000110"
	},
	{// 10-0
		"none",
		"111111010",
		"1111111111000111",
		"1111111111001000",
		"1111111111001001",
		"1111111111001010",
		"1111111111001011",
		"1111111111001100",
		"1111111111001101",
		"1111111111001110",
		"1111111111001111"
	},
	{// 11-0
		"none",
		"1111111001",
		"1111111111010000",
		"1111111111010001",
		"1111111111010010",
		"1111111111010011",
		"1111111111010100",
		"1111111111010101",
		"1111111111010110",
		"1111111111010111",
		"1111111111011000"
	},
	{// 12-0
		"none",
		"1111111010",
		"1111111111011001",
		"1111111111011010",
		"1111111111011011",
		"1111111111011100",
		"1111111111011101",
		"1111111111011110",
		"1111111111011111",
		"1111111111100000",
		"1111111111100001"
	},
	{// 13-0
		"none",
		"11111111000",
		"1111111111100010",
		"1111111111100011",
		"1111111111100100",
		"1111111111100101",
		"1111111111100110",
		"1111111111100111",
		"1111111111101000",
		"1111111111101001",
		"1111111111101010"
	},
	{// 14-0
		"none",
		"1111111111101011",
		"1111111111101100",
		"1111111111101101",
		"1111111111101110",
		"1111111111101111",
		"1111111111110000",
		"1111111111110001",
		"1111111111110010",
		"1111111111110011",
		"1111111111110100"
	},
	{// 15-0
		"11111111001",	//ZLR
		"1111111111110101",
		"1111111111110110",
		"1111111111110111",
		"1111111111111000",
		"1111111111111001",
		"1111111111111010",
		"1111111111111011",
		"1111111111111100",
		"1111111111111101",
		"1111111111111110"
	}
};

const int ZigZagSequenceGray64[64][2] = {   
    {0,0},                                              
    {1,0},{0,1},                                        
    {0,2},{1,1},{2,0},                                  
    {3,0},{2,1},{1,2},{0,3},                            
    {0,4},{1,3},{2,2},{3,1},{4,0},                      
    {5,0},{4,1},{3,2},{2,3},{1,4},{0,5},                
    {0,6},{1,5},{2,4},{3,3},{4,2},{5,1},{6,0},          
    {7,0},{6,1},{5,2},{4,3},{3,4},{2,5},{6,1},{0,7},        
    {1,7},{2,6},{3,5},{4,4},{5,3},{6,2},{7,1},
    {7,2},{6,3},{5,4},{4,5},{3,6},{2,7},                
    {3,7},{4,6},{5,5},{6,4},{7,3},                      
    {7,4},{6,5},{5,6},{4,7},                            
    {5,7},{6,6},{7,5},                                  
    {7,6},{6,7},                                        
    {7,7}                                               
};


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



