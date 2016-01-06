#ifndef _TABLE_H_
#define _TABLE_H_

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

#endif