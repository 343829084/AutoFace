#ifndef _RBF_H
#define _RBF_H

#include<opencv2/opencv.hpp>
#include<cmath>

using namespace std;
using namespace cv;

enum KERNEL_TYPE{GAUSS, REFLECTED_SIGMOIDAL, INVERSE_MULTIQUADRICS};


/*��������������ֻ��Զ�ά�������*/
class RBF{

public:
	RBF();
	bool train(InputArray src, InputArray dst);
	bool predict(InputArray src, OutputArray dst);
	float kernel(double x);
	inline void set_kernel_type(KERNEL_TYPE kt){ kernel_type = kt; }
	inline void set_sigma(double s){ sigma = s; cout<<sigma<<endl;}

private:
	double sigma;
	int centers_num;  //�������ά��
	int dim;          //����������ά��
	KERNEL_TYPE kernel_type;
	Mat centers;      //���ĵ�
	Mat omigas;       //Ȩֵ
};

#endif