#include "test.h"

int GRAY_THRESH = 150;
int HOUGH_VOTE = 90;
int BLOCK_SIZE = 25;
int CONST_VALUE = 5;

int g_dst_hight;  //最终图像的高度
int g_dst_width; //最终图像的宽度

Point2f center(0, 0);
String path;

bool sort_corners(vector<Point2f>& corners)
{
	vector<Point2f> top, bot;
	Point2f tmp_pt;
	vector<Point2f> olddata = corners;

	if (corners.size() != 4)
	{
		return false;
	}

	for (size_t i = 0; i < corners.size(); i++)
	{
		for (size_t j = i + 1; j < corners.size(); j++)
		{
			if (corners[i].y < corners[j].y)
			{
				tmp_pt = corners[i];
				corners[i] = corners[j];
				corners[j] = tmp_pt;
			}
		}
	}
	top.push_back(corners[0]);
	top.push_back(corners[1]);
	bot.push_back(corners[2]);
	bot.push_back(corners[3]);
	if (top.size() == 2 && bot.size() == 2) {
		corners.clear();
		Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];
		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
		return true;
	}
	else
	{
		corners = olddata;
		return false;
	}
}

Point2f computeIntersect(Vec4i a, Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		Point2f pt;
		pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
		pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
		return pt;
	}
	else
		return Point2f(-1, -1);
}

bool IsBadLine(int a, int b)
{
	if (a * a + b * b < 100)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool x_sort(const Point2f & m1, const Point2f & m2)
{
	return m1.x < m2.x;
}

void sortCorners(vector<Point2f> & corners, Point2f center)
{
	//确定四个点的中心线
	vector<Point2f> top, bot;
	vector<Point2f> backup = corners;

	sort(corners.begin(), corners.end(), x_sort);  //注意先按x的大小给4个点排序

	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y&& top.size() < 2)    //这里的小于2是为了避免三个顶点都在top的情况
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	corners.clear();

	if (top.size() == 2 && bot.size() == 2)
	{
		cout << "log" << endl;
		Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];


		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
	}
	else
	{
		corners = backup;
	}
}

void CalcDstSize(const vector<Point2f>& corners)
{
	int h1 = sqrt((corners[0].x - corners[3].x) * (corners[0].x - corners[3].x) + (corners[0].y - corners[3].y) * (corners[0].y - corners[3].y));
	int h2 = sqrt((corners[1].x - corners[2].x) * (corners[1].x - corners[2].x) + (corners[1].y - corners[2].y) * (corners[1].y - corners[2].y));
	g_dst_hight = MAX(h1, h2);

	int w1 = sqrt((corners[0].x - corners[1].x) * (corners[0].x - corners[1].x) + (corners[0].y - corners[1].y) * (corners[0].y - corners[1].y));
	int w2 = sqrt((corners[2].x - corners[3].x) * (corners[2].x - corners[3].x) + (corners[2].y - corners[3].y) * (corners[2].y - corners[3].y));
	g_dst_width = MAX(w1, w2);
}

