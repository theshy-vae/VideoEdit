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
	//给des赋初值
	src1.copyTo(des);
}

//设置亮度和对比度
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

//改变导出视频尺寸
void ImagePro::resizeOutputVideo(double width, double heigh)
{
	if (des.empty()) return;
	resize(des,des, Size(width, heigh));
}

//高斯金字塔
void ImagePro::pyDown(double count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
		pyrDown(des, des);
}
//拉普拉斯金字塔
void ImagePro::pyUp(double count)
{
	if (des.empty()) return;
	for(int i=0;i<count;i++)
		pyrUp(des, des);
}

//图像裁剪（裁剪后后图像缩放到原来图像的大小）
void ImagePro::clip(int x, int y, int w, int h)
{
	if (des.empty()) return;
	des = des(Rect(x, y, w, h));
}

//图像变为灰度图
void ImagePro::gray()
{
	if (des.empty()) return;
	cvtColor(des, des, COLOR_BGR2GRAY);
}

//添加水印  水印位置，水印权重
void ImagePro::mark(int x, int y, double alpha)
{
	if (des.empty()) return;
	//没有水印图片
	if (src2.empty()) return;
	//原图上裁剪下与水印大小相同的图片 rectSrc并没有新分配内存，仍指向原图
	Mat rectSrc = des(Rect(x, y, src2.cols, src2.rows));
	if (x + src2.cols > VideoThread::Get()->width || y + src2.rows > VideoThread::Get()->height)
		return;
	addWeighted(src2, alpha, rectSrc, 1 - alpha, 0, rectSrc);
}

//融合视频 透明度
void ImagePro::blend(double a)
{
	if (des.empty()) return;
	//第二路视频
	if (src2.empty()) return;
	//将src2尺寸调整为src1
	resize(src2, src2, des.size());
	addWeighted(src2, a, des, 1 - a, 0, des);
	VideoThread::Get()->desW = 0;
	VideoThread::Get()->desH = 0;
}

//拼接视频
void ImagePro::concat()
{
	if (des.empty()) return;
	//第二路视频
	if (src2.empty()) return;
	//将src2高度变为src1一致
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
