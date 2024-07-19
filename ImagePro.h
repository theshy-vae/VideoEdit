#pragma once
#include<opencv2/core.hpp>
class ImagePro
{
public:
	//����ԭͼ������������
	void set(cv::Mat mat1, cv::Mat mat2);

	//��ȡ�����Ľ��
	cv::Mat get() { return des; }
	//�������ȺͶԱȶ�
	/// ���� 0-100  �Աȶ� 1-3
	void gain(double bright, double contrast);
	//��ת���
	void rotate90();
	void rotate180();
	void rotate270();
	//�������
	void flipX();
	void flipY();
	void flipXY();
	//�ı䵼����Ƶ�ߴ�
	void resizeOutputVideo(double width, double heigh);
	//��˹������
	void pyDown(double count);
	//������˹������
	void pyUp(double count);
	//ͼ��ü����ü����ͼ�����ŵ�ԭ��ͼ��Ĵ�С��
	void clip(int x, int y, int w, int h);
	//ͼ���Ϊ�Ҷ�ͼ
	void gray();
	//���ˮӡ  ˮӡλ�ã�ˮӡ͸����
	void mark(int x, int y, double transparency);
	//�ں���Ƶ ͸����
	void blend(double a);
	//ƴ����Ƶ
	void concat();

	ImagePro();
	~ImagePro();

private:
	//src1��ԭͼ src2��ˮӡ���ߵڶ���ͼ��
	cv::Mat src1, src2;
	cv::Mat des;
};

