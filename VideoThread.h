#pragma once
#include<QThread>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<QMutex>
#include"Video.h"
#include<iostream>

class VideoThread:public QThread
{

	Q_OBJECT

public:
	int fps=0;
	//cap1 ���
	int width=0;
	int height=0;
	//des��Ƶ���
	int desW = 0;
	int desH = 0;
	//��Ƶʱ��
	int totalMs = 0;

	//�ü���Ƶ���ҵİٷֱ�
	double begin = 0;
	double end = 1;

	//src1�ļ�·��
	std::string srcFile1;
	//des�ļ�·��
	std::string desFile;
	//�Ƿ�Ԥ��
	int preview = 0;
	std::vector<Video*> videoList;
	Video* curVideo = nullptr;

	//����ģʽ
	static VideoThread* Get() {
		static VideoThread vt;
		return &vt;
	}
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file, const std::string filename);
	//�򿪶�����ƵԴ�ļ�
	bool Open2(const std::string file);
	//��ȡһ����Ƶλ��
	double getPts1();

	~VideoThread();

	//�߳���ں���
	void run();
	//������Ƶ֡λ�ã��ٷֱȣ���ת��Ƶ
	void seek(double pos);
	//��ʼ������Ƶ
	bool startSave(const std::string filename, int width = 0,int height = 0, bool isColor=true);
	//ֹͣ������Ƶ��д����Ƶ֡������
	void stopSave();
	void play() {
		mutex.lock();
		isPlay = true;
		mutex.unlock();
	}
	void pause() {
		mutex.lock();
		isPlay = false;
		mutex.unlock();
	}
	void setMark(cv::Mat m) { mutex.lock(); mark = m; mutex.unlock(); }
	
	//���òü�λ��
	void setSplitLeft(double a);
	void setSplitRight(double a);
	//�Ƿ�Ԥ������������Ƶ
	void setPreview();
	//���õ�ǰ���ŵ�video
	Video* setCurrentVideo(int row);



signals:
	void ViewImg1(cv::Mat mat);
	void ViewDesImg(cv::Mat mat);
	//��Ƶ�����������ź�
	void ExportEnd();
	//���Ŷ�����Ƶ
	void ViewImg2(cv::Mat mat2);

protected:
	//��ֹ��������ģʽ
	VideoThread();
	QMutex mutex;
	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	//�Ƿ񲥷���Ƶ
	bool isPlay = false;
	//ˮӡͼƬ
	cv::Mat mark;

};

