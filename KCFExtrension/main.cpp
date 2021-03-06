/*</readme>
		time:2019/1/26     created by:zpp
简介：
	通过拓展对比帧数，增加KCF算法的准确度，将原来KCF只在两帧比较的基础上拓展为在六帧的跨度上比较。

摘要：
	使用余弦相似度，比较两帧之间的相似度:(A*B)/(||A||*||B||）。为了在矩阵上求相似度(Mat),将矩阵按列拆分为列向量col_vector(i),
再对col_vector(i)、col_vector(j)求列余弦相似度。将两矩阵的列余弦相似度累加，求平均值AVGSimi近似作为两矩阵A、B的相似度。前六帧与
现在帧的平均相似度定义为：AVGSIMI=SUM(imgSeq[i])/a i=[1,6]。
	我们可以将相似度偏小作为脱锁的指标，设alpah=0.2,则if(alpha>AVGSIMI),判定跟踪算法为脱锁，保留“脱锁前特征”，……现在还没有好的方
法解决重新锁定的问题。
	为了测试该算法的效果，我们使用OTB-100中的跟踪算法测试序列。进行测试。
	改算法还未能移植到KCF源代码中………  '='
*/

#include<iostream>
#include<opencv2/opencv.hpp>
float GetSim(const cv::Mat image_1, cv::Mat image_2);
int main() {
	//--------------------------------------------------------------------------
		//载入测试图像序列
	std::cout << "序列示例文件名：";
	std::cout << "Car1、Car4、Biker、Basketball、Bird1、Box……" << std::endl;
	std::cout << "Enter the name of the file can be test:";
	std::string name;
	std::cin >> name;
	std::string first_file = name + "/img/%4d.jpg";
	if (!first_file.empty())
		std::cout << "文件加载成功……\n";										//<--可设置断点
	else
	{
		std::cout << "文件加载失败……\n请检查文件名";
		return 0;
	}
	//--------------------------------------------------------------------------
	cv::VideoCapture capture(first_file);									//读取测试图片序列
	cv::Mat image;
	//cv::Mat Temp;
	bool stop(false);		//初始化stop
	//capture >> Temp;
	//----------------------------
	int counter = 1;			//计数器从“1”开始
	float SIMI = 0;				//累加矩阵相似度
	float AVGSIMI = 0;			//前六帧与现在帧平均相似度
	const float Q = 0.2;		//阈值，可设置
	const int a = 6;			//前六帧
	cv::Mat imgSeq[a + 1];		//不使用imgSeq[0]
	//----------------------------

//--------------------------------------------------------------------------
	//序列测试循环
	while (!stop)
	{
		capture >> image;
		if (image.empty())
			return 0;
		cv::imshow("Show Pictureflow", image);

		//std::cout << "现在帧的通道数："<<image.channels() << std::endl;			//<--可设置断点
		//std::cout << "输出相似度：" << GetSim(image, Temp) << std::endl;
		//-----------------------------------------------------------------
			//保留前六帧
		if (counter == a + 1) {			//初始化成功，开始进行运算。
			for (int i = 1; i <= a; i++) {
				SIMI += GetSim(image, imgSeq[i]);
			}
			AVGSIMI = SIMI / a;
			std::cout << "前六帧相似度：" << AVGSIMI << std::endl;
			if (AVGSIMI < Q)
			{
				std::cout << "差异过大，判定为脱锁！" << std::endl;
				std::cout << "…………" << std::endl;
			}
			else
				std::cout << "未脱锁" << std::endl;

			SIMI = 0;															//重新置零
			//更新图片序列
			for (int i = 1; i < a; i++) {
				imgSeq[i] = imgSeq[i + 1];										//后移，用队列更加麻烦
			}
			capture >> imgSeq[a];												//更新第六帧
		}
		//初始化图片序列imgSeq[]
		if (counter < a + 1)
		{
			capture >> imgSeq[counter];
			counter++;															//counter==6时停止计数
		}
		//----------------------------------------------------------------------
		//Temp = image;															//<--
		if (cv::waitKey(10) >= 0)												//延时是10ms,可设置。
			stop = true;
	}

	capture.release();															//释放内存、可要可不要
	getchar();

	return 0;
}

//-----------------------------------------------------GetSim(image_1,image_2)-----------------------------------------------
//求两帧之间相似度算法实现，输入：两张图片，输出：相似度。注：只能在规模相同的单通道图像使用。
float GetSim(const cv::Mat image_1, const cv::Mat image_2)
{

	if (image_1.empty() & image_2.empty())			//<--异常1
		return 0.0;

	//----------------------------------------------------------------------
		//转换通道数
	cv::Mat  Temp_1, Temp_2;
	if (image_1.channels() != 1)
	{
		//std::cout << "需要进行通道数转换……\n";
		cv::cvtColor(image_1, Temp_1, CV_BGR2GRAY);						//KCf因为KCF算法里面是一通道图像（灰度图），也为了计算方便
		cv::cvtColor(image_2, Temp_2, CV_BGR2GRAY);
		//std::cout << "转换成功。\n";
		//std::cout << "转换：" << Temp_1.channels() << std::endl;
	}
	//----------------------------------------------------------------------
		//转换数值
	Temp_1.convertTo(Temp_1, CV_32F, 1 / 255.f);						//本来可以不做这个运算，但是KCF里面的图像数值，是浮点型，，这个影响很大
	Temp_2.convertTo(Temp_2, CV_32F, 1 / 255.f);
	//---------------------------------------------------------------------
	float DOT = 0;														//列向量内积
	int SUM_1 = 0;														//image_1列向量模的平方
	int SUM_2 = 0;														//image_2列向量膜的平方
	float SIM = 0;														//列相似度
//---------------------------------------------------------------------
	//计算平均相似度
	if (Temp_1.size != Temp_2.size) {
		std::cout << "Wrong,The pictures'size is defference!\n";
		return 0;
	}
	else {
		//任取一张图片的规格
		int COLS = Temp_1.cols;											 //行数
		int ROWS = Temp_1.rows;											 //列数
		//--------------------------------------------------------------------------//在这我又将数据类型转换为float->uchar,应为float类型的数据，计算
		//按列求余弦相似度															//效果实在是不好，但是KCF算法里面的使用的是float->CV_32F。
		for (int j = 0; j <= COLS; j++) {											//************************可能要改算法************************
			for (int i = 0; i <= ROWS; i++) {										//
				DOT = DOT + Temp_1.at<uchar>(i, j) * Temp_2.at<uchar>(i, j);		//求内积
				SUM_1 = SUM_1 + Temp_1.at<uchar>(i, j) * Temp_1.at<uchar>(i, j);	//求模1
				SUM_2 = SUM_2 + Temp_2.at<uchar>(i, j) * Temp_2.at<uchar>(i, j);	//求模2
			}																		//
			SIM = SIM + DOT / (sqrt(SUM_1)*sqrt(SUM_2));				//相似度计算、累加相似度
			DOT = 0; SUM_1 = 0; SUM_2 = 0;								//变量置空
		}																			//
		//--------------------------------------------------------------------------//
		float AVGSIM = SIM / COLS;										//平均相似度
		return AVGSIM;													//返回平均相似度
	}

}
//***********************************************************GetSimi(image_1,image_2)**********************************************