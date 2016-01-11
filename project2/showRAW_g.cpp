#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#define NAME_SIZE 256
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	char filename[NAME_SIZE];
	int width, height;
	int row, col;
	unsigned char byte;
	fstream raw;

	cout<<"Enter image name: ";
	cin>>filename;
	filename[NAME_SIZE-1] = '\0';
	cout<<"Enter image width: ";
	cin>>width;
	cout<<"Enter image height: ";
	cin>>height;

	Mat image(height, width, CV_8UC1);
	raw.open(filename, ios::in | ios::binary);
	if(!raw){
		cout<<"Cannot open image!"<<endl;
		return -1;
	}
	for( row=0; row<height; row++)
		for( col=0; col<width; col++){
			if(raw.eof()){
				cout<<"Reading image error!"<<endl;
				return -1;
			}
			byte = (unsigned char)raw.get();
			image.at<uchar>(row, col) = byte;
		}
	
	imshow("Result", image);
	waitKey(0);
	
	return 0;
}


