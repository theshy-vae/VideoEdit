#include "VideoUI.h"
#include"VideoThread.h"
#include<QFileDialog.h>
#include<QMessageBox.h>
#include<iostream>
#include<QDebug.h>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<QMenuBar>
#include<QListWidgetItem>
#include"Filter.h"
#include"Video.h"

using namespace std;

//�Ƿ����˽����� ��ֹ�϶�������ʱ��ʱ����ȾͼƬ���϶���������ȾͼƬ��ͻ
static bool isSliderPress = false;
static bool isSliderMove = false;
//�Ƿ���rgb��Ĭ����true
static bool isColor = true;
//�Ƿ�ѡ����ˮӡͼƬ
static bool hasMark = false;
//�Ƿ�ѡ���ں���Ƶ
static bool isBlend = false;
//�Ƿ�ѡ����Ƶƴ��
static bool isConcat = false;

VideoUI::VideoUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //���ر�����
    //setWindowFlags(Qt::FramelessWindowHint); 
    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* file = menuBar->addMenu(QStringLiteral("�ļ�"));
    QAction* openfile = new QAction(QStringLiteral("���ļ�"));
    file->addAction(openfile);
    connect(openfile, &QAction::triggered, this,&VideoUI::open);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(VideoThread::Get(), SIGNAL(ViewImg1(cv::Mat)), ui.src1, SLOT(receiveMat(cv::Mat)));
    //connect(VideoThread::Get(), SIGNAL(ViewImg2(cv::Mat)), ui.src2, SLOT(receiveMat(cv::Mat)));

    connect(VideoThread::Get(), SIGNAL(ViewDesImg(cv::Mat)), ui.des, SLOT(receiveMat(cv::Mat)));
    //����������ı䵼����ť������
    connect(VideoThread::Get(), SIGNAL(ExportEnd()), this, SLOT(exportVideoEnd()));

    pause();
}

VideoUI::~VideoUI()
{}

void VideoUI::timerEvent(QTimerEvent * ev)
{
    if (isSliderPress) return;
    //��ȡ��ǰ��Ƶ���Ȱٷֱ�
    double cur = VideoThread::Get()->getPts1();
    if (cur >= 0 && cur <= 1) {
        ui.playSlider->setValue(cur * 1000);
    }
}

void VideoUI::sliderPress()
{
    isSliderPress = true;
    isSliderMove = false;

    int pos = ui.playSlider->value();
    VideoThread::Get()->seek((double)pos / 1000.);

}

void VideoUI::sliderRelease()
{
    isSliderPress = false;
    if (isSliderMove) {
        int pos = ui.playSlider->value();
        VideoThread::Get()->seek((double)pos / 1000.);
    }
    isSliderMove = false;
}

//�϶�������
//@param pos ������λ�ã�����������Ϊ 0-999
void VideoUI::setPos(int pos)
{   
    isSliderMove = true;
}

void VideoUI::open(){
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ����Ƶ"));
    if (filePath.isEmpty()) {
        return;
    }
    string stdFilepath = filePath.toStdString();
    int index = stdFilepath.rfind("/");
    string filename = stdFilepath.substr(index + 1);
    if (!VideoThread::Get()->Open(filePath.toLocal8Bit().data(),filename)) {
        QMessageBox::information(this, "error", filePath + "open failed!");
        return;
    }
    srcList.push_back(filePath.toStdString());

    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(filename));
    ui.srcList->addItem(item);
    startTimer(80);
}

