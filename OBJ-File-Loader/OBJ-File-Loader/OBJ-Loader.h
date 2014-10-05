#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <vector>

struct VERTEX
{
	D3DXVECTOR4 vert;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 texCoords;
};


class Model {

	public:
		std::vector< VERTEX > *vertices;
		Model(){};
		~Model();
		int modelInit(char * filename);

	private:
		HRESULT OBJParse(char * filename);
};