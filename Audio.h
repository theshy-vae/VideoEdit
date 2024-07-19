#pragma once
#include<string>
class Audio
{
public:
	static Audio* Get(); 
	

	/// <summary>
	/// ������Ƶ�ļ�
	/// </summary>
	/// <param name="src">Դ�ļ�����ý���ļ���</param>
	/// <param name="out">�������Ƶ�ļ���MP3��</param>
	//@param beginMs ��ʼ�ĺ��루������Ƶ��
	//@param endMs �����ĺ���
	/// <returns></returns>
	virtual bool exportA(std::string src, std::string out, int beginMs=0, int endMs=0) = 0;

	//�ϲ���Ƶ�ļ�+��Ƶ�ļ�
	//@param video Դ��Ƶ�ļ�
	//@param audio Դ��Ƶ�ļ�
	//@param out ����ϲ�����Ƶ�ļ�
	virtual bool merge(std::string video, std::string audio, std::string out)=0;


protected:
	Audio();
	virtual ~Audio();
};

