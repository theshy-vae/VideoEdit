#pragma once
#include<opencv2/core.hpp>
class ImagePro
{
public:
	//设置原图，会清理处理结果
	void set(cv::Mat mat1, cv::Mat mat2);

	//获取处理后的结果
	cv::Mat get() { return des; }
	//设置亮度和对比度
	/// 亮度 0-100  对比度 1-3
	void gain(double bright, double contrast);
	//旋转相关
	void rotate90();
	void rotate180();
	void rotate270();
	//镜像相关
	void flipX();
	void flipY();
	void flipXY();
	//改变导出视频尺寸
	void resizeOutputVideo(double width, double heigh);
	//高斯金字塔
	void pyDown(double count);
	//拉普拉斯金字塔
	void pyUp(double count);
	//图像裁剪（裁剪后后图像缩放到原来图像的大小）
	void clip(int x, int y, int w, int h);
	//图像变为灰度图
	void gray();
	//添加水印  水印位置，水印透明度
	void mark(int x, int y, double transparency);
	//融合视频 透明度
	void blend(double a);
	//拼接视频
	void concat();

	ImagePro();
	~ImagePro();

private:
	//src1：原图 src2：水印或者第二层图形
	cv::Mat src1, src2;
	cv::Mat des;
};

