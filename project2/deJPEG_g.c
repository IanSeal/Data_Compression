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
int bNums, wNums, hNums;
int forOne = 0x00000001;
int forZero = 0xFFFFFFFE;
const unsigned char one = 0x01;

void readImg(float ***img, char *buff);
void bin2str(unsigned char byte, char *buff);
int bin2int(char *input);
void deDC(int *prevDC, int *currDC, char **buff_seek);
void deAC(struct AC_list **ac_list, char **buff_seek);
void izz(float **block, struct blockInfo *bInfo);
void iquanBlock(float **block);
void mergeBlock(float **block, float **img, int index);
void writeRaw(float **img);

int main(int argc, char **argv)
{
	float **img, **block;
	int index, prevDC = 0;
	FILE *output;
	char *buff = (char*)malloc(sizeof(char)*BUFF_SIZE);
	memset(buff, 0, sizeof(char)*BUFF_SIZE);
	struct blockInfo *bInfo = (struct blockInfo*)malloc(sizeof(struct blockInfo));
	memset(bInfo, 0, sizeof(struct blockInfo));
	block = (float**)malloc(sizeof(float*)*8);
	for( index=0; index<8; index++)
		block[index] = (float*)malloc(sizeof(float)*8);
	char *buff_seek = buff;

	readImg(&img, buff);
	for( index=0; index<bNums; index++){
		deDC(&prevDC, &(bInfo->dc), &buff_seek);
		deAC(&(bInfo->ac), &buff_seek);
		izz(block, bInfo);
		iquanBlock(block);
		idct2(block, 8);
		mergeBlock(block, img, index);
		printf("Block %d fin.\n", index);
	}
	writeRaw(img);
	puts("Decode done!");

	return 0;
}

void readImg(float ***img, char *buff)
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
	int i;
	for( i=0; i<height; i++)
		(*img)[i] = (float *)malloc(sizeof(float)*width);
	
	wNums = width/8;
	hNums = height/8;
	bNums = wNums*hNums;

	unsigned char byte;
	while( fread(&byte, 1, 1, imgFile)!=0 )
		bin2str(byte, buff);
	
	fclose(imgFile);
}

void bin2str(unsigned char byte, char *buff)
{
	int i;
	for( i=7; i>=0; i--){
		if( (byte>>i) & one )
			sprintf( buff + strlen(buff), "1");
		else
			sprintf( buff + strlen(buff), "0");
	}
}

int bin2int(char *input)
{
	int i, result = 0x00000000;
	for( i=0; input[i]!='\0'; i++){
		result <<= 1;
		if( input[i] == '1' )
			result |= forOne;
		else
			result &= forZero;
	}
	return result;
}

void deDC(int *prevDC, int *currDC, char **buff_seek)
{
	char binStr[11];
	int i, j, isfind = 0;
	int ssss, isNeg = 0;
	int diff;
	for( i=0; i<10; i++){
		binStr[i] = (*buff_seek)[i];
		binStr[i+1] = '\0';
		for( j=0; j<12; j++)
			if( strcmp(binStr, DCtable[j])==0 ){
				isfind = 1;
				break;
			}
		if( isfind == 1 ){
			ssss = j;
			*buff_seek += (i+1);
			break;
		}
	}
	if( isfind == 0 ){
		puts("Decode DC error!");
		exit(-1);
	}
	
	for( i=0; i<ssss; i++)
		binStr[i] = (*buff_seek)[i];
	binStr[ssss] = '\0';
	*buff_seek += ssss;
	
	if( binStr[0] == '0'){
		isNeg = 1;
		for( i=0; i<ssss; i++){
			if( binStr[i] == '0' )
				binStr[i] = '1';
			else
				binStr[i] = '0';
		}
	}
	
	diff = bin2int(binStr);
	if( isNeg )
		diff *= -1;

	*currDC = *prevDC + diff;
	*prevDC = *currDC;
}

