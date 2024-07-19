#pragma once
#include<iostream>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
class Video
{
public:
	Video() {

	}
	Video(std::string path) {
		this->path = path;

	}

    cv::VideoCapture cap;
    std::string filename;

    std::string getPath() const { return path; }
    void setPath(const std::string& path) { this->path = path; }

    int getFps() const { return fps; }
    void setFps(int fps) { this->fps = fps; }

    int getWidth() const { return width; }
    void setWidth(int width) { this->width = width; }

    int getHeight() const { return height; }
    void setHeight(int height) { this->height = height; }

    int getCount() const { return count; }
    void setCount(int count) { this->count = count; }

    int getTotalMs() const { return totalMs; }
    void setTotalMs(int totalMs) { this->totalMs = totalMs; }

    int getClipX() const { return clipX; }
    void setClipX(int clipX) { this->clipX = clipX; }

    int getClipY() const { return clipY; }
    void setClipY(int clipY) { this->clipY = clipY; }

    int getClipW() const { return clipW; }
    void setClipW(int clipW) { this->clipW = clipW; }

    int getClipH() const { return clipH; }
    void setClipH(int clipH) { this->clipH = clipH; }

    int getPyDownCount() const { return pyDownCount; }
    void setPyDownCount(int pyDownCount) { this->pyDownCount = pyDownCount; }

    int getPyUpCount() const { return pyUpCount; }
    void setPyUpCount(int pyUpCount) { this->pyUpCount = pyUpCount; }

    int getNewWidth() const { return newWidth; }
    void setNewWidth(int newWidth) { this->newWidth = newWidth; }

    int getNewHeight() const { return newHeight; }
    void setNewHeight(int setHeight) { this->newHeight = newHeight; }

    int getBright() const { return bright; }
    void setBright(int bright) { this->bright = bright; }

    int getContrast() const { return contrast; }
    void setContrast(int contrast) { this->contrast = contrast; }

    int getRotateIndex() const { return rotateIndex; }
    void setRotateIndex(int rotateIndex) { this->rotateIndex = rotateIndex; }

    int getFlipIndex() const { return flipIndex; }
    void setFlipIndex(int flipIndex) { this->flipIndex = flipIndex; }

    int getGraySelect() const { return graySelect; }
    void setGraySelect(int graySelect) { this->graySelect = graySelect; }

    bool getHasMark() const { return hasMark; }
    void setHasMark(bool hasMark) { this->hasMark = hasMark; }

    cv::Mat getMark() const { return mark; }
    void setMark(const cv::Mat& mark) { this->mark = mark; }



private:
	std::string path;
	
	int fps;
	int width;
	int height;
    //帧数
    int count;
    int totalMs;
	//裁剪坐标 裁剪宽高
	int clipX=0;
	int clipY=0;
	int clipW=0;
	int clipH=0;
	int pyDownCount=0;
	int pyUpCount=0;
	//输出宽高33
	int newWidth;
	int newHeight;
	//亮度和对比度
	int bright=1;
	int contrast=0;
	//旋转
	int rotateIndex=0;
	//镜像
	int flipIndex=0;
	//灰度图、RGB
	int graySelect=0;
	//设置水印
	bool hasMark=false;
	cv::Mat mark;
};