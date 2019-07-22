#ifndef CAR_FUNCS
#define CAR_FUNCS


//utility functions for rendering the car body and wheels
void renderCarWithShader(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						  D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX proj,
						  D3DXVECTOR3 camPos, IDirect3DCubeTexture9 *cTex);
void renderWheelsWithShader(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						  D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX proj,
						  D3DXVECTOR3 camPos);
void renderWithoutShader(IDirect3DDevice9* device, CDXUTMesh *mesh, D3DXMATRIX mWorld);


#endif