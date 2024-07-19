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
		//ffmpeg -i test.mp4 -ss 0:1:10.112 -t 0:1:10.100 -vn -y test.mp3  -ss 开始时间位置，0时1分钟10.100秒， -t: 时长，0时1分10.100秒  -vn video no，不要视频， -y 弹出提示框都确认
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
		//防止多个线程同时调用system
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}

	//合并视频文件+音频文件
	bool merge(std::string video, std::string audio, std::string out)
	{
		//ffmpeg -i test.avi -i test.mp3 -c copy out.avi
		string cmd = "ffmpeg -i ";
		cmd += video;
		cmd += " -i ";
		cmd += audio;
		cmd += " -c copy ";
		cmd += out;
		//防止多个线程同时调用system
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
