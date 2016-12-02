#include "model.h"
#include "io.h"
#include <fstream>

using namespace std;

Model::Model(){
	n_vertice = 0;
	n_faces = 0;
	SUsNum = AUsNum = 0;
	TexExist = false;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
bool Model::open(const char *f){
	int nVerts, nFaces;
	int content = 0,            //���ݵľ�������:1.����;2.ƽ��;3.��̬��Ԫ;4.��̬��Ԫ;5.��̬����;6.��̬����;7.��������;
		te;

	char line[100];
	char *title[] = {"# END OF FILE","# VERTEX LIST:","# FACE LIST:","# ANIMATION UNITS LIST:","# SHAPE UNITS LIST:",
					 "# ANIMATION PARAMETERS:","# SHAPE PARAMETERS:","# TEXTURE:"};

	string *sname,*aname;
	string t0(title[0]),    //�ļ�������ʶ
		   t1(title[1]),    //�����б�
		   t2(title[2]),    //ƽ���б�
		   t3(title[3]),    //��̬��Ԫ�б�
		   t4(title[4]),    //��̬��Ԫ�б�
	       t5(title[5]),    //��̬�����б�
		   t6(title[6]),    //��̬�����б�
	       t7(title[7]);    //���������б�
		   
	M3DVector3f *vertex;
	M3DVector3i *face;

	ifstream file(f,ios::in);
	if(file){
		do{
			file.getline(line,100);

			if(!t1.compare(line))content = 1;  //���������б�
			if(!t2.compare(line))content = 2;  //����ƽ���б�
			if(!t3.compare(line))content = 3;  //������̬��Ԫ�б�
			if(!t4.compare(line))content = 4;  //������̬��Ԫ�б�
			if(!t5.compare(line))content = 5;  //������̬�����б�
			if(!t6.compare(line))content = 6;  //������̬�����б�
			if(!t7.compare(line))content = 7;  //�������������б�

			switch (content){

			case 1:                           //���붥���б�
				file >> line;
				nVerts = atoi(line);
				vertex = new M3DVector3f[nVerts];
				for(int i = 0; i < nVerts; i++){
					file >> line;
					vertex[i][0] = atof(line);
					file >> line;
					vertex[i][1] = atof(line);
					file >> line;
					vertex[i][2] = atof(line);
				}
				copyVerticesData(nVerts,vertex);
				content = 0;
				break;

			case 2:                           //����ƽ���б�
				file.getline(line,100);
				nFaces = atoi(line);
				face = new M3DVector3i[nFaces];
				for(int i = 0; i < nFaces; i++){
					file >> line;
					face[i][0] = atoi(line);
					file >> line;
					face[i][1] = atoi(line);
					file >> line;
					face[i][2] = atoi(line);
				}
				copyFacesData(nFaces,face);
				content = 0;
				break;

			case 3:                           //���붯̬��Ԫ�б�
				const char **AUsName;
				int AUNum, nAU, *AUIndex;
				M3DVector3f *AUStep;
				Unit *AU;

				file.getline(line,100);
				AUNum = atoi(line);
				
				AU = new Unit[AUNum];
				AUsName = new const char*[AUNum];
				aname = new string[AUNum];

				for(int i = 0; i < AUNum; i++){
					do{
						file.getline(line,100);
					}while(line[0] != '#');
					aname[i].append(line);
					AUsName[i] = aname[i].data();
					while(line[0] == '#')
						file.getline(line,100);
					nAU = atoi(line);
					AUIndex = new int[nAU];
					AUStep = new M3DVector3f[nAU];
			
					AU[i].setNum(nAU);
					for(int j = 0; j < nAU; j++){
						file >> line;
						AUIndex[j] = atoi(line);
						file >> line;
						AUStep[j][0] = atof(line);
						file >> line;
						AUStep[j][1] = atof(line);
						file >> line;
						AUStep[j][2] = atof(line);
						AU[i].setIndex(j,AUIndex[j]);
						AU[i].setStep(j,AUStep[j]);
					}
				}
				copyAUsData(AUNum,AU);

				//��ȡAUsName����
				for(int i = 0; i < AUNum; i++)
					setAUsName(i,AUsName[i]);

				content = 0;
				break;

			case 4:                           //���뾲̬��Ԫ�б�
				const char **SUsName;
				int SUNum, nSU, *SUIndex;
				M3DVector3f *SUStep;
				Unit *SU;

				file.getline(line,100);
				SUNum = atoi(line);
				
				SU = new Unit[SUNum];
				SUsName = new const char*[SUNum];
				sname = new string[SUNum];

				for(int i = 0; i < SUNum; i++){
					do{
						file.getline(line,100);
					}while(line[0] != '#');
					sname[i].append(line);
					SUsName[i] = sname[i].data();
					while(line[0] == '#')
						file.getline(line,100);
					nSU = atoi(line);
					SUIndex = new int[nSU];
					SUStep = new M3DVector3f[nSU];
			
					SU[i].setNum(nSU);
					for(int j = 0; j < nSU; j++){
						file >> line;
						SUIndex[j] = atoi(line);
						file >> line;
						SUStep[j][0] = atof(line);
						file >> line;
						SUStep[j][1] = atof(line);
						file >> line;
						SUStep[j][2] = atof(line);
						SU[i].setIndex(j,SUIndex[j]);
						SU[i].setStep(j,SUStep[j]);
					}
				}
				copySUsData(SUNum,SU);

				//��ȡSUsName����
				for(int i = 0; i < SUNum; i++)
					setSUsName(i,SUsName[i]);
				
				content = 0;
				break;

			case 5:
				for(int i = 0; i < AUsNum; i++){
					file.getline(line,100);
					AP[i] = atof(line);
				}
				content = 0;
				break;

			case 6:
				for(int i = 0; i < SUsNum; i++){
					file.getline(line,100);
					SP[i] = atof(line);
				}
				content = 0;
				break;

			case 7:                           //������ͼƬ����������
				file.getline(line,100);
				te = atoi(line);
				if(te == 1){
					TexExist = true;
					file.getline(line,100);
					string temp(line);
					TexImage = temp.data();
				}
				else TexExist = false;

				for(int i = 0; i < n_vertice; i++){
					file.getline(line,100);
					char2TexCoord(line,texture_coordinates[i][0],texture_coordinates[i][1]);
				}
				content = 0;
				break;

			default:;
			}
		}while(t0.compare(line));             //�����ļ�β
		file.close();
		cout<<"Read model succeeded."<<endl;
		return true;
	}else{
		cerr<<"Read model error!"<<endl;
		return false;
	}
}

bool Model::write(const char *f){
	ofstream file(f,ios::out);

	if(file){
		//д��Vertex����
		file<<"# VERTEX LIST:"<<endl;
		file<<n_vertice<<endl;
		for(int i = 0; i < n_vertice; i++){
			file<<vertices[i][0]<<' '<<vertices[i][1]<<' '<<vertices[i][2]<<endl;
		}
		file<<endl<<endl;

		//д��Face����
		file<<"# FACE LIST:"<<endl;
		file<<n_faces<<endl;
		for(int i = 0; i < n_faces; i++){
			file<<faces[i][0]<<' '<<faces[i][1]<<' '<<faces[i][2]<<endl;
		}
		file<<endl<<endl;

		//д��AU����
		file<<"# ANIMATION UNITS LIST:"<<endl;
		file<<AUsNum<<endl<<endl;
		for(int i = 0; i < AUsNum; i++){
			file<<AUsName[i]<<endl;
			file<<AU[i].getNum()<<endl;
			M3DVector3f s;
			for(int j = 0; j < AU[i].getNum(); j++){
				AU[i].getStep(j,s);
				file<<AU[i].getIndex(j)<<' '<<s[0]<<' '<<s[1]<<' '<<s[2]<<endl;
			}
			file<<endl;
		}
		file<<endl;

		//д��SU����
		file<<"# SHAPE UNITS LIST:"<<endl;
		file<<SUsNum<<endl<<endl;
		for(int i = 0; i < SUsNum; i++){
			file<<SUsName[i]<<endl;
			file<<SU[i].getNum()<<endl;
			M3DVector3f s;
			for(int j = 0; j < SU[i].getNum(); j++){
				SU[i].getStep(j,s);
				file<<SU[i].getIndex(j)<<' '<<s[0]<<' '<<s[1]<<' '<<s[2]<<endl;
			}
			file<<endl;
		}

		//д��AP����
		file<<"# ANIMATION PARAMETERS:"<<endl;
		for(int i = 0; i < AUsNum; i++)
			file<<AP[i]<<endl;
		file<<endl;

		//д��SP����
		file<<"# SHAPE PARAMETERS:"<<endl;
		for(int i = 0; i < SUsNum; i++)
			file<<SP[i]<<endl;
		file<<endl;

		//д��������������
		file<<"# TEXTURE:"<<endl;
		if(TexExist)file<<1<<endl<<TexImage<<endl;
		else file<<0<<endl;
		for(int i = 0; i < n_vertice; i++)
			file<<texture_coordinates[i][0]<<' '<<texture_coordinates[i][1]<<endl;
		file<<endl;

		file<<"# END OF FILE";

		file.close();
		cout<<"Write model succeeded."<<endl;
		return true;
	}else{
		cerr<<"Write model error!"<<endl;
		return false;
	}
}

void Model::copyVerticesData(int vertexnum, M3DVector3f *vertex){
	n_vertice = vertexnum;
	vertices = new M3DVector3f[n_vertice];
	TransCoords = new M3DVector3f[n_vertice];
	texture_coordinates = new M3DVector2f[n_vertice];
	for(int i = 0; i < n_vertice; i++){
		setVertex(i,vertex[i]);
		setTransCoords(i,vertex[i]);
		setTexCoords(i,0,0);
	}
}

void Model::copyFacesData(int facenum, M3DVector3i *face){
	n_faces = facenum;
	faces = new M3DVector3i[n_faces];
	for(int i = 0; i < n_faces; i++)
		setFace(i,face[i]);
}

void Model::copySUsData(int susnum, Unit *sus){
	SUsNum = susnum;
	SU = new Unit[SUsNum];
	SP = new float[SUsNum];
	SUsName = new const char*[SUsNum];
	for(int i = 0; i < SUsNum; i++){
		setSU(i,sus[i]);
		SP[i] = 0;
	}
}

void Model::copyAUsData(int ausnum, Unit *aus){
	AUsNum = ausnum;
	AU = new Unit[AUsNum];
	AP = new float[AUsNum];
	AUsName = new const char*[AUsNum];
	for(int i = 0; i < AUsNum; i++){
		setAU(i,aus[i]);
		AP[i] = 0;
	}
}

void Model::copySPsData(int spnum, float *sps){
	if(spnum == SUsNum)
		for(int i = 0; i < SUsNum; i++)
			SP[i] = sps[i];
}

void Model::copyAPsData(int apnum, float *aps){
	if(apnum == AUsNum)
		for(int i = 0; i < AUsNum; i++)
			AP[i] = aps[i];
}

bool Model::loadTexImage(const char *f){
	ifstream file(f,ios::in);
	if(file){
		TexImage = f;
		TexExist = true;
	}else TexExist = false;

	return TexExist;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::getVertex(int n, M3DVector3f &vertex) const{
	if(n<n_vertice){
		vertex[0] = vertices[n][0];
		vertex[1] = vertices[n][1];
		vertex[2] = vertices[n][2];
	}else{
		vertex[0] = 0.0f;
		vertex[1] = 0.0f;
		vertex[2] = 0.0f;
	}
}

void Model::getTransCoords(int n, M3DVector3f &coord){
	if(n<n_vertice){
		coord[0] = TransCoords[n][0];
		coord[1] = TransCoords[n][1];
		coord[2] = TransCoords[n][2];
	}else{
		coord[0] = 0.0f;
		coord[1] = 0.0f;
		coord[2] = 0.0f;
	}
}

void Model::getTexCoords(int n, float &x, float &y){
	if(n<n_vertice){
		x = texture_coordinates[n][0];
		y = texture_coordinates[n][1];
	}else{
		x = 0.0f;
		y = 0.0f;
	}
}

void Model::getFace(int n, M3DVector3i &face){
	if(n<n_faces){
		face[0] = faces[n][0];
		face[1] = faces[n][1];
		face[2] = faces[n][2];
	}else{
		face[0] = 0;
		face[1] = 0;
		face[2] = 0;
	}
}

int  Model::getSUNum(int n){
	if(n<SUsNum) return SU[n].getNum();
}

int  Model::getAUNum(int n){
	if(n<AUsNum) return AU[n].getNum();
}

void Model::getSUIndex(int n, int *index){
	if(n<SUsNum)
		for(int i = 0; i < SU[n].getNum(); i++)
			index[i] = SU[n].getIndex(i);
}

void Model::getAUIndex(int n, int *index){
	if(n<AUsNum)
		for(int i = 0; i < AU[n].getNum(); i++)
			index[i] = AU[n].getIndex(i);
}

void Model::getSUSteps(int n, M3DVector3f *steps){
	if(n<SUsNum)
		for(int i = 0; i < SU[n].getNum(); i++)
			SU[n].getStep(i,steps[i]);
}

void Model::getAUSteps(int n, M3DVector3f *steps){
	if(n<AUsNum)
		for(int i = 0; i < AU[n].getNum(); i++)
			AU[n].getStep(i,steps[i]);
}

const char *  Model::getSUsName(int n){
	if(n<SUsNum) return SUsName[n];
}

const char *  Model::getAUsName(int n){
	if(n<AUsNum) return AUsName[n];
}

float Model::getSP(int n){
	if(n<SUsNum) return SP[n];
}

float Model::getAP(int n){
	if(n<AUsNum) return AP[n];
}

//GLbyte * Model::getTexData(){
//	return image.genTexData();
//}


//////////////////////////////////////////////////////////////////////////////////////////////////////
void Model::setVertex(int n, M3DVector3f vertex){
	if(n<n_vertice){
		vertices[n][0] = vertex[0];
		vertices[n][1] = vertex[1];
		vertices[n][2] = vertex[2];
	}
}

void Model::setTransCoords(int n, M3DVector3f coord){
	if(n<n_vertice){
		TransCoords[n][0] = coord[0];
		TransCoords[n][1] = coord[1];
		TransCoords[n][2] = coord[2];
	}
}

void Model::setTexCoords(int n, float x, float y){
	if(n<n_vertice){
		texture_coordinates[n][0] = x;
		texture_coordinates[n][1] = y;
	}
}

void Model::setFace(int n, M3DVector3i face){
	if(n<n_faces){
		faces[n][0] = face[0];
		faces[n][1] = face[1];
		faces[n][2] = face[2];
	}
}

void Model::setSU(int n, Unit su){
	if(n<SUsNum){
		int num = su.getNum();
		SU[n].setNum(num);

		M3DVector3f s;
		for(int i = 0; i < num; i++){
			SU[n].setIndex(i,su.getIndex(i));
			su.getStep(i,s);
			SU[n].setStep(i,s);
		}
	}
}

void Model::setAU(int n, Unit au){
	if(n<AUsNum){
		int num = au.getNum();
		AU[n].setNum(num);

		M3DVector3f s;
		for(int i = 0; i < num; i++){
			AU[n].setIndex(i,au.getIndex(i));
			au.getStep(i,s);
			AU[n].setStep(i,s);
		}
	}
}

void Model::setSUsName(int n, const char *name){
	if(n<SUsNum){
		SUsName[n] = name;
	}
}

void Model::setAUsName(int n, const char *name){
	if(n<AUsNum){
		AUsName[n] = name;
	}
}

void Model::setSP(int n, float sp){
	if(n<SUsNum) SP[n] = sp;
}

void Model::setAP(int n, float ap){
	if(n<AUsNum) AP[n] = ap;
}

//void Model::addSP(int n, float sp){
//	if(n<SUsNum) SP[n] += sp;
//}
//
//void Model::addAP(int n, float ap){
//	if(n<AUsNum) AP[n] += ap;
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Model::addSP(int n, float sp){
	if(n<SUsNum){
		SP[n] += sp;
		M3DVector3f result, orignal, step;
		for(int j = 0; j < SU[n].getNum(); j++){
			getTransCoords(SU[n].getIndex(j), orignal);
			SU[n].getStep(j,step);
			step[0] *= sp; step[1] *= sp; step[2] *= sp;
			m3dAddVectors3(result, orignal, step);
			setTransCoords(SU[n].getIndex(j), result);
		}
	}
}

void Model::addAP(int n, float ap){
	if(n<AUsNum){
		AP[n] += ap;
		M3DVector3f result, orignal, step;
		for(int j = 0; j < AU[n].getNum(); j++){
			getTransCoords(AU[n].getIndex(j), orignal);
			AU[n].getStep(j,step);
			step[0] *= ap; step[1] *= ap; step[2] *= ap;
			m3dAddVectors3(result, orignal, step);
			setTransCoords(AU[n].getIndex(j), result);
		}
	}
}

void Model::applySP(){
	M3DVector3f result, orignal, step;
	for(int i = 0; i < SUsNum; i++)
		for(int j = 0; j < SU[i].getNum(); j++){
			getTransCoords(SU[i].getIndex(j), orignal);
			SU[i].getStep(j,step);
			step[0] *= SP[i]; step[1] *= SP[i]; step[2] *= SP[i];;
			m3dAddVectors3(result, orignal, step);
			setTransCoords(SU[i].getIndex(j), result);
		}
}

void Model::applyAP(){
	M3DVector3f result, orignal, step;
	for(int i = 0; i < AUsNum; i++)
		for(int j = 0; j < AU[i].getNum(); j++){
			getTransCoords(AU[i].getIndex(j), orignal);
			AU[i].getStep(j,step);
			step[0] *= AP[i]; step[1] *= AP[i]; step[2] *= AP[i];
			m3dAddVectors3(result, orignal, step);
			setTransCoords(AU[i].getIndex(j), result);
		}
}

/////////////////////////////////////////////////////////////////////////////////
void Model::clearAP(){
	M3DVector3f result, orignal, step;

	for(int i = 0; i < AUsNum; i++)
		setAP(i,0);

	for(int j = 0; j < n_vertice; j++){
		getVertex(j, orignal);
		setTransCoords(j, orignal);
	}

	applySP();
}

//void Model::updateModel(){
//	for(int i = 0; i < n_vertice; i++){
//		Vertex[i][0] = TransCoords[i][0];
//		Vertex[i][1] = TransCoords[i][1];
//		Vertex[i][2] = TransCoords[i][2];
//	}
//}


///////////////////////////////////////////////////////
void Model::clear(){
	n_vertice = 0;
	n_faces = 0;
	SUsNum = 0;
	AUsNum = 0;
	delete[] vertices; vertices = NULL;
	delete[] faces;   faces = NULL;
	delete[] SU;     SU = NULL;
	delete[] AU;     AU = NULL;
}

Model::~Model(){
	clear();
}