Mat ocr()
{
	Mat src = imread(path);
	Mat source = src.clone();

	Mat bkup = src.clone();

	Mat img = src.clone();
	cvtColor(img, img, CV_RGB2GRAY);   //二值化

	GaussianBlur(img, img, Size(5, 5), 0, 0);  //高斯滤波

	//获取自定义核
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核
	//膨胀操作
	dilate(img, img, element);  //实现过程中发现，适当的膨胀很重要
	Canny(img, img, 30, 120, 3);   //边缘提取

	vector<vector<Point> > contours;
	vector<vector<Point> > f_contours;
	vector<Point> approx2;
	//注意第5个参数为CV_RETR_EXTERNAL，只检索外框  
	findContours(img, f_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

	//求出面积最大的轮廓
	int max_area = 0;
	int index;
	for (int i = 0; i < f_contours.size(); i++)
	{
		double tmparea = fabs(contourArea(f_contours[i]));
		if (tmparea > max_area)
		{
			index = i;
			max_area = tmparea;
		}

	}
	contours.push_back(f_contours[index]);

	vector<Point> tmp = contours[0];

	for (int line_type = 1; line_type <= 3; line_type++)
	{
		cout << "line_type: " << line_type << endl;
		Mat black = img.clone();
		black.setTo(0);
		drawContours(black, contours, 0, Scalar(255), line_type);  //注意线的厚度，不要选择太细的

		vector<Vec4i> lines;
		vector<Point2f> corners;
		vector<Point2f> approx;

		int para = 10;
		int flag = 0;
		int round = 0;
		for (; para < 300; para++)
		{
			cout << "round: " << ++round << endl;
			lines.clear();
			corners.clear();
			approx.clear();
			center = Point2f(0, 0);

			HoughLinesP(black, lines, 1, CV_PI / 180, para, 30, 10);

			//过滤距离太近的直线
			set<int> ErasePt;
			for (int i = 0; i < lines.size(); i++)
			{
				for (int j = i + 1; j < lines.size(); j++)
				{
					if (IsBadLine(abs(lines[i][0] - lines[j][0]), abs(lines[i][1] - lines[j][1])) && (IsBadLine(abs(lines[i][2] - lines[j][2]), abs(lines[i][3] - lines[j][3]))))
					{
						ErasePt.insert(j);//将该坏线加入集合
					}
				}
			}

			int Num = lines.size();
			while (Num != 0)
			{
				set<int>::iterator j = ErasePt.find(Num);
				if (j != ErasePt.end())
				{
					lines.erase(lines.begin() + Num - 1);
				}
				Num--;
			}
			if (lines.size() != 4)
			{
				continue;
			}

			//计算直线的交点，保存在图像范围内的部分
			for (int i = 0; i < lines.size(); i++)
			{
				for (int j = i + 1; j < lines.size(); j++)
				{
					Point2f pt = computeIntersect(lines[i], lines[j]);
					if (pt.x >= 0 && pt.y >= 0 && pt.x <= src.cols && pt.y <= src.rows)             //保证交点在图像的范围之内
						corners.push_back(pt);
				}
			}
			if (corners.size() != 4)
			{
				continue;
			}
#if 1
			bool IsGoodPoints = true;

			//保证点与点的距离足够大以排除错误点
			for (int i = 0; i < corners.size(); i++)
			{
				for (int j = i + 1; j < corners.size(); j++)
				{
					int distance = sqrt((corners[i].x - corners[j].x) * (corners[i].x - corners[j].x) + (corners[i].y - corners[j].y) * (corners[i].y - corners[j].y));
					if (distance < 5)
					{
						IsGoodPoints = false;
					}
				}
			}

			if (!IsGoodPoints) continue;
#endif
			approxPolyDP(Mat(corners), approx, arcLength(Mat(corners), true) * 0.02, true);

			if (lines.size() == 4 && corners.size() == 4 && approx.size() == 4)
			{
				flag = 1;
				break;
			}
		}

		// Get mass center  
		for (int i = 0; i < corners.size(); i++)
			center += corners[i];
		center *= (1. / corners.size());

		if (flag)
		{
			cout << "we found it!" << endl;
			circle(bkup, corners[0], 3, CV_RGB(255, 0, 0), -1);
			circle(bkup, corners[1], 3, CV_RGB(0, 255, 0), -1);
			circle(bkup, corners[2], 3, CV_RGB(0, 0, 255), -1);
			circle(bkup, corners[3], 3, CV_RGB(255, 255, 255), -1);
			circle(bkup, center, 3, CV_RGB(255, 0, 255), -1);

			sortCorners(corners, center);

			CalcDstSize(corners);

			Mat quad = Mat::zeros(g_dst_hight, g_dst_width, CV_8UC3);
			vector<Point2f> quad_pts;
			quad_pts.push_back(Point2f(0, 0));
			quad_pts.push_back(Point2f(quad.cols, 0));
			quad_pts.push_back(Point2f(quad.cols, quad.rows));

			quad_pts.push_back(Point2f(0, quad.rows));

			Mat transmtx = getPerspectiveTransform(corners, quad_pts);
			warpPerspective(source, quad, transmtx, quad.size());

			Mat local, gray;
			cvtColor(quad, gray, CV_RGB2GRAY);
			int blockSize = 25;
			int constValue = 10;
			adaptiveThreshold(gray, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);

			return local;
		}
	}
	return src;
}

Mat rotate(Mat srcImage)
{
	//转换为灰度图
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_RGB2GRAY);

	//获取图片原尺寸
	const int nRows = grayImage.rows;
	const int nCols = grayImage.cols;

	//图片尺寸转换，获取傅里叶变换尺寸
	//返回DFT最优尺寸大小的函数
	int mRows = getOptimalDFTSize(nRows);
	int mCols = getOptimalDFTSize(nCols);

	Mat newImage;
	//边界扩充函数
	copyMakeBorder(grayImage, newImage, 0, mRows - nRows, 0, mCols - nCols, BORDER_CONSTANT, Scalar::all(0));

	//图像DFT变换
	//通道组建立，使用Mat_容器，一个存实部，一个存虚部
	Mat groupImage[] = { Mat_<float>(newImage), Mat::zeros(newImage.size(), CV_32F) };
	Mat mergeImage;

	//合并通道
	merge(groupImage, 2, mergeImage);

	//离散傅里叶变换即DFT
	dft(mergeImage, mergeImage);

	//分离通道 
	split(mergeImage, groupImage);

	//调整数据
	//计算傅里叶变化各频率的幅值
	magnitude(groupImage[0], groupImage[1], groupImage[0]);
	Mat magImage = groupImage[0];

	//归一化操作，幅值加1
	magImage += Scalar::all(1);

	//取对数
	log(magImage, magImage);


	//重新分配象限，使(0,0)移动到图像中心，即把低频部分移动到中心  
	//傅里叶变换之前要对源图像乘以(-1)^(x+y)，进行中心化  
	int cx = magImage.cols / 2;
	int cy = magImage.rows / 2;
	Mat temp;
	//左上象限
	Mat LT(magImage, Rect(0, 0, cx, cy));
	//右上象限
	Mat RT(magImage, Rect(cx, 0, cx, cy));
	//左下象限
	Mat LB(magImage, Rect(0, cy, cx, cy));
	//右下象限
	Mat RB(magImage, Rect(cx, cy, cx, cy));

	//交换象限，左上换右下
	LT.copyTo(temp);
	RB.copyTo(LT);
	temp.copyTo(RB);

	//交换象限，右上换左下 
	RT.copyTo(temp);
	LB.copyTo(RT);
	temp.copyTo(LB);

	//归一化
	//在0-1之间是统计概率分布，为了后续操作方便
	normalize(magImage, magImage, 0, 1, CV_MINMAX);
	//像素强度变换，输出单通道灰度图
	Mat magImg;
	magImage.convertTo(magImg, CV_8UC1, 255, 0);
	//imshow("magnitude", magImg);

	//检测直线
	//二值化
	threshold(magImg, magImg, GRAY_THRESH, 255, CV_THRESH_BINARY);

	//构造8UC1格式图像
	vector<Vec2f> lines;
	Mat houghImg(magImg.size(), CV_8UC3);

	//Houge直线检测
	HoughLines(magImg, lines, 1, CV_PI / 180, HOUGH_VOTE, 0, 0);
	// cout << "检测直线条数:  " << lines.size() << endl;

	//绘制检测线
	for (int l = 0; l < lines.size(); l++)
	{
		float rho = lines[l][0], theta = lines[l][1];
		Point pt1, pt2;
		//坐标变换生成线表达式
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(houghImg, pt1, pt2, Scalar(255, 0, 0), 3, 8, 0);
	}
	// imshow("hough", houghImg);

	//获取角度
	float angel = 0;
	float m = PI / 90;
	float n = PI / 2;
	for (int l = 0; l < lines.size(); l++)
	{
		//遍历检测直线的角度
		float theta = lines[l][1];
		if (abs(theta) > m && abs(n - theta) > m)
		{
			//取有效角度
			angel = theta;
			break;
		}
	}

	//确保角度在0到90度内
	angel = angel < PI / 2 ? angel : angel - PI;

	//角度换算
	if (angel != PI / 2)
	{
		//作图一目了然
		float angelT = srcImage.rows * tan(angel) / srcImage.cols;
		angel = atan(angelT);
	}
	float angel_rad = angel * 180 / PI;
	// cout << "旋转角度: " << angel_rad << endl;

	//取图像中心
	Point2f centerPoint = Point2f(nCols / 2, nRows / 2);
	double scale = 1;

	//计算旋转中心
	Mat rotateMat = getRotationMatrix2D(centerPoint, angel_rad, scale);

	//仿射变换
	Mat resultImage(grayImage.size(), srcImage.type());
	warpAffine(srcImage, resultImage, rotateMat, srcImage.size(), 1, 0, Scalar(255, 255, 255));
	return resultImage;
}

