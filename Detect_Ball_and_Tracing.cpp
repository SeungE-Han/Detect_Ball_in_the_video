#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture capture;
	capture.open("pipoball.avi");
	CV_Assert(capture.isOpened()); //예외처리

	double frame_rate = capture.get(CAP_PROP_FPS);
	int delay = 1000 / frame_rate;

	int width = capture.get(CAP_PROP_FRAME_WIDTH);
	int height = capture.get(CAP_PROP_FRAME_HEIGHT);

	int nFrameCount = 0;
	Size sz(width, height);
	Mat frame;

	Point2i bcenter[30] = {};
	int i = 0;

	while (capture.read(frame)) {
		printf("nFrameCount=%d\n", nFrameCount);

		Point2i pt, sum(0, 0);
		int count = 0;
		Mat dst(sz, CV_8UC3, Scalar(255, 255, 255));

		for (pt.y = 0; pt.y < frame.rows; pt.y++) {
			for (pt.x = 0; pt.x < frame.cols ; pt.x++) {
				Vec3b val = frame.at<Vec3b>(pt.y, pt.x);
				float lum = 0.11*(float)val[0] + 0.59*(float)val[1] + 0.3*(float)val[2];

				if (val[2]>0.5*lum&&val[2]>1.5*val[0]&&val[2]>1.5+val[1]&&lum>50.) { //주황색
					count++; //주황색을 나타내는 픽셀 수 세기
					sum += pt; //다 더하기
				}
			}
		}
		
		Point2i center(sum.x / count, sum.y / count); //원의 중점
		
		int radius = sqrt((float)count / 3.14); //원 반지름 구하기
	
		rectangle(dst, Point(center.x - radius, center.y - radius), Point(center.x + radius, center.y + radius)
			, Scalar(0, 0, 0),2);
		
		circle(dst, center, radius, Scalar(0, 255, 255), -1); //원 그림
		circle(dst, center, radius, Scalar(0, 0, 255), 2);
		putText(dst, "A", center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0)); //글씨
		
		if (nFrameCount%30 == 0) {
			for (int k = 0; k < 30; k++) {
				bcenter[k].x = NULL;
				bcenter[k].y = NULL;
			}
			i = 0;
		}
		bcenter[i++] = center; //중심좌표 30개 저장
		for (int j = 1; j<i; j++) {
				line(dst, bcenter[j - 1], bcenter[j], Scalar(0, 255, 0)); //선 그림
		}

		imshow("frame", frame);
		imshow("dst", dst);
		
		char chKey = waitKey(5);
		if (chKey == 27) break;

		nFrameCount++;
	}
}