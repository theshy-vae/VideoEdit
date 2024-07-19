#include "VideoThread.h"

#include<QFile>
#include"Filter.h"
#include"Audio.h"

using namespace cv;
using namespace std;



//һ����ƵԴ
static VideoCapture cap1;
//������ƵԴ
static VideoCapture cap2;
//�����Ƿ��˳�����ֹ�����˳����̻߳���ִ��崻���
static bool isExit = false;

static VideoWriter vw;

VideoThread::VideoThread() {
	start();
}

VideoThread::~VideoThread() {
	mutex.lock();
	isExit = true;
	mutex.unlock();
}

void VideoThread::run()
{
	Mat mat1;
	for (;;) {
		mutex.lock();
		if (isExit) {
			mutex.unlock();
			break;
		}
		if (!curVideo) {
			mutex.unlock();
			msleep(5);

			continue;

		}

		//�ж���Ƶ�Ƿ�򿪡��Ƿ񲥷���Ƶ
		if (!curVideo->cap.isOpened()|| !isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		int curFrame = curVideo->cap.get(CAP_PROP_POS_FRAMES);
		int totalFrame = curVideo->cap.get(CAP_PROP_FRAME_COUNT);
		//��ȡһ֡��Ƶ�����벢��ɫת��
		if (curFrame> totalFrame*end||!curVideo->cap.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//�Զ�ֹͣ������������ɣ�
			if (vw.isOpened()) {
				stopSave();
				//��ui�����źţ��ı䰴ť����
				ExportEnd();
			}
			msleep(5);
			continue;
		}
		if (!preview) 
			ViewImg1(mat1);
		Mat mat2;
		//���˶�����ƵԴ�Ͳ������ˮӡ
		if (cap2.isOpened()) {
			if (cap2.read(mat2) && !mat2.empty()&&!preview) {
				ViewImg2(mat2);
			}
		}
		else {
			mat2 = mark;
		}
		auto it = std::find(videoList.begin(), videoList.end(), curVideo);
		cout << "index��" << it - videoList.begin();
		Mat des = Filter::Get()->filter(it-videoList.begin(), mat1, mat2);
		if(preview)
			//��ʾĿ��ͼ�� 
			ViewDesImg(des);
		int s = 0;
		s = 1000 / curVideo->getFps();

		//������Ƶ
		if (isWrite) {
			//vw.write(des);
			//����״̬������Ƶ���ţ��ӿ쵼���ٶ�
			cout<<"����һ֡"<<endl;
		}
		msleep(s);
		//�����������棺���������������˳��ˣ�isExit��Ϊtrue������ʱ����ViewImg1����
		mutex.unlock();
	}
}

bool VideoThread::Open(const std::string file,const std::string filename)
{
	//cout << "open" << file << endl;
	//mutex.lock();
	//bool re = cap1.open(file);
	//if (re) {
	//	fps = cap1.get(CAP_PROP_FPS);
	//	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	//	height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	//}
	//if (fps <= 0) fps = 30;
	//srcFile1 = file;
	//double count = cap1.get(CAP_PROP_FRAME_COUNT);
	//totalMs = (count / (double)fps) * 1000;
	//mutex.unlock();
	//cout <<"���"<< re << "fps:"<<fps<<"width:"<<width<<"height:"<<height<<endl;
	mutex.lock();
	Video* video = new Video(file);
	bool re = video->cap.open(file);
	if (re) {
		video->filename = filename;
		if (video->cap.get(CAP_PROP_FPS) > 0)
			video->setFps(video->cap.get(CAP_PROP_FPS));
		else
			video->setFps(30);
		video->setWidth(video->cap.get(CAP_PROP_FRAME_WIDTH));
		video->setHeight(video->cap.get(CAP_PROP_FRAME_HEIGHT));
		cout << video->cap.get(CAP_PROP_FPS);
		cout << video->cap.get(CAP_PROP_FRAME_WIDTH);
		cout << video->cap.get(CAP_PROP_FRAME_HEIGHT);

		double count = video->cap.get(CAP_PROP_FRAME_COUNT);
		video->setCount(count);
		video->setTotalMs((count / (double)fps) * 1000);
		videoList.push_back(video);
		if (videoList.size() == 1)
			curVideo = video;
	}
	mutex.unlock();
	//��ʼ�������Ƶ�ı༭��Ϣ
	return true;
}

//�򿪶�����ƵԴ�ļ�
bool VideoThread::Open2(const std::string file)
{
	cout << "open2" << file << endl;
	mutex.lock();
	bool re = cap2.open(file);
	curVideo->cap.set(CAP_PROP_POS_FRAMES, 0);
	mutex.unlock();
	return re;
}

double VideoThread::getPts1()
{
	double pos = 0;
	mutex.lock();
	if (!curVideo->cap.isOpened()) {
		mutex.unlock();
		return pos;
	}
	double total = curVideo->cap.get(CAP_PROP_FRAME_COUNT);
	double cur = curVideo->cap.get(CAP_PROP_POS_FRAMES);
	//��ֹtotal̫С�����Ժ����
	if (total > 0.001) {
		pos = cur / total;
	}
	mutex.unlock();
	return pos;
}

//������Ƶλ�ã��ٷֱȣ���ת��Ƶ
void VideoThread::seek(double pos) {
	mutex.lock();
	if (!curVideo->cap.isOpened()) {
		mutex.unlock();
		return;
	}
	double total = curVideo->cap.get(CAP_PROP_FRAME_COUNT);
	double curFrame = pos * total;
	if (curFrame >= 1 && curFrame < total) {
		curVideo->cap.set(CAP_PROP_POS_FRAMES, curFrame);
		cap2.set(CAP_PROP_POS_FRAMES, curFrame);
	}
	mutex.unlock();
}

bool VideoThread::startSave(const std::string filename, int width, int height,bool isColor)
{
	mutex.lock();
	if (!cap1.isOpened()) {
		mutex.unlock();
		return false;
	}
	if (width == 0) {
		if (desW > 0) {
			width = desW;
		}
		else {
			width = cap1.get(CAP_PROP_FRAME_WIDTH);
		}
	}
	if (height == 0) {
		if (desH > 0) {
			height = desH;
		}
		else {
			height = cap1.get(CAP_PROP_FRAME_HEIGHT);
		}
	}
	vw.open(filename, VideoWriter::fourcc('X', '2', '6', '4'), this->fps, Size(width, height),isColor);
	if (vw.isOpened()) {
		isWrite = true;
	}
	cap1.set(CAP_PROP_POS_FRAMES, begin*cap1.get(CAP_PROP_FRAME_COUNT));
	desFile = filename;
	cout << "��ʼ����" << endl;
	mutex.unlock();
	return true;
}

void VideoThread::stopSave()
{
	mutex.lock();
	cout << "ֹͣ����";
	vw.release();
	isWrite = false;
	mutex.unlock();
	//��Ƶ��ʼʱ�䣨���룩
	int ss = 0;
	//��Ƶʱ�����ü���ģ�
	int t = 0;
	ss = totalMs * begin;
	int splitEnd = totalMs * end;
	t = splitEnd - ss;
	//���ԭ��Ƶ��Ƶ���ϲ���des��
	Audio::Get()->exportA(srcFile1, srcFile1 + ".mp3",ss,t);
	string temp =  desFile+".avi";
	QFile::remove(temp.c_str());
	QFile::rename(desFile.c_str(), temp.c_str());
	Audio::Get()->merge(temp.c_str(), srcFile1 + ".mp3", desFile);
}

void VideoThread::setSplitLeft(double a) {
	if (a < 0 || a>1) return;
	mutex.lock();
	begin = a;
	mutex.unlock();
	seek(a);

}

void VideoThread::setSplitRight(double a) {
	if (a < 0 || a>1) return;
	mutex.lock();
	end =  a;
	mutex.unlock();
}

void VideoThread::setPreview() {
	mutex.lock();
	preview == 0 ? preview = 1 : preview = 0;
	mutex.unlock();
}

Video* VideoThread::setCurrentVideo(int row) {
	mutex.lock();
	curVideo = videoList[row];
	mutex.unlock();
	return curVideo;
}