Mat scan(Mat srcImage)
{
	Mat local, gray;
	local = rotate(srcImage);
	cvtColor(local, gray, CV_RGB2GRAY);
	adaptiveThreshold(gray, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, BLOCK_SIZE, CONST_VALUE);
	return local;
}

test::test(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

test::~test()
{
	//析构函数
}

void test::on_OpenFig_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("Open"),
		"",
		tr("Images (*.png *.bmp *.jpg )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{
		path = filename.toStdString(); 
		image = imread(path);
		cvtColor(image, image, CV_BGR2RGB);
		cv::resize(image, image, Size(400, 300));
		QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);

		label = new QLabel();
		label->setPixmap(QPixmap::fromImage(img));
		label->resize(QSize(img.width(), img.height()));
		ui.scrollArea->setWidget(label);

	}
}

void test::on_Progress_clicked()
{
	if (label) {
		Mat img2 = rotate(image);
		result = img2;
		QImage img1 = QImage((const unsigned char*)(img2.data), img2.cols, img2.rows, QImage::Format_RGB888);
		label_2 = new QLabel();
		label_2->setPixmap(QPixmap::fromImage(img1));
		label_2->resize(QSize(img1.width(), img1.height()));
		ui.scrollArea_2->setWidget(label_2);
	}
	else
	QMessageBox::warning(NULL, "Warning", "Rotate failed!");
}

