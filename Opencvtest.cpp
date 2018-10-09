#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include<fstream>
#include<string.h>
#include <time.h>

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	char theline [100] ;
	
	//read file
	fstream fin;
	fin.open("data.txt",ios::in);
	double data[400][100];


	//initial
	for(int i=0; i<400; i++){
		for(int j=0; j<100; j++){
			data[i][j] = 0 ;
		}
	}
	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	unsigned seed;
	
	

	int checkran[400] ;
	for(int i=0; i<400; i++){
		checkran[i] = 0;
	}

	
	int result = 0;

	int his[100];
	for(int i=0; i<100; i++){
		his[i] = 0;
	}

	//run 2000 times
	for(int t=0; t<2000; t++){
		bool again = true;
		double h = 0;
		double w[5] = {0,0,0,0,0};
		int start = 0;
		int countstop = 0;
		int count = 0;
		int number = 0;
		int order[400] ;
		for(int i=0; i<400; i++){
			checkran[i] = 0;
			order[i] = 0;
		}
		srand(t); // 以時間序列當亂數種子
		while(count!=399){
			number = rand()%400;
			if(checkran[number] == 1){

			}else{
				checkran[number] = 1;
				order[count] = number;
				count++;
			}
		}
		count = 0;
		//start PLA
		while(again){
			again = false;
			countstop++;
			number = order[start];
			double temp = w[0]*1 + w[1]*data[number][0] +  w[2]*data[number][1] + w[3]*data[number][2] + w[4]*data[number][3] ;
			if(temp>0){
				h = 1 ;
			}else{
				h = -1;
			}
			if(h == data[number][4]){

			}else{
				w[0] = w[0] + data[number][4] * 1;
				w[1] = w[1] + data[number][4] * data[number][0] ;
				w[2] = w[2] + data[number][4] * data[number][1] ;
				w[3] = w[3] + data[number][4] * data[number][2] ;
				w[4] = w[4] + data[number][4] * data[number][3] ;
				countstop = 0;
				count++;
			}
			start++;
			if(start==400){
				start = 0;
			}
			if(countstop!=400){
				again = true;
			}
			
		}
		result =result + count;
		his[count]++;
	}
	for(int i=0; i<100; i++){
		cout<<his[i]<<endl;
	}
	//find largest
	int largest=0;
	for(int i=0; i<100; i++){
		if(largest<his[i]){
			largest = his[i] ;
		}
	}

	//set largest
	for(int i=0; i<100; i++){
		his[i] = his[i]*400/largest;
	}

	//draw histogram
	Mat histImg (400,400,CV_8UC3,Scalar(255,255,255));
	for(int i=0; i<100; i++){
		line(histImg,Point(i*4,400-his[i]),Point(i*4,400),Scalar(0,0,0));
	}
	imshow("histogram",histImg);
	imwrite("histogram.jpg",histImg);

	result = result/2000;
	cout<<result<<endl;//update times
	
	waitKey(0);
	return 0;
}