#include "GPIOlib.h"
#include <bits/stdc++.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include  <opencv2/opencv.hpp>

const double PI = 3.1415926535897932384;

using namespace GPIO;
using namespace cv;
using namespace std;
const string CAM_PATH = "/dev/video0";
const string MAIN_WINDOW_NAME = "Processed Image";
const string CANNY_WINDOW_NAME = "Canny";

const int CANNY_LOWER_BOUND = 50;
const int CANNY_UPPER_BOUND = 250;
const int HOUGH_THRESHOLD = 150;
const double DELTA = 30;
typedef pair<Point,Point> LINE;
pair<double,double> get_cross(LINE line1,LINE line2 ){
	return {-1,-1};
}
void controlDirection(VideoCapture & capture){
	Mat image;
	capture>> image;
	if (image.empty())return ;
	Rect roi(0,0,image.cols,image.rows);
	Mat imgRoi = image(roi);
	Mat contours;
	Canny(imgRoi,contours,CANNY_LOWER_BOUND,CANNY_UPPER_BOUND);
	#ifdef _DEBUG
	imshow(CANNY_WINDOW_NAME,contours);
	#endif	
	vector<Vec2f>lines;
	HoughLines(contours,lines,1,PI/180,HOUGH_THRESHOLD);
	Mat result(imgRoi.size(),CV_8U,Scalar(255));
	imgRoi.copyTo(result);
	clog<<lines.size()<<endl;
	float maxRad = -2*PI;
	float minRad = 2*PI;
	vector<LINE > ls;
	for (vector<Vec2f>::const_iterator it = lines.begin();it != lines.end();it++){
		float rho = (*it)[0];
		float theta = (*it)[1];
		if ((theta>0.09 && theta< 1.48) or (theta>1.62 && theta < 3.05)){
			if (theta > maxRad)maxRad = theta;
			if (theta < minRad)minRad = theta;
			Point pt1 = (rho/cos(theta),0);
			Point pt2 = ((rho - result.rows*sin(theta))/cos(theta),result.rows);
			ls.push_back({pt1,pt2});
		}
	}
	if (ls.size() <2)return;
	pair<double,double > l1 = ls[0],l2 = ls[1];
	pair<double,double> crossPoint = get_cross(l1,l2);
	if (crossPoint.second < image.rows/2 - DELTA){
		controlLeft(FORWARD,11);
		controlRight(FORWARD,10);
	}else if (crossPoint.second > image.rows/2 + DELTA){
		controlLeft(FORWARD,10);
		controlRight(FORWARD,11);
	}
}


int main()
{
	init();
	VideoCapture capture(CAM_PATH);
	if (!capture.isOpened()){
		capture.open(atoi(CAM_PATH.c_str()));
	}
	Mat image;
	for (int i=1;i<=3;i++){
		controlDirection(capture);
		if (i&1){
			controlLeft(FORWARD,10);
			controlRight(FORWARD,10);
		}else{
			controlRight(FORWARD,10);
			controlLeft(FORWARD,10);
		}
		delay(200);
	}
	stopLeft();
	stopRight();
	return 0;


}