void deAC(struct AC_list **ac_list, char **buff_seek)
{
	int size;
	int val;
	int isfind = 0, isNeg = 0;
	int i, j, k;
	char binStr[18];
	struct AC_list *ac_curr;
	
	if( *ac_list == NULL ){
		*ac_list = (struct AC_list *)malloc(sizeof(struct AC_list));
		memset(*ac_list, 0, sizeof(struct AC_list));
	}
	ac_curr = *ac_list;

	while(1){
		for( i=0; i<17; i++){
			binStr[i] = (*buff_seek)[i];
			binStr[i+1] = '\0';
			for( j=0; j<16; j++){
				for( k=0; k<11; k++)
					if( strcmp(binStr, ACtable[j][k])==0 ){
						isfind = 1;
						break;
					}
				if( isfind == 1)
					break;
			}
			if( isfind == 1 ){
				ac_curr->run = j;
				size = k;
				*buff_seek += (i+1);
				break;
			}
		}
		if( isfind == 0 ){
			puts("Decode AC error!");
			exit(-1);
		}
		else if( j==0 && k==0 ){
			ac_curr->run = -1;
			break;
		}

		isfind = 0;
		for( i=0; i<size; i++)
			binStr[i] = (*buff_seek)[i];
		binStr[size] = '\0';
		*buff_seek += size;
	
		if( binStr[0] == '0'){
			isNeg = 1;
			for( i=0; i<size; i++){
				if( binStr[i] == '0' )
					binStr[i] = '1';
				else
					binStr[i] = '0';
			}
		}
		val = bin2int(binStr);
		if( isNeg )
			val *= -1;

		ac_curr->val = val;
		if( ac_curr->next == NULL ){
			ac_curr->next = (struct AC_list*)malloc(sizeof(struct AC_list));
			memset(ac_curr->next, 0, sizeof(struct AC_list));
		}

		ac_curr = ac_curr->next;
	}
}

void izz(float **block, struct blockInfo *bInfo)
{
	int i, j, index;
	int run_curr;
	float val;
	struct AC_list *ac_curr;

	block[0][0] = (float)bInfo->dc;
	ac_curr = bInfo->ac;

	run_curr = ac_curr->run;
	for( index=1; index<64; index++){
		j = ZigZagSequenceGray64[index][0];
		i = ZigZagSequenceGray64[index][1];
		if( run_curr > 0 ){
			val = 0.0;
			run_curr--;
		}
		else if( run_curr == 0 ){
			val = (float)ac_curr->val;
			ac_curr = ac_curr->next;
			run_curr = ac_curr->run;
		}
		else{
			int endBlock, m, n;
			for( endBlock=index; endBlock<64; endBlock++){
				n = ZigZagSequenceGray64[index][0];
				m = ZigZagSequenceGray64[index][1];
				block[m][n] = 0.0;
			}
		}
		block[i][j] = val;
	}
}

void iquanBlock(float **block)
{
	int i, j;
	for( i=0; i<8; i++)
		for( j=0; j<8; j++){
			block[i][j] = block[i][j]*Qtable[i][j];
			block[i][j] = round(block[i][j]);
		}
}

void mergeBlock(float **block, float **img, int index)
{
	int i, j;
	int row = index/wNums;
	int col = index%wNums;
	for( i=0; i<8; i++)
		for( j=0; j<8; j++)
			img[row*8+i][col*8+j] = block[i][j];
}

void writeRaw(float **img)
{
	int i, j;
	FILE *output;
	unsigned char byte;

	output = fopen("output.raw", "wb");
	if( output == NULL){
		puts("Cannot write raw file!");
		exit(-1);
	}
	for( i=0; i<height; i++)
		for( j=0; j<width; j++){
			byte = (unsigned char)img[i][j];
			fwrite(&byte, sizeof(unsigned char), 1, output);
		}
	fclose(output);
}

