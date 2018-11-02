#include "GPIOlib.h"
#include <bits/stdc++.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include  <opencv2/opencv.hpp>

const double PI = 3.1415926535897932384;
#ifndef _FAKE
using namespace GPIO;
#endif
using namespace cv;
using namespace std;
const string CAM_PATH = "/dev/video0";
const string MAIN_WINDOW_NAME = "Processed Image";
const string CANNY_WINDOW_NAME = "Canny";

const int CANNY_LOWER_BOUND = 50;
const int CANNY_UPPER_BOUND = 250;
const int HOUGH_THRESHOLD = 150;


/** parameter to adjust **/
const double LIMIT_K_LOWER_BOUND = 1;/** small is stable **/
const double LIMIT_K_UPPER_BOUND = 5;
const int normal_speed = 13;
const int delta_speed = 1;
const int TIME_INT = 150;
/**                     **/



int left_speed ,right_speed;
typedef pair<Point,Point> LINE;
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
			Point pt1(rho/cos(theta),0);
			Point pt2((rho - result.rows*sin(theta))/cos(theta),result.rows);
			ls.push_back({pt1,pt2});
#ifdef _DEBUG
                        line(result,pt1,pt2,Scalar(0,255,255),3,CV_AA);
#endif
		}
	}
        lines.clear();
        waitKey(1);
	if (ls.size() <2)return;
        double min_pos_k = 1e10;
        double min_neg_k = 1e10;
        for (int i=0;i<(int32_t)ls.size();i++){
                LINE & le = ls[i];
                double k = le.second.y - le.first.y;
                k /= (le.second.x - le.first.x);
                //cout<<le.first.x<<","<<le.first.y<<endl;
                //cout<<le.second.x<<","<<le.second.y<<endl;
                //cout<<"k="<<k<<endl;
#ifdef _DEBUG_MORE
                stringstream stm;
                stm<<"k = "<<k<<endl;
                putText(result,stm.str(),Point((ls[i].first.x + ls[i].second.x)/2+rand()%5,(ls[i].first.y + ls[i].second.y)/2+rand()%100),2,0.8,Scalar(0,0,255),0);
#endif

                if (k>0){
                        min_pos_k = min(min_pos_k,k);
                }else{
                        min_neg_k = min(min_neg_k,-k);
                }
        }
#ifdef _DEBUG
        imshow(MAIN_WINDOW_NAME,result);
#endif
        cout<<"min_pos_k: "<<min_pos_k<<endl;
        cout<<"min_neg_k: "<<min_neg_k<<endl;
        if (min_pos_k < LIMIT_K_LOWER_BOUND || min_neg_k >LIMIT_K_UPPER_BOUND){
                //turn left
                right_speed += delta_speed;
                cout<<"turning left"<<endl;
        }else if(min_neg_k < LIMIT_K_LOWER_BOUND || min_pos_k >LIMIT_K_UPPER_BOUND){
                //turn right
                left_speed += delta_speed;
                cout<<"turning right"<<endl;
        }     
}


int main()
{
#ifndef _FAKE
	init();
#endif
	VideoCapture capture(CAM_PATH);
	if (!capture.isOpened()){
		capture.open(atoi(CAM_PATH.c_str()));
	}
	Mat image;
#ifndef _FAKE
	for (int i=1;i<=100;i++){
#else
        while (1){
#endif
		left_speed = right_speed = normal_speed;
                controlDirection(capture);

#ifndef _FAKE
		controlLeft(FORWARD,10);
		controlRight(FORWARD,10);
#else
                cout<<"Left Speed = "<<left_speed<<endl;
                cout<<"Right Speed = "<<right_speed<<endl;        
#endif
#ifndef _FAKE
		delay(TIME_INT);
#endif
        }
#ifndef _FAKE
	stopLeft();
	stopRight();
#endif
	return 0;


}