void test::on_Save_clicked()
{
	//GRAY_THRESH越大，过滤散点越强
	//HOUGH_VOTE越大，检测到的直线越多
	GRAY_THRESH = ui.spinBox->value();
	HOUGH_VOTE = ui.spinBox_2->value();
	CONST_VALUE = ui.spinBox_3->value();
	QMessageBox::information(NULL, "Success", "Data has been saved!");
}

void test::on_SaveAs_clicked()
{
	if (label_2) {
		QString filename;
		filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.bmp *.jpg)"));
		string fileAsSave = filename.toStdString();
		if (filename.isEmpty())
		{
			return;
		}
		imwrite(fileAsSave, result);
		QMessageBox::information(NULL, "Success", "Save success!");
	}
	else
		QMessageBox::warning(NULL, "Warning", "Save failed!");
}

void test::on_Clear_clicked()
{
	if(label) label->clear();
	if(label_2) label_2->clear();
}

void test::on_OCR_clicked()
{
	ocr();
	if (label) {
		Mat img2 = ocr();
		result = img2;
		cv::resize(img2, img2, Size(800, 600));
		QImage img1 = QImage((const unsigned char*)(img2.data), img2.cols, img2.rows, QImage::Format_Grayscale8);
		label_2 = new QLabel();
		label_2->setPixmap(QPixmap::fromImage(img1));
		label_2->resize(QSize(img1.width(), img1.height()));
		ui.scrollArea_2->setWidget(label_2);
	}
	else
		QMessageBox::warning(NULL, "Warning", "OCR failed!");
	
}

void test::on_Sharpen_clicked()
{
	if (label) {
		Mat img2 = scan(image);
		result = img2;
		cv::resize(img2, img2, Size(800, 600));
		QImage img1 = QImage((const unsigned char*)(img2.data), img2.cols, img2.rows, QImage::Format_Grayscale8);
		label_2 = new QLabel();
		label_2->setPixmap(QPixmap::fromImage(img1));
		label_2->resize(QSize(img1.width(), img1.height()));
		ui.scrollArea_2->setWidget(label_2);
	}
	else
		QMessageBox::warning(NULL, "Warning", "Shrapen failed!");

}

