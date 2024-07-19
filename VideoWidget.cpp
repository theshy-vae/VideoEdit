#include "VideoWidget.h"
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<QPainter.h>
#include<iostream>
using namespace cv;
using namespace std;

VideoWidget::VideoWidget(QWidget* p):QOpenGLWidget(p)
{
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.drawImage(0,0, img);
}

//void VideoWidget::receiveMat(cv::Mat mat) {
//	if (mat.empty()) return;
//	QImage::Format fmt = QImage::Format_RGB888;
//	int pixSize = 3;
//	//图像是灰度图
//	if (mat.type() == CV_8UC1) {
//  		fmt = QImage::Format_Grayscale8;
//		pixSize = 1;
//	}
//	if (img.isNull()||img.format()!=fmt)
//	{
//		delete img.bits();
//		uchar* buf = new uchar[width() * height() * pixSize];
//		img = QImage(buf, width(), height(), fmt);
//	}
//	Mat des;
//	//设置图像大小
//	cv::resize(mat, des, Size(width(), height()));
//
//
//	//设置图像颜色格式
//	if (pixSize == 3) {
//		cv::cvtColor(des, des, COLOR_BGR2RGB);
//	}
//
//
//
//	//复制内存空间
//	memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
//	update();
//}

void VideoWidget::receiveMat(cv::Mat mat) {
    if (mat.empty()) return;

    QImage::Format fmt = QImage::Format_RGB888;
    int pixSize = 3;

    // 图像是灰度图
    if (mat.type() == CV_8UC1) {
        fmt = QImage::Format_Grayscale8;
        pixSize = 1;
    }

    if (img.isNull() || img.format() != fmt) {
        img = QImage(width(), height(), fmt);
    }

    Mat des;
    // 设置图像大小
    cv::resize(mat, des, Size(width(), height()));

    // 设置图像颜色格式
    if (pixSize == 3) {
        cv::cvtColor(des, des, COLOR_BGR2RGB);
    }

    // 复制内存空间
    memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
    update();
}
