#include "ImagePro.h"
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include"VideoThread.h"

using namespace cv;

void ImagePro::set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty()) return;
	src1 = mat1;
	src2 = mat2;
	//��des����ֵ
	src1.copyTo(des);
}

//�������ȺͶԱȶ�
void ImagePro::gain(double bright, double contrast)
{
	if (des.empty()) return;
	des.convertTo(des, -1, bright, contrast);
}

void ImagePro::rotate90()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_CLOCKWISE);
}

void ImagePro::rotate180()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_180);

}

void ImagePro::rotate270()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_COUNTERCLOCKWISE);
}
void ImagePro::flipX()
{
	if (des.empty()) return;
	flip(des, des, 0);

}

void ImagePro::flipY()
{
	if (des.empty()) return;
	flip(des, des, 1);
}

void ImagePro::flipXY()
{
	if (des.empty()) return;
	flip(des, des, -1);
}

//�ı䵼����Ƶ�ߴ�
void ImagePro::resizeOutputVideo(double width, double heigh)
{
	if (des.empty()) return;
	resize(des,des, Size(width, heigh));
}

//��˹������
void ImagePro::pyDown(double count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
		pyrDown(des, des);
}
//������˹������
void ImagePro::pyUp(double count)
{
	if (des.empty()) return;
	for(int i=0;i<count;i++)
		pyrUp(des, des);
}

//ͼ��ü����ü����ͼ�����ŵ�ԭ��ͼ��Ĵ�С��
void ImagePro::clip(int x, int y, int w, int h)
{
	if (des.empty()) return;
	des = des(Rect(x, y, w, h));
}

//ͼ���Ϊ�Ҷ�ͼ
void ImagePro::gray()
{
	if (des.empty()) return;
	cvtColor(des, des, COLOR_BGR2GRAY);
}

//���ˮӡ  ˮӡλ�ã�ˮӡȨ��
void ImagePro::mark(int x, int y, double alpha)
{
	if (des.empty()) return;
	//û��ˮӡͼƬ
	if (src2.empty()) return;
	//ԭͼ�ϲü�����ˮӡ��С��ͬ��ͼƬ rectSrc��û���·����ڴ棬��ָ��ԭͼ
	Mat rectSrc = des(Rect(x, y, src2.cols, src2.rows));
	if (x + src2.cols > VideoThread::Get()->width || y + src2.rows > VideoThread::Get()->height)
		return;
	addWeighted(src2, alpha, rectSrc, 1 - alpha, 0, rectSrc);
}

//�ں���Ƶ ͸����
void ImagePro::blend(double a)
{
	if (des.empty()) return;
	//�ڶ�·��Ƶ
	if (src2.empty()) return;
	//��src2�ߴ����Ϊsrc1
	resize(src2, src2, des.size());
	addWeighted(src2, a, des, 1 - a, 0, des);
	VideoThread::Get()->desW = 0;
	VideoThread::Get()->desH = 0;
}

//ƴ����Ƶ
void ImagePro::concat()
{
	if (des.empty()) return;
	//�ڶ�·��Ƶ
	if (src2.empty()) return;
	//��src2�߶ȱ�Ϊsrc1һ��
	int dSrc2W = src2.cols * src2.cols / src1.cols;
	resize(src2, src2, Size(dSrc2W, des.rows));
	des = Mat(Size(src1.cols + dSrc2W, src1.rows), src1.type());
	Mat m1 = des(Rect(0, 0, src1.cols, src1.rows));
	Mat m2 = des(Rect(src1.cols, 0, dSrc2W, src1.rows));
	src1.copyTo(m1);
	src2.copyTo(m2);
	VideoThread::Get()->desW = des.cols;
	VideoThread::Get()->desH = des.rows;

}

ImagePro::ImagePro()
{
}

ImagePro::~ImagePro()
{
}
