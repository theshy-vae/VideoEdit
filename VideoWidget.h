#pragma once
#include<QOpenGLWidget>
#include<opencv2/core.hpp>
#include<QImage>

class VideoWidget:public QOpenGLWidget
{

	Q_OBJECT
		
public:
	VideoWidget(QWidget* p);
	virtual ~VideoWidget();
	void paintEvent(QPaintEvent* e);
public slots:
	void receiveMat(cv::Mat mat);
protected:
	QImage img;
};

