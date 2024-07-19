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
	//cap1 宽高
	int width=0;
	int height=0;
	//des视频宽高
	int desW = 0;
	int desH = 0;
	//视频时长
	int totalMs = 0;

	//裁剪视频左右的百分比
	double begin = 0;
	double end = 1;

	//src1文件路径
	std::string srcFile1;
	//des文件路径
	std::string desFile;
	//是否预览
	int preview = 0;
	std::vector<Video*> videoList;
	Video* curVideo = nullptr;

	//单件模式
	static VideoThread* Get() {
		static VideoThread vt;
		return &vt;
	}
	//打开一号视频源文件
	bool Open(const std::string file, const std::string filename);
	//打开二号视频源文件
	bool Open2(const std::string file);
	//获取一号视频位置
	double getPts1();

	~VideoThread();

	//线程入口函数
	void run();
	//根据视频帧位置（百分比）跳转视频
	void seek(double pos);
	//开始保存视频
	bool startSave(const std::string filename, int width = 0,int height = 0, bool isColor=true);
	//停止保存视频，写入视频帧的索引
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
	
	//设置裁剪位置
	void setSplitLeft(double a);
	void setSplitRight(double a);
	//是否预览剪辑完后的视频
	void setPreview();
	//设置当前播放的video
	Video* setCurrentVideo(int row);



signals:
	void ViewImg1(cv::Mat mat);
	void ViewDesImg(cv::Mat mat);
	//视频导出结束的信号
	void ExportEnd();
	//播放二号视频
	void ViewImg2(cv::Mat mat2);

protected:
	//防止跳出单件模式
	VideoThread();
	QMutex mutex;
	//是否开始写视频
	bool isWrite = false;
	//是否播放视频
	bool isPlay = false;
	//水印图片
	cv::Mat mark;

};

