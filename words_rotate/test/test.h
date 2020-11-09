#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test.h"
#include <QGraphicsScene>  
#include <QGraphicsView> 
#include <qfiledialog.h> 
#include <qlabel.h> 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

#include <QMessageBox>
#include <QWidget>
#include <QPainter>
#include <QBitmap>
#include <QImage>
#include <QSpinBox>
#include <iostream>

#include <algorithm>
#include <set>

using namespace cv;
using namespace std;

//#define GRAY_THRESH 150
//#define HOUGH_VOTE 70
#define PI 3.14159

class test : public QMainWindow
{
	Q_OBJECT

public:
	test(QWidget* parent = Q_NULLPTR);
	~test(); //析构函数

//添加槽函数
private slots:
	void on_OpenFig_clicked();
	void on_Progress_clicked();
	void on_Save_clicked();
	void on_SaveAs_clicked();
	void on_Clear_clicked();
	void on_OCR_clicked();
	void on_Sharpen_clicked();

private:
	Ui::testClass ui;
	Mat image;
	QLabel* label;
	QLabel* label_2;
	QSpinBox* spinbox_1;
	QSpinBox* spinbox_2;
	QSpinBox* spinbox_3;
	Mat result;
};
