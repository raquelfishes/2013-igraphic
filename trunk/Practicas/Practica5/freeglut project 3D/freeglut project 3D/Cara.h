#pragma once
#include "VerticeNormal.h"

#include <vector>
#include <iostream>

using namespace std;

class Cara
{
private:
	int numV; // numero de vertices (Por defecto 4)

	std::vector<VerticeNormal*> *indicesVN;

public:
	Cara(int nV);
	int getNumV();
	int getNormalIndex(int i);
	int getVertexIndex(int i);
	void setIndicesVN(vector<VerticeNormal*> *array);
	~Cara(void);
};

