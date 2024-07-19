#include "Audio.h"
#include<string>
#include<iostream>
#include<QMutex>

using namespace std;

struct YTime
{
	YTime(int tms) {
		h = (tms / 1000) / 3600;
		m = ((tms / 1000) % 3600) / 60;
		s = (tms / 1000) % (3600 * 60);
		ms = tms % 1000;
	}
	std::string toString() {
		char buf[16] = { 0 };
		sprintf(buf, "%d:%d:%d.%d", h, m, s, ms);
		return buf;
	}
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
};

class YAudio :public Audio
{
public:
	QMutex mutex;


	bool exportA(std::string src, std::string out, int beginMs = 0, int endMs = 0) {
		//ffmpeg -i test.mp4 -ss 0:1:10.112 -t 0:1:10.100 -vn -y test.mp3  -ss ��ʼʱ��λ�ã�0ʱ1����10.100�룬 -t: ʱ����0ʱ1��10.100��  -vn video no����Ҫ��Ƶ�� -y ������ʾ��ȷ��
		string cmd = "ffmpeg ";
		cmd += " -i ";
		cmd += src;
		if (beginMs > 0) {
			cmd += " -ss ";
			YTime yt(beginMs);
			cmd += yt.toString();
		}
		if (endMs > 0) {
			cmd += " -t ";
			YTime yt(endMs);
			cmd += yt.toString();
		}
		cmd += " -vn -y ";
		cmd += out;
		cout << cmd << endl;
		//��ֹ����߳�ͬʱ����system
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}

	//�ϲ���Ƶ�ļ�+��Ƶ�ļ�
	bool merge(std::string video, std::string audio, std::string out)
	{
		//ffmpeg -i test.avi -i test.mp3 -c copy out.avi
		string cmd = "ffmpeg -i ";
		cmd += video;
		cmd += " -i ";
		cmd += audio;
		cmd += " -c copy ";
		cmd += out;
		//��ֹ����߳�ͬʱ����system
		cout << endl << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
};

Audio* Audio::Get()
{
	static YAudio a;
	return &a;
}

Audio::Audio()
{
}

Audio::~Audio()
{
}
