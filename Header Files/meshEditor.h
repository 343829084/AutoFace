#ifndef __MESH_EDITOR_H__
#define __MESH_EDITOR_H__

#include <iostream>
#include "math3d.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;

//��ȡ.off�ļ������㡢�����ݴ洢��������
int myReadMesh(const char* path, M3DVector3f*& vertice_array, M3DVector3i*& faces_array, int& number_vertice, int& number_faces);

//�������еĵ㡢�����ݱ���Ϊ.off�ļ�
int myWriteMesh(float *vertice, M3DVector3i* face, int number_vertice, int number_faces);

#endif