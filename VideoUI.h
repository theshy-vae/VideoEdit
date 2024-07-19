#pragma once

#include <QtWidgets/QWidget>
#include "ui_VideoUI.h"

class VideoUI : public QWidget
{
    Q_OBJECT

public:
    VideoUI(QWidget *parent = nullptr);
    ~VideoUI();

    void timerEvent(QTimerEvent* ev);
    void saveEditInfo();
    

public slots:
    void open();
    void sliderPress();
    void sliderRelease();
    //滑动条拖动设置视频播放位置
    void setPos(int pos);
    //设置过滤器
    void setFilter();
    //导出/停止导出
    void exportVideo();
    //导出结束
    void exportVideoEnd();
    //播放视频
    void play();
    //暂停视频
    void pause();
    //设置水印图片
    void setMark();
    //设置第二路视频
    void open2();
    //拼接视频
    void concat();
    //裁剪左进度条移动
    void splitLeft(int pos);
    //裁剪右进度条移动
    void splitRight(int pos);
    //预览剪辑后的视频
    void preview();
    //选择视频列表中某个视频
    void setCurrentVideoRow(int row);

private:
    Ui::VideoUIClass ui;
    //视频源数组
    std::vector<std::string> srcList;
    
};
