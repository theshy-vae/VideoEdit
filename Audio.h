#pragma once
#include<string>
class Audio
{
public:
	static Audio* Get(); 
	

	/// <summary>
	/// 导出音频文件
	/// </summary>
	/// <param name="src">源文件（多媒体文件）</param>
	/// <param name="out">输出的音频文件（MP3）</param>
	//@param beginMs 开始的毫秒（剪辑视频）
	//@param endMs 结束的毫秒
	/// <returns></returns>
	virtual bool exportA(std::string src, std::string out, int beginMs=0, int endMs=0) = 0;

	//合并视频文件+音频文件
	//@param video 源视频文件
	//@param audio 源音频文件
	//@param out 输出合并的视频文件
	virtual bool merge(std::string video, std::string audio, std::string out)=0;


protected:
	Audio();
	virtual ~Audio();
};

