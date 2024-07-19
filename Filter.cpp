#include "Filter.h"
#include<QThread>
#include"ImagePro.h"
#include<QMutex>
#include"Video.h"


class YFilter :public Filter
{
public:
	QMutex mutex;
	std::vector<CompleteTaskItem> completeTasks;

	virtual cv::Mat filter(int videoIndex,cv::Mat mat1, cv::Mat mat2) {
		mutex.lock();
		ImagePro imgPro;
		imgPro.set(mat1, mat2);
		if (videoIndex >= completeTasks.size()) {
			mutex.unlock();
			return mat1;

		}
		std::vector<Task> tasks = completeTasks[videoIndex].tasks;
		int size = tasks.size();
		for (int i = 0; i < size; i++) {
			switch (tasks[i].type)
			{
			case NONE:
				imgPro.gain(tasks[i].para[0], tasks[i].para[1]);
				break;
			case ROTATE_90:
				imgPro.rotate90();
				break;
			case ROTATE_180:
				imgPro.rotate180();
				break;
			case ROTATE_270:
				imgPro.rotate270();
				break;
			case FLIPX:
				imgPro.flipX();
				break;
			case FLIPY:
				imgPro.flipY();
				break;
			case FLIPXY:
				imgPro.flipXY();
				break;
			case RESIZE:
				imgPro.resizeOutputVideo(tasks[i].para[0], tasks[i].para[1]);
				break;
			case PYUP:
				imgPro.pyUp(tasks[i].para[0]);
				break;
			case PYDOWN:
				imgPro.pyDown(tasks[i].para[0]);
				break;
			case CLIP:
				imgPro.clip(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
				break;
			case GRAY:
				imgPro.gray();
				break;
			case MARK:
				imgPro.mark(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2]);
				break;
			case BLEND:
				imgPro.blend(tasks[i].para[0]);
				break;
			case CONCAT:
				imgPro.concat();
				break;
			default:
				break;
			}
		}
		mutex.unlock();
		return imgPro.get();
	}
	virtual void add(int videoIndex,Task task) {
		mutex.lock();
		if (videoIndex >= completeTasks.size()) {
			CompleteTaskItem item;
			completeTasks.push_back(item);
		}
		completeTasks[videoIndex].tasks.push_back(task);
		mutex.unlock();
	}
	virtual void clear() {
		mutex.lock();
		completeTasks.clear();
		mutex.unlock();
	}

};

Filter* Filter::Get()
{
	static YFilter filter;
	return &filter;
}

Filter::~Filter()
{
}

Filter::Filter()
{
}
