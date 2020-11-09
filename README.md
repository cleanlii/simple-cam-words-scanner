## 功能介绍
- 图片打开和保存
- 图片矫正（证件扫描、文字纠正...）
- 图片锐化增强
- 图片清空
- 阈值设置
## 项目实现
#### 基本思路（证件扫描）
- 抠图：提取轮廓
- 矫正：透视变换
- 锐化增强：二值化
#### 算法设计（证件扫描）
###### 第一步：提取边缘
- 读取图像，转化为灰度图
- 降噪，二值化   高斯滤波 `GaussianBlur()`
- 适当膨胀，提高检测效率
- 边缘检测 `Canny()`，打印出二值图验证
###### 第二步：轮廓查找与筛选
- 轮廓检测 `findContours()`
- 霍夫直线检测 `HoughLines()`
- 绘制检测到的直线并验证 `line()`
- 排除距离过近、不相交的直线
- 排除距离过近的两直线交点
###### 第三步：透视变换
- 由第二步筛选出的四个顶点得出一组坐标
- 确定输出图像长宽（或自适应），验证
- 计算透视变换矩阵 `GetPerspectiveTransform()`
- 透视变换函数 `warpPerspective()`
###### 第四步：锐化增强
- 必要的二值化 `adaptiveThreshold()`
- ...
- 输出图像
## UI设计
![1](https://img-blog.csdnimg.cn/2020020521491456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NsZWFubGlp,size_16,color_FFFFFF,t_70)
## 核心代码
- 证件扫描
```cpp
Mat scanning()
{
	Mat src = imread(path);
	Mat source = src.clone();

	Mat bkup = src.clone();

	Mat img = src.clone();
	//二值化
	threshold(img, img, GRAY_THRESH, 255, CV_THRESH_BINARY);   

     //高斯滤波
	GaussianBlur(img, img, Size(5, 5), 0, 0); 

	//获取自定义核
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); 
	//适当膨胀
	dilate(img, img, element);
	//边缘提取
	Canny(img, img, 30, 120, 3);

	vector<vector<Point> > contours;
	vector<vector<Point> > f_contours;
	vector<Point> approx2;

	//轮廓检测
	findContours(img, f_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//求出面积最大的轮廓
	int max_area = 0;
	int index;
	for (int i = 0; i < f_contours.size(); i  )
	{
		double tmparea = fabs(contourArea(f_contours[i])); 
		if (tmparea > max_area)
		{
			index = i;
			max_area = tmparea;
		}
	}

	//找顶点
	Mat f_img = img.clone();
	vector<Vec4i> lines;
	vector<Point2f> corners;

    //验证轮廓
	drawContours(f_img, contours, 0, Scalar(255)); 
	lines.clear();
	corners.clear();

	//这里的阈值提供给用户修改
	//直线检测
	HoughLinesP(f_img, lines, 1, PI / 180, HOUGH_VOTE, 30, 10);

	//1.过滤不符条件的直线
	//2.计算直线交点
	//3.过滤不符条件的点

	DstSize(corners); //计算输出尺寸

	Mat dst = Mat::zeros(dst_hight, dst_width, CV_8UC3);
	vector<Point2f> f_points; //四边形顶点坐标组

	f_points.push_back(Point2f(0, 0));
	f_points.push_back(Point2f(dst.cols, 0));
	f_points.push_back(Point2f(dst.cols, dst.rows));
	f_points.push_back(Point2f(0, dst.rows));

	Mat temp = getPerspectiveTransform(corners, f_points); //计算透视变换矩阵 
	warpPerspective(source, dst, temp, dst.size()); //透视变换

    //这里也可以提供给用户修改
	//自动增强
	Mat local, gray;
	cvtColor(dst, gray, CV_RGB2GRAY);
	int blockSize = 25;
	int constValue = 10;
	//自适应二值化
	adaptiveThreshold(gray, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue); 

	return local;
}
```
- 文字纠正

```cpp
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
	magImage  = Scalar::all(1);

	//取对数
	log(magImage, magImage);


	//重新分配象限，使(0,0)移动到图像中心，即把低频部分移动到中心  
	//傅里叶变换之前要对源图像乘以(-1)^(x y)，进行中心化  
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
	for (int l = 0; l < lines.size(); l  )
	{
		float rho = lines[l][0], theta = lines[l][1];
		Point pt1, pt2;
		//坐标变换生成线表达式
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0   1000 * (-b));
		pt1.y = cvRound(y0   1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(houghImg, pt1, pt2, Scalar(255, 0, 0), 3, 8, 0);
	}
	// imshow("hough", houghImg);

	//获取角度
	float angel = 0;
	float m = PI / 90;
	float n = PI / 2;
	for (int l = 0; l < lines.size(); l  )
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
```
## 项目截图
- 证件扫描
![1](https://img-blog.csdnimg.cn/20200205220144815.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NsZWFubGlp,size_16,color_FFFFFF,t_70)
- 文字纠正
![2](https://img-blog.csdnimg.cn/20200205220228911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NsZWFubGlp,size_16,color_FFFFFF,t_70)
- 效果对比
![1](https://img-blog.csdnimg.cn/20200205220303528.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NsZWFubGlp,size_16,color_FFFFFF,t_70)
- 效果对比（娱乐向）
![2](https://img-blog.csdnimg.cn/20200205220424446.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NsZWFubGlp,size_16,color_FFFFFF,t_70)

## 项目总结
- 本项目基本实现了证件扫描和文字纠正两大基本功能，其中类似“全能扫描王”的扫描功能被我单独做了个版本，所以上述截图UI有些不一样，特此说明；
- 在这种基于透视变化的算法中，可以看见一定弊端：**直线检测的阈值、轮廓检测的标准、顶点筛选的严密性等，对最终的结果影响很大**，所以找到合理的、或者自适应的参数是最关键一步；故市面上的扫描软件一定有更复杂的思路或算法，还需要继续学习！
- 刚开始写的时候对OCR等词汇的理解不当，所以在函数命名和UI设计上出现了失误，特此指出；**OCR（Optical Character Recognition，光学字符识别）意为文字识别**，与本项目的功能不同；
- 希望本文能帮助到那些刚入门图像处理的同学，咱们一起加油！

- 关于证件扫描算法的疑惑可以参考 [这位大神的干货文章](https://www.cnblogs.com/skyfsm/p/7324346.html)