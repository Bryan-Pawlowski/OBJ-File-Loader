#include "OBJ-Loader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;


Model::~Model()
{
	vertices->clear();
	delete[] vertices;
}

int Model::modelInit(char * filename)
{
	vertices = new vector< VERTEX >();
	HRESULT res = OBJParse(filename);
	if (res == S_FALSE)
	{
		cout << "Model could not be created!" << endl;
		return 1;
	}

	return 0;
}

HRESULT Model::OBJParse(char* filename)
{
	ifstream inFile;
	string line, v = "v", vt = "vt", vn = "vn", f = "f";
	unsigned int lineCount = 0;

	vector< D3DXVECTOR4 > vs;
	vector< D3DXVECTOR3 > vns;
	vector< D3DXVECTOR2 > vts;


	inFile.open(filename);

	if (!inFile.is_open()){
		cout << "File " << filename << "could not be opened." << endl;
		return S_FALSE;
	}
	while (getline(inFile, line)) // we get a new line to process, here.
	{
		istringstream streamLine(line);
		string token;

		if (++lineCount == 33){
			lineCount = lineCount;
		}

		streamLine >> token;

		if (token == v){
			D3DXVECTOR4 vert;
			streamLine >> vert.x;
			streamLine >> vert.y;
			streamLine >> vert.z;
			vert.w = 1.0f;
			vs.push_back(vert);
		}
		else if (token == vt) {
			D3DXVECTOR2 coord;
			streamLine >> coord.x;
			streamLine >> coord.y;
			vts.push_back(coord);
		}
		else if (token == vn){
			D3DXVECTOR3	norm;
			streamLine >> norm.x;
			streamLine >> norm.y;
			streamLine >> norm.z;
			vns.push_back(norm);
		}
		else if (token == f)
		{
			int vInd, tInd = 0, nInd = 0;


			while (streamLine.good())
			{
				VERTEX vInfo;

				string chunk;
				string empty = "";  //this empty string is basically to cover the case where
				char dummy;			//there is whitespace after the data.
				streamLine >> chunk;

				if (chunk == empty) continue;

				istringstream streamChunk(chunk);

				streamChunk >> vInd;
				vInfo.vert = vs[vInd - 1];

				streamChunk >> dummy;
				if (vts.size() != 0){
					streamChunk >> tInd;
					vInfo.texCoords = vts[tInd - 1];
				}

				streamChunk >> dummy;
				if (vns.size() != 0){
					streamChunk >> nInd;
					vInfo.norm = vns[nInd - 1];
				}


				vertices->push_back(vInfo);

			}
		}

		
	}



	inFile.close();

	vs.clear();
	vts.clear();
	vns.clear();

	return S_OK;
}

unsigned int Model::getSize()
{
	return vertices->size();
}