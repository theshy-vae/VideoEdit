#pragma once
#include<opencv2/core.hpp>
#include<vector>

enum TaskType {
	NONE,
	Gain,	//�������ȺͶԱȶ�
	ROTATE_90,	//��ת90��
	ROTATE_180,
	ROTATE_270,
	FLIPX,	//x�᾵��
	FLIPY,	//y�᾵��
	FLIPXY,
	RESIZE,	//�ı�ͼ�γߴ�
	PYUP,	//ͼ���ϲ���
	PYDOWN,  //ͼ���²���
	CLIP,	//ͼ��ü�
	GRAY,	//�Ҷ�ͼ
	MARK,	//ˮӡ
	BLEND,	//��Ƶ�ں�
	CONCAT,	//��Ƶƴ��
};

struct Task {
	TaskType type;
	//��ȷ��������������vector�洢
	std::vector<double> para;
};

struct CompleteTaskItem {
	std::string videoPath;
	std::vector<Task> tasks;
};

class Filter
{
public:
	virtual cv::Mat filter(int videoIndex,cv::Mat mat1, cv::Mat mat2) = 0;
	virtual void add(int videoIndex,Task task) = 0;
	virtual void clear() = 0;
	static Filter* Get();
	virtual ~Filter();

protected:
	Filter();

};

