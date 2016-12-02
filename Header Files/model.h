#ifndef __MODEL_H__
#define __MODEL_H__

#include <GLTools.h>
#include "math3d.h"

class Unit{  //��̬��Ԫ�Ͷ�̬��Ԫ�����ݽṹ

public:
	Unit(){ Num = 0; }

	//////////////////////////////////////////////////////////////////////////////////////
	//inline void getName(char *name){ name = Name;}

	inline int getNum(){return Num;}

	int getIndex(int n){ if(n<Num) return Index[n]; }

	void getStep(int n, M3DVector3f &s){
		if(n<Num){
			s[0] = Steps[n][0];
			s[1] = Steps[n][1];
			s[2] = Steps[n][2];
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	//inline void setName(char *name){ Name = name; }

	inline void setNum(int n){ Num = n; Index = new int[n]; Steps = new M3DVector3f[n]; }

	void setIndex(int n,int i){ if(n<Num) Index[n] = i; }

	void setStep(int n, M3DVector3f s){
		if(n<Num){
			Steps[n][0] = s[0];
			Steps[n][1] = s[1];
			Steps[n][2] = s[2];
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////

private:
	int         Num;             //ÿ����Ԫ�Ķ������Ŀ
	int         *Index;          //��������б�
	M3DVector3f *Steps;          //ÿ�������Ӧ������
};

class Model{

public:
	Model();

	inline int nVertex()const {return n_vertice;}
	inline int nFace(){return n_faces;};
	inline int nSUs(){return SUsNum;};
	inline int nAUs(){return AUsNum;};

	bool open(const char *file);                                  //��һ��wfm�ļ�
	bool write(const char *file);                                 //дһ��wfm�ļ�
	void copyVerticesData(int vertexnum, M3DVector3f *vertex);
	void copyFacesData(int facenum, M3DVector3i *face);
	void copySUsData(int susnum, Unit *sus);
	void copyAUsData(int ausnum, Unit *aus);
	void copySPsData(int spnum, float *sps);
	void copyAPsData(int apnum, float *aps);
    bool loadTexImage(const char *f);                             //��������ͼƬ

	void          getVertex(int n, M3DVector3f &vertex) const;
	void          getTransCoords(int n, M3DVector3f &coord);
	void          getTexCoords(int n, float &x, float &y);
	void          getFace(int n, M3DVector3i &face);
	int           getSUNum(int n);
	int           getAUNum(int n);
	void          getSUIndex(int n, int *index);
	void          getAUIndex(int n, int *index);
	void          getSUSteps(int n, M3DVector3f *steps);
	void          getAUSteps(int n, M3DVector3f *steps);
	const char *  getSUsName(int n);
	const char *  getAUsName(int n);
	float         getSP(int n);
	float         getAP(int n);
	//inline int  getImageHeight(){return image.height();}
	//inline int  getImageWidth(){return image.width();}
	//GLbyte *    getTexData();

	void setVertex(int n, M3DVector3f vertex);
	void setTransCoords(int n, M3DVector3f coord);
	void setTexCoords(int n, float x, float y);
	void setFace(int n, M3DVector3i face);
	void setSU(int n, Unit su);
	void setAU(int n, Unit au);
	void setSUsName(int n, const char *name);
	void setAUsName(int n, const char *name);
	void setSP(int n, float sp);
	void setAP(int n, float ap);

	void addSP(int n, float sp);
	void addAP(int n, float ap);
	void applySP();             //TransCoords * SP �� TransCoords
	void applyAP();             //TransCoords * AP �� TransCoords
	//void updateModel();

	void clear();      //���ģ�Ͷ������������
	void clearAP();
	~Model();

private:
	int          n_vertice;  //������Ŀ
	int          n_faces;    //����������Ŀ
	int          SUsNum;
	int          AUsNum;

	const char   **SUsName;
	const char   **AUsName;
	const char   *TexImage;         //����ͼƬ

	float        *SP;
	float        *AP;

	bool         TexExist;          //����ͼƬ�Ƿ���Ч

	M3DVector3f  *vertices;             //ָ�򶥵�����
	M3DVector3f  *TransCoords;  //
	M3DVector3i  *faces;                //ָ��������������
	M3DVector2f  *texture_coordinates;  //ָ���ά������������
	Unit         *SU;
	Unit         *AU;

	//Image        image;
};

void makeCandide3Model(Model &candide3);
void makeSUs(Model &candide3);
void makeAUs(Model &candide3);

#endif