/*</readme>
		time:2019/1/26     created by:zpp
��飺
	ͨ����չ�Ա�֡��������KCF�㷨��׼ȷ�ȣ���ԭ��KCFֻ����֡�ȽϵĻ�������չΪ����֡�Ŀ���ϱȽϡ�

ժҪ��
	ʹ���������ƶȣ��Ƚ���֮֡������ƶ�:(A*B)/(||A||*||B||����Ϊ���ھ����������ƶ�(Mat),�������в��Ϊ������col_vector(i),
�ٶ�col_vector(i)��col_vector(j)�����������ƶȡ�������������������ƶ��ۼӣ���ƽ��ֵAVGSimi������Ϊ������A��B�����ƶȡ�ǰ��֡��
����֡��ƽ�����ƶȶ���Ϊ��AVGSIMI=SUM(imgSeq[i])/a i=[1,6]��
	���ǿ��Խ����ƶ�ƫС��Ϊ������ָ�꣬��alpah=0.2,��if(alpha>AVGSIMI),�ж������㷨Ϊ����������������ǰ���������������ڻ�û�кõķ�
������������������⡣
	Ϊ�˲��Ը��㷨��Ч��������ʹ��OTB-100�еĸ����㷨�������С����в��ԡ�
	���㷨��δ����ֲ��KCFԴ�����С�����  '='
*/

#include<iostream>
#include<opencv2/opencv.hpp>
float GetSim(const cv::Mat image_1, cv::Mat image_2);
int main() {
	//--------------------------------------------------------------------------
		//�������ͼ������
	std::cout << "����ʾ���ļ�����";
	std::cout << "Car1��Car4��Biker��Basketball��Bird1��Box����" << std::endl;
	std::cout << "Enter the name of the file can be test:";
	std::string name;
	std::cin >> name;
	std::string first_file = name + "/img/%4d.jpg";
	if (!first_file.empty())
		std::cout << "�ļ����سɹ�����\n";										//<--�����öϵ�
	else
	{
		std::cout << "�ļ�����ʧ�ܡ���\n�����ļ���";
		return 0;
	}
	//--------------------------------------------------------------------------
	cv::VideoCapture capture(first_file);									//��ȡ����ͼƬ����
	cv::Mat image;
	//cv::Mat Temp;
	bool stop(false);		//��ʼ��stop
	//capture >> Temp;
	//----------------------------
	int counter = 1;			//�������ӡ�1����ʼ
	float SIMI = 0;				//�ۼӾ������ƶ�
	float AVGSIMI = 0;			//ǰ��֡������֡ƽ�����ƶ�
	const float Q = 0.2;		//��ֵ��������
	const int a = 6;			//ǰ��֡
	cv::Mat imgSeq[a + 1];		//��ʹ��imgSeq[0]
	//----------------------------

//--------------------------------------------------------------------------
	//���в���ѭ��
	while (!stop)
	{
		capture >> image;
		if (image.empty())
			return 0;
		cv::imshow("Show Pictureflow", image);

		//std::cout << "����֡��ͨ������"<<image.channels() << std::endl;			//<--�����öϵ�
		//std::cout << "������ƶȣ�" << GetSim(image, Temp) << std::endl;
		//-----------------------------------------------------------------
			//����ǰ��֡
		if (counter == a + 1) {			//��ʼ���ɹ�����ʼ�������㡣
			for (int i = 1; i <= a; i++) {
				SIMI += GetSim(image, imgSeq[i]);
			}
			AVGSIMI = SIMI / a;
			std::cout << "ǰ��֡���ƶȣ�" << AVGSIMI << std::endl;
			if (AVGSIMI < Q)
			{
				std::cout << "��������ж�Ϊ������" << std::endl;
				std::cout << "��������" << std::endl;
			}
			else
				std::cout << "δ����" << std::endl;

			SIMI = 0;															//��������
			//����ͼƬ����
			for (int i = 1; i < a; i++) {
				imgSeq[i] = imgSeq[i + 1];										//���ƣ��ö��и����鷳
			}
			capture >> imgSeq[a];												//���µ���֡
		}
		//��ʼ��ͼƬ����imgSeq[]
		if (counter < a + 1)
		{
			capture >> imgSeq[counter];
			counter++;															//counter==6ʱֹͣ����
		}
		//----------------------------------------------------------------------
		//Temp = image;															//<--
		if (cv::waitKey(10) >= 0)												//��ʱ��10ms,�����á�
			stop = true;
	}

	capture.release();															//�ͷ��ڴ桢��Ҫ�ɲ�Ҫ
	getchar();

	return 0;
}

