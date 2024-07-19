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

//是否点击了进度条 防止拖动进度条时定时器渲染图片和拖动进度条渲染图片冲突
static bool isSliderPress = false;
static bool isSliderMove = false;
//是否是rgb，默认是true
static bool isColor = true;
//是否选择了水印图片
static bool hasMark = false;
//是否选了融合视频
static bool isBlend = false;
//是否选了视频拼接
static bool isConcat = false;

VideoUI::VideoUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //隐藏标题栏
    //setWindowFlags(Qt::FramelessWindowHint); 
    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* file = menuBar->addMenu(QStringLiteral("文件"));
    QAction* openfile = new QAction(QStringLiteral("打开文件"));
    file->addAction(openfile);
    connect(openfile, &QAction::triggered, this,&VideoUI::open);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(VideoThread::Get(), SIGNAL(ViewImg1(cv::Mat)), ui.src1, SLOT(receiveMat(cv::Mat)));
    //connect(VideoThread::Get(), SIGNAL(ViewImg2(cv::Mat)), ui.src2, SLOT(receiveMat(cv::Mat)));

    connect(VideoThread::Get(), SIGNAL(ViewDesImg(cv::Mat)), ui.des, SLOT(receiveMat(cv::Mat)));
    //导出结束后改变导出按钮的字体
    connect(VideoThread::Get(), SIGNAL(ExportEnd()), this, SLOT(exportVideoEnd()));

    pause();
}

VideoUI::~VideoUI()
{}

