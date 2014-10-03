#include "OBJ-Loader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int Model::modelInit(char * filename)
{
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
	unsigned int vc = 0, vtc = 0, vnc = 0, fc = 0;

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
		size_t location = 0;
		size_t origLoc = location;
		istringstream streamLine(line);
		string token;

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

	}


	inFile.close();

	return S_OK;
}