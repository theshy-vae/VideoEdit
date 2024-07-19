#pragma once
#include<opencv2/core.hpp>
#include<vector>

enum TaskType {
	NONE,
	Gain,	//设置亮度和对比度
	ROTATE_90,	//旋转90度
	ROTATE_180,
	ROTATE_270,
	FLIPX,	//x轴镜像
	FLIPY,	//y轴镜像
	FLIPXY,
	RESIZE,	//改变图形尺寸
	PYUP,	//图形上采样
	PYDOWN,  //图形下采样
	CLIP,	//图像裁剪
	GRAY,	//灰度图
	MARK,	//水印
	BLEND,	//视频融合
	CONCAT,	//视频拼接
};

struct Task {
	TaskType type;
	//不确定参数个数，用vector存储
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