//void VideoUI::setFilter() {
//    Filter::Get()->clear();
//    //�Ƿ�����˽��������²��������оͲ���������Ƶ�ߴ�
//    bool isPy = false;
//    double bright = ui.bright->value();
//    double contrast = ui.contrast->value();
//    //��ת���� index->value 1->90 2->180 3->270
//    int rotateIndex = ui.rotateSelect->currentIndex();
//    //��������
//    int flipIndex = ui.flipSelect->currentIndex();
//    //���õ������Ƶ���
//    double setWidth = ui.setWidth->text().toDouble();
//    //���õ������Ƶ�߶�
//    double setHight = ui.setHight->text().toDouble();
//    //��˹����������
//    double pyDownCount = ui.pyDown->text().toDouble();
//    //������˹����������
//    double pyUpCount = ui.pyUp->text().toDouble();
//
//    //�ü����Ȳü����ٽ�ͼƬ���ŵ�ԭ���ߴ磬��������͸ı�ߴ��ͻ
//    bool isClip = false;
//
//    int width = VideoThread::Get()->width;
//    int height = VideoThread::Get()->height;
//
//
//
//    //�ü�
//    if (ui.clipX->value() >= 0 && ui.clipY->value() >= 0 && ui.clipW->value() > 0 && ui.clipH->value() > 0) {
//        if ((ui.clipW->value() + ui.clipX->value() > width) || (ui.clipH->value() + ui.clipY->value() > height)) {
//            QMessageBox::warning(this, QStringLiteral("��ʾ"), QStringLiteral("�����ü���Χ"));
//            return;
//        }
//        Filter::Get()->add(Task{ CLIP,{ui.clipX->value(),ui.clipY->value(),ui.clipW->value(),ui.clipH->value()} });
//        //�ü���ָ�ԭ���ߴ�
//        Filter::Get()->add(Task{ RESIZE,{(double)width,(double)height} });
//    }
//
//    //ͼ�������
//    if (pyDownCount > 0) {
//        isPy = true;
//        Filter::Get()->add(Task{ PYDOWN,{pyDownCount} });
//        width = width / pow(2, pyDownCount);
//        height = height / pow(2, pyDownCount);
//        ui.setWidth->setText(QString::number(width));
//        ui.setHight->setText(QString::number(height));
//    }
//    if (pyUpCount > 0) {
//        isPy = true;
//        Filter::Get()->add(Task{ PYUP,{pyUpCount} });
//        width = width * pow(2, pyUpCount);
//        height = height * pow(2, pyUpCount);
//        ui.setWidth->setText(QString::number(width));
//        ui.setHight->setText(QString::number(height));
//    }
//
//    //�����Ƶ�ߴ�
//    if (!isPy&&setWidth > 0 && setHight > 0) {
//        Filter::Get()->add(Task{ RESIZE,{setWidth,setHight} });
//    }
//
//    //���ȺͶԱȶ�
//    if (bright >= 0 && bright <= 100 && contrast >= 0 && contrast <= 100) {
//        Filter::Get()->add(Task{ NONE,{bright,contrast} });
//    }
//    //��ת
//    if (rotateIndex == 1) {
//        Filter::Get()->add(Task{ ROTATE_90 });
//    }
//    else if (rotateIndex == 2) {
//        Filter::Get()->add(Task{ ROTATE_180 });
//    }
//    else if (rotateIndex == 3) {
//        Filter::Get()->add(Task{ ROTATE_270 });
//    }
//    //����ת
//    if (flipIndex == 1) {
//        Filter::Get()->add(Task{ FLIPY });
//    }
//    else if (flipIndex == 2) {
//        Filter::Get()->add(Task{ FLIPX });
//    }
//    else if (flipIndex == 3) {
//        Filter::Get()->add(Task{ FLIPXY });
//    }
//
//    //�Ҷ�ͼ��RGB
//    if (ui.graySelect->currentIndex() == 1) {
//        Filter::Get()->add(Task{ GRAY });
//        isColor = false;
//    }
//    else {
//        isColor = true;
//    }
//    //����ˮӡ
//    if (hasMark) {
//        int mx = ui.mx->value();
//        int my = ui.my->value();
//        double mApha = ui.mApha->value();
//        Filter::Get()->add(Task{ MARK,{(double)mx,(double)my,mApha} });
//    }
//
//
//    //��Ƶ�ں�
//    if (ui.src2Select->currentIndex()==0) {
//        double a = ui.src2Apha->value();
//        Filter::Get()->add(Task{ BLEND,{(double)a} });
//    }
//    //��Ƶƴ��
//    if (ui.src2Select->currentIndex() == 1) {
//        Filter::Get()->add(Task{ CONCAT});
//    }
//}