void VideoUI::timerEvent(QTimerEvent * ev)
{
    if (isSliderPress) return;
    //获取当前视频进度百分比
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

//拖动进度条
//@param pos 进度条位置，界面中设置为 0-999
void VideoUI::setPos(int pos)
{   
    isSliderMove = true;
}

void VideoUI::open(){
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择视频"));
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
//    //是否进行了金字塔上下采样，如有就不能设置视频尺寸
//    bool isPy = false;
//    double bright = ui.bright->value();
//    double contrast = ui.contrast->value();
//    //旋转类型 index->value 1->90 2->180 3->270
//    int rotateIndex = ui.rotateSelect->currentIndex();
//    //镜像类型
//    int flipIndex = ui.flipSelect->currentIndex();
//    //设置的输出视频宽度
//    double setWidth = ui.setWidth->text().toDouble();
//    //设置的输出视频高度
//    double setHight = ui.setHight->text().toDouble();
//    //高斯金字塔次数
//    double pyDownCount = ui.pyDown->text().toDouble();
//    //拉普拉斯金字塔次数
//    double pyUpCount = ui.pyUp->text().toDouble();
//
//    //裁剪会先裁剪后再将图片缩放到原来尺寸，与金字塔和改变尺寸冲突
//    bool isClip = false;
//
//    int width = VideoThread::Get()->width;
//    int height = VideoThread::Get()->height;
//
//
//
//    //裁剪
//    if (ui.clipX->value() >= 0 && ui.clipY->value() >= 0 && ui.clipW->value() > 0 && ui.clipH->value() > 0) {
//        if ((ui.clipW->value() + ui.clipX->value() > width) || (ui.clipH->value() + ui.clipY->value() > height)) {
//            QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("超出裁剪范围"));
//            return;
//        }
//        Filter::Get()->add(Task{ CLIP,{ui.clipX->value(),ui.clipY->value(),ui.clipW->value(),ui.clipH->value()} });
//        //裁剪后恢复原来尺寸
//        Filter::Get()->add(Task{ RESIZE,{(double)width,(double)height} });
//    }
//
//    //图像金字塔
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
//    //输出视频尺寸
//    if (!isPy&&setWidth > 0 && setHight > 0) {
//        Filter::Get()->add(Task{ RESIZE,{setWidth,setHight} });
//    }
//
//    //亮度和对比度
//    if (bright >= 0 && bright <= 100 && contrast >= 0 && contrast <= 100) {
//        Filter::Get()->add(Task{ NONE,{bright,contrast} });
//    }
//    //旋转
//    if (rotateIndex == 1) {
//        Filter::Get()->add(Task{ ROTATE_90 });
//    }
//    else if (rotateIndex == 2) {
//        Filter::Get()->add(Task{ ROTATE_180 });
//    }
//    else if (rotateIndex == 3) {
//        Filter::Get()->add(Task{ ROTATE_270 });
//    }
//    //镜像翻转
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
//    //灰度图、RGB
//    if (ui.graySelect->currentIndex() == 1) {
//        Filter::Get()->add(Task{ GRAY });
//        isColor = false;
//    }
//    else {
//        isColor = true;
//    }
//    //设置水印
//    if (hasMark) {
//        int mx = ui.mx->value();
//        int my = ui.my->value();
//        double mApha = ui.mApha->value();
//        Filter::Get()->add(Task{ MARK,{(double)mx,(double)my,mApha} });
//    }
//
//
//    //视频融合
//    if (ui.src2Select->currentIndex()==0) {
//        double a = ui.src2Apha->value();
//        Filter::Get()->add(Task{ BLEND,{(double)a} });
//    }
//    //视频拼接
//    if (ui.src2Select->currentIndex() == 1) {
//        Filter::Get()->add(Task{ CONCAT});
//    }
//}

//对所有视频添加编辑任务
void VideoUI::setFilter() {
    saveEditInfo();
    Filter::Get()->clear();
    //是否进行了金字塔上下采样，如有就不能设置视频尺寸

    //裁剪会先裁剪后再将图片缩放到原来尺寸，与金字塔和改变尺寸冲突


    for (int i = 0; i < VideoThread::Get()->videoList.size(); i++) {
        Video* v = VideoThread::Get()->videoList[i];
        //裁剪
        if (v->getClipX() >= 0 && v->getClipY() >= 0 && v->getClipW() > 0 && v->getClipH() > 0) {
            if ((v->getClipW() + v->getClipX() > v->getWidth()) || (v->getClipH()+ ui.clipY->value() > v->getHeight())) {
                const QString filenameQString = "cxk";
                QMessageBox::warning(this, QStringLiteral("提示"), QString::fromStdString(v->filename)+"裁剪超过范围");
                return;
            }
            Filter::Get()->add(i,Task{ CLIP,{ui.clipX->value(),ui.clipY->value(),ui.clipW->value(),ui.clipH->value()} });
            //裁剪后恢复原来尺寸
            Filter::Get()->add(i,Task{ RESIZE,{(double)v->getWidth(),(double)v->getHeight()} });
        }
        //图像金字塔
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
        
        //亮度和对比度
        if (v->getBright() >= 0 && v->getBright() <= 100 && v->getContrast() >= 0 && v->getContrast() <= 100) {
            Filter::Get()->add(i,Task{ NONE,{(double)v->getBright(),(double)v->getContrast()} });
        }
        //旋转
        if (v->getRotateIndex() == 1) {
            Filter::Get()->add(i,Task{ ROTATE_90 });
        }
        else if (v->getRotateIndex() == 2) {
            Filter::Get()->add(i, Task{ ROTATE_180 });
        }
        else if (v->getRotateIndex() == 3) {
            Filter::Get()->add(i, Task{ ROTATE_270 });
        }
        //镜像翻转
        if (v->getFlipIndex() == 1) {
            Filter::Get()->add(i, Task{ FLIPY });
        }
        else if (v->getFlipIndex() == 2) {
            Filter::Get()->add(i, Task{ FLIPX });
        }
        else if (v->getFlipIndex() == 3) {
            Filter::Get()->add(i, Task{ FLIPXY });
        }

        //全局
        //输出视频尺寸
        if (!v->getNewWidth() > 0 && v->getNewHeight() > 0) {
            Filter::Get()->add(i,Task{ RESIZE,{(double)v->getNewWidth(),(double)v->getNewHeight()} });
        }
    }






    ////灰度图、RGB
    //if (ui.graySelect->currentIndex() == 1) {
    //    Filter::Get()->add(Task{ GRAY });
    //    isColor = false;
    //}
    //else {
    //    isColor = true;
    //}
    ////设置水印
    //if (hasMark) {
    //    int mx = ui.mx->value();
    //    int my = ui.my->value();
    //    double mApha = ui.mApha->value();
    //    Filter::Get()->add(Task{ MARK,{(double)mx,(double)my,mApha} });
    //}


    ////视频融合
    //if (ui.src2Select->currentIndex() == 0) {
    //    double a = ui.src2Apha->value();
    //    Filter::Get()->add(Task{ BLEND,{(double)a} });
    //}
    ////视频拼接
    //if (ui.src2Select->currentIndex() == 1) {
    //    Filter::Get()->add(Task{ CONCAT });
    //}
}

//判断多次点击同一按钮是导出还是停止导出
static bool isExport = false;

void VideoUI::exportVideo() {

    if (isExport) {
        //停止导出
        VideoThread::Get()->stopSave();
        isExport = false;
        ui.exportBtn->setText(QStringLiteral("导出"));
        return;
    }
    QString name =  QFileDialog::getSaveFileName(this, "导出", "video.avi");
    if (name.isEmpty()) {
        QMessageBox::warning(this,"警告",QStringLiteral("导出失败"));
        return;
    }
    std::string filename = name.toLocal8Bit().data();
    if (VideoThread::Get()->startSave(filename,ui.setWidth->text().toInt(), ui.setHight->text().toInt(),isColor)) {
        //导出
        isExport = true;
        ui.exportBtn->setText(QStringLiteral("停止导出"));
    }
    else {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("导出失败"));
    }
}

void VideoUI::exportVideoEnd()
{
    isExport = false;
    ui.exportBtn->setText(QStringLiteral("导出"));

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

//设置水印图片
void VideoUI::setMark()
{
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("选择水印图片"));
    if (file.isEmpty()) return;
    string filename = file.toLocal8Bit().data();
    cv::Mat mark = cv::imread(filename);
    if (mark.empty()) return;
    VideoThread::Get()->setMark(mark);
    hasMark = true;

}
//设置融合视频
void VideoUI::open2()
{
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("选择要融合的视频"));
    if (file.isEmpty()) return;
    string filename = file.toLocal8Bit().data();
    if (!VideoThread::Get()->Open2(filename)) {
        return;
    }
    hasMark = false;
}

//拼接视频
void VideoUI::concat()
{
    hasMark = false;
    isBlend = false;

}

//裁剪左进度条移动
void VideoUI::splitLeft(int pos)
{
    VideoThread::Get()->setSplitLeft((double)pos / 1000.);
}

//裁剪右进度条移动
void VideoUI::splitRight(int pos)
{
    VideoThread::Get()->setSplitRight((double)pos / 1000.);
}

void VideoUI::preview()
{
    VideoThread::Get()->setPreview();
}

//跳转视频前先保存当前视频的编辑信息
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

//选择视频列表中某个视频
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