//-----------------------------------------------------GetSim(image_1,image_2)-----------------------------------------------
//����֮֡�����ƶ��㷨ʵ�֣����룺����ͼƬ����������ƶȡ�ע��ֻ���ڹ�ģ��ͬ�ĵ�ͨ��ͼ��ʹ�á�
float GetSim(const cv::Mat image_1, const cv::Mat image_2)
{

	if (image_1.empty() & image_2.empty())			//<--�쳣1
		return 0.0;

	//----------------------------------------------------------------------
		//ת��ͨ����
	cv::Mat  Temp_1, Temp_2;
	if (image_1.channels() != 1)
	{
		//std::cout << "��Ҫ����ͨ����ת������\n";
		cv::cvtColor(image_1, Temp_1, CV_BGR2GRAY);						//KCf��ΪKCF�㷨������һͨ��ͼ�񣨻Ҷ�ͼ����ҲΪ�˼��㷽��
		cv::cvtColor(image_2, Temp_2, CV_BGR2GRAY);
		//std::cout << "ת���ɹ���\n";
		//std::cout << "ת����" << Temp_1.channels() << std::endl;
	}
	//----------------------------------------------------------------------
		//ת����ֵ
	Temp_1.convertTo(Temp_1, CV_32F, 1 / 255.f);						//�������Բ���������㣬����KCF�����ͼ����ֵ���Ǹ����ͣ������Ӱ��ܴ�
	Temp_2.convertTo(Temp_2, CV_32F, 1 / 255.f);
	//---------------------------------------------------------------------
	float DOT = 0;														//�������ڻ�
	int SUM_1 = 0;														//image_1������ģ��ƽ��
	int SUM_2 = 0;														//image_2������Ĥ��ƽ��
	float SIM = 0;														//�����ƶ�
//---------------------------------------------------------------------
	//����ƽ�����ƶ�
	if (Temp_1.size != Temp_2.size) {
		std::cout << "Wrong,The pictures'size is defference!\n";
		return 0;
	}
	else {
		//��ȡһ��ͼƬ�Ĺ��
		int COLS = Temp_1.cols;											 //����
		int ROWS = Temp_1.rows;											 //����
		//--------------------------------------------------------------------------//�������ֽ���������ת��Ϊfloat->uchar,ӦΪfloat���͵����ݣ�����
		//�������������ƶ�															//Ч��ʵ���ǲ��ã�����KCF�㷨�����ʹ�õ���float->CV_32F��
		for (int j = 0; j <= COLS; j++) {											//************************����Ҫ���㷨************************
			for (int i = 0; i <= ROWS; i++) {										//
				DOT = DOT + Temp_1.at<uchar>(i, j) * Temp_2.at<uchar>(i, j);		//���ڻ�
				SUM_1 = SUM_1 + Temp_1.at<uchar>(i, j) * Temp_1.at<uchar>(i, j);	//��ģ1
				SUM_2 = SUM_2 + Temp_2.at<uchar>(i, j) * Temp_2.at<uchar>(i, j);	//��ģ2
			}																		//
			SIM = SIM + DOT / (sqrt(SUM_1)*sqrt(SUM_2));				//���ƶȼ��㡢�ۼ����ƶ�
			DOT = 0; SUM_1 = 0; SUM_2 = 0;								//�����ÿ�
		}																			//
		//--------------------------------------------------------------------------//
		float AVGSIM = SIM / COLS;										//ƽ�����ƶ�
		return AVGSIM;													//����ƽ�����ƶ�
	}

}
//***********************************************************GetSimi(image_1,image_2)**********************************************