//��������Ƶ��ӱ༭����
void VideoUI::setFilter() {
    saveEditInfo();
    Filter::Get()->clear();
    //�Ƿ�����˽��������²��������оͲ���������Ƶ�ߴ�

    //�ü����Ȳü����ٽ�ͼƬ���ŵ�ԭ���ߴ磬��������͸ı�ߴ��ͻ


    for (int i = 0; i < VideoThread::Get()->videoList.size(); i++) {
        Video* v = VideoThread::Get()->videoList[i];
        //�ü�
        if (v->getClipX() >= 0 && v->getClipY() >= 0 && v->getClipW() > 0 && v->getClipH() > 0) {
            if ((v->getClipW() + v->getClipX() > v->getWidth()) || (v->getClipH()+ ui.clipY->value() > v->getHeight())) {
                const QString filenameQString = "cxk";
                QMessageBox::warning(this, QStringLiteral("��ʾ"), QString::fromStdString(v->filename)+"�ü�������Χ");
                return;
            }
            Filter::Get()->add(i,Task{ CLIP,{ui.clipX->value(),ui.clipY->value(),ui.clipW->value(),ui.clipH->value()} });
            //�ü���ָ�ԭ���ߴ�
            Filter::Get()->add(i,Task{ RESIZE,{(double)v->getWidth(),(double)v->getHeight()} });
        }
        //ͼ�������
        if (v->getPyDownCount() > 0) {
            Filter::Get()->add(i,Task{ PYDOWN,{(double)v->getPyDownCount()} });
            //width = width / pow(2, pyDownCount);
            //height = height / pow(2, pyDownCount);
            //ui.setWidth->setText(QString::number(width));
            //ui.setHight->setText(QString::number(height));
        }
        if (v->getPyUpCount() > 0) {
            Filter::Get()->add(i,Task{ PYUP,{(double)v->getPyUpCount()} });
        }
        
        //���ȺͶԱȶ�
        if (v->getBright() >= 0 && v->getBright() <= 100 && v->getContrast() >= 0 && v->getContrast() <= 100) {
            Filter::Get()->add(i,Task{ NONE,{(double)v->getBright(),(double)v->getContrast()} });
        }
        //��ת
        if (v->getRotateIndex() == 1) {
            Filter::Get()->add(i,Task{ ROTATE_90 });
        }
        else if (v->getRotateIndex() == 2) {
            Filter::Get()->add(i, Task{ ROTATE_180 });
        }
        else if (v->getRotateIndex() == 3) {
            Filter::Get()->add(i, Task{ ROTATE_270 });
        }
        //����ת
        if (v->getFlipIndex() == 1) {
            Filter::Get()->add(i, Task{ FLIPY });
        }
        else if (v->getFlipIndex() == 2) {
            Filter::Get()->add(i, Task{ FLIPX });
        }
        else if (v->getFlipIndex() == 3) {
            Filter::Get()->add(i, Task{ FLIPXY });
        }

        //ȫ��
        //�����Ƶ�ߴ�
        if (!v->getNewWidth() > 0 && v->getNewHeight() > 0) {
            Filter::Get()->add(i,Task{ RESIZE,{(double)v->getNewWidth(),(double)v->getNewHeight()} });
        }
    }






    ////�Ҷ�ͼ��RGB
    //if (ui.graySelect->currentIndex() == 1) {
    //    Filter::Get()->add(Task{ GRAY });
    //    isColor = false;
    //}
    //else {
    //    isColor = true;
    //}
    ////����ˮӡ
    //if (hasMark) {
    //    int mx = ui.mx->value();
    //    int my = ui.my->value();
    //    double mApha = ui.mApha->value();
    //    Filter::Get()->add(Task{ MARK,{(double)mx,(double)my,mApha} });
    //}


    ////��Ƶ�ں�
    //if (ui.src2Select->currentIndex() == 0) {
    //    double a = ui.src2Apha->value();
    //    Filter::Get()->add(Task{ BLEND,{(double)a} });
    //}
    ////��Ƶƴ��
    //if (ui.src2Select->currentIndex() == 1) {
    //    Filter::Get()->add(Task{ CONCAT });
    //}
}

//�ж϶�ε��ͬһ��ť�ǵ�������ֹͣ����
static bool isExport = false;

void VideoUI::exportVideo() {

    if (isExport) {
        //ֹͣ����
        VideoThread::Get()->stopSave();
        isExport = false;
        ui.exportBtn->setText(QStringLiteral("����"));
        return;
    }
    QString name =  QFileDialog::getSaveFileName(this, "����", "video.avi");
    if (name.isEmpty()) {
        QMessageBox::warning(this,"����",QStringLiteral("����ʧ��"));
        return;
    }
    std::string filename = name.toLocal8Bit().data();
    if (VideoThread::Get()->startSave(filename,ui.setWidth->text().toInt(), ui.setHight->text().toInt(),isColor)) {
        //����
        isExport = true;
        ui.exportBtn->setText(QStringLiteral("ֹͣ����"));
    }
    else {
        QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("����ʧ��"));
    }
}

void VideoUI::exportVideoEnd()
{
    isExport = false;
    ui.exportBtn->setText(QStringLiteral("����"));

}

void VideoUI::play()
{
    ui.playBtn->hide();
    ui.pauseBtn->setGeometry(ui.playBtn->geometry());
    ui.pauseBtn->show();
    VideoThread::Get()->play();
}

void VideoUI::pause()
{
    ui.playBtn->show();
    ui.pauseBtn->hide();
    VideoThread::Get()->pause();

}

//����ˮӡͼƬ
void VideoUI::setMark()
{
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��ˮӡͼƬ"));
    if (file.isEmpty()) return;
    string filename = file.toLocal8Bit().data();
    cv::Mat mark = cv::imread(filename);
    if (mark.empty()) return;
    VideoThread::Get()->setMark(mark);
    hasMark = true;

}
//�����ں���Ƶ
void VideoUI::open2()
{
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��Ҫ�ںϵ���Ƶ"));
    if (file.isEmpty()) return;
    string filename = file.toLocal8Bit().data();
    if (!VideoThread::Get()->Open2(filename)) {
        return;
    }
    hasMark = false;
}

//ƴ����Ƶ
void VideoUI::concat()
{
    hasMark = false;
    isBlend = false;

}

//�ü���������ƶ�
void VideoUI::splitLeft(int pos)
{
    VideoThread::Get()->setSplitLeft((double)pos / 1000.);
}

//�ü��ҽ������ƶ�
void VideoUI::splitRight(int pos)
{
    VideoThread::Get()->setSplitRight((double)pos / 1000.);
}

void VideoUI::preview()
{
    VideoThread::Get()->setPreview();
}

//��ת��Ƶǰ�ȱ��浱ǰ��Ƶ�ı༭��Ϣ
void VideoUI::saveEditInfo() {
    if (VideoThread::Get()->curVideo) {
        VideoThread::Get()->curVideo->setBright(ui.bright->value());
        VideoThread::Get()->curVideo->setContrast(ui.contrast->value());
        VideoThread::Get()->curVideo->setRotateIndex(ui.rotateSelect->currentIndex());
        VideoThread::Get()->curVideo->setFlipIndex(ui.flipSelect->currentIndex());
        VideoThread::Get()->curVideo->setPyDownCount(ui.pyDown->text().toInt());
        VideoThread::Get()->curVideo->setPyUpCount(ui.pyUp->text().toInt());
        VideoThread::Get()->curVideo->setClipH(ui.clipH->value());
        VideoThread::Get()->curVideo->setClipW(ui.clipW->value());
        VideoThread::Get()->curVideo->setClipX(ui.clipX->value());
        VideoThread::Get()->curVideo->setClipY(ui.clipY->value());
    }
}

//ѡ����Ƶ�б���ĳ����Ƶ
void VideoUI::setCurrentVideoRow(int row)
{
    if (row >= 0) {
        saveEditInfo();
        Video* curVideo = VideoThread::Get()->setCurrentVideo(row);
        ui.bright->setValue(curVideo->getBright());
        ui.contrast->setValue(curVideo->getContrast());
        ui.rotateSelect->setCurrentIndex(curVideo->getRotateIndex());
        ui.flipSelect->setCurrentIndex(curVideo->getFlipIndex());
        ui.pyDown->setText(QString::number(curVideo->getPyDownCount()));
        ui.pyUp->setText(QString::number(curVideo->getPyUpCount()));
        ui.clipH->setValue((double)curVideo->getClipH());
        ui.clipW->setValue((double)curVideo->getClipW());
        ui.clipX->setValue((double)curVideo->getClipX());
        ui.clipY->setValue((double)curVideo->getClipY());
    } 

}
