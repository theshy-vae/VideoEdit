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
    //�������϶�������Ƶ����λ��
    void setPos(int pos);
    //���ù�����
    void setFilter();
    //����/ֹͣ����
    void exportVideo();
    //��������
    void exportVideoEnd();
    //������Ƶ
    void play();
    //��ͣ��Ƶ
    void pause();
    //����ˮӡͼƬ
    void setMark();
    //���õڶ�·��Ƶ
    void open2();
    //ƴ����Ƶ
    void concat();
    //�ü���������ƶ�
    void splitLeft(int pos);
    //�ü��ҽ������ƶ�
    void splitRight(int pos);
    //Ԥ�����������Ƶ
    void preview();
    //ѡ����Ƶ�б���ĳ����Ƶ
    void setCurrentVideoRow(int row);

private:
    Ui::VideoUIClass ui;
    //��ƵԴ����
    std::vector<std::string> srcList;
    
};
