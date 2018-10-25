#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>



using namespace cv;
using namespace std;

Point CenterPoint(vector<Point> points) {
	Point ret(0, 0);
	size_t siz = points.size();
	for (int i = 0; i < siz; i++)
		ret += points[i];
	ret /= (int)siz;
	return ret;
}

double GetAngle(Point prev, Point curr, Point next) {
	Point dir1 = prev - curr;
	double a_length = sqrt(dir1.x*dir1.x + dir1.y*dir1.y);
	Point dir2 = next - curr;
	double b_length = sqrt(dir2.x*dir2.x + dir2.y*dir2.y);
	Point dir3 = next - prev;
	double c_length = sqrt(dir3.x*dir3.x + dir3.y*dir3.y);
	double angle1, angle2;
	/*
	if (a_length < b_length && c_length < b_length) {
		angle1 = acos((dir1.x*dir2.x + dir1.y*dir2.y) / (a_length*b_length));
		angle2 = acos((dir3.x*dir2.x + dir3.y*dir2.y) / (a_length*b_length));
	}
	else if (a_length < c_length && b_length < c_length) {
		angle1 = acos((dir3.x*dir2.x + dir3.y*dir2.y) / (a_length*b_length));
		angle2 = acos((dir3.x*dir1.x + dir3.y*dir1.y) / (a_length*b_length));
	}
	else {
		angle1 = acos((dir1.x*dir2.x + dir1.y*dir2.y) / (a_length*b_length));
		angle2 = acos((dir3.x*dir1.x + dir3.y*dir1.y) / (a_length*b_length));
	}*/
	//double cosAngle = (dir1.x*dir2.x + dir1.y*dir2.y) / (a_length*b_length);
	
	double angle  = acos((dir1.x*dir2.x + dir1.y*dir2.y) / (a_length*b_length));

	return (dir1.x*dir2.y - dir1.y*dir2.x) > 0 ? angle : 2 * M_PI - angle;
}

void WriteAngles(Mat image, vector<Point> polygon, Scalar color) {
	String text;
	int i = 0;
	text = to_string(floor(GetAngle(polygon.back(), polygon[i], polygon[i + 1])/M_PI * 180));
	putText(image, text, polygon[0], FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 255));

	for (i = 1; i < polygon.size()-1; i++) {
		text = to_string(floor(GetAngle(polygon[i - 1], polygon[i], polygon[i + 1])/M_PI * 180));
		putText(image, text, polygon[i], FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 255));
	}

	text = to_string(floor(GetAngle(polygon[i-1], polygon[i], polygon[0]) / M_PI * 180));
	putText(image, text, polygon[i], FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 255));
}

int main(int args, const char** argv) {
	Mat img = imread("untitled2.png", IMREAD_COLOR);
	if (img.empty()) {
		cout << "Error: Image cannot be loaded..." << endl;
		return -1;
	}
		namedWindow("MYWINDOW", CV_WINDOW_AUTOSIZE);

		Mat img_gray;
		cvtColor(img, img_gray, COLOR_BGR2GRAY);
		//threshold(img_gray, img_gray, 185, 200, THRESH_BINARY_INV);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(img_gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

		String text;
		Point centerKoord;
		vector<Point> poly;

		for (size_t i = 0; i < contours.size(); i++)
		{
			
			text.clear();
			poly.clear();

			approxPolyDP(contours[i], poly, 4, true);
			if (poly.size() < 3)
				continue;
			text += std::to_string(poly.size());
			centerKoord = CenterPoint(poly);
			putText(img, text, centerKoord, FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 255));

			
			WriteAngles(img, poly, (0,255,255));
			

			Scalar color = Scalar(0, 0, 255);
			drawContours(img, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
		}

		//Mat dst;
		//adaptiveThreshold(img_gray, dst, 200, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 2);

		imshow("MYWINDOW", img);

		waitKey(0);
		destroyWindow("MYWINDOW");
		return 0;
	
}