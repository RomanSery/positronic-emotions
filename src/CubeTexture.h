//Encapsulates functionality for rendering to a cube texture

#pragma once

class CubeTexture
{
  IDirect3DCubeTexture9 *cubeTexture;
  IDirect3DSurface9		*cubeMapFace;
  IDirect3DSurface9		*origBackBuffer;
  IDirect3DSurface9		*origDepthStencil;
 
  D3DXMATRIX			matProj;
 

public:
	CubeTexture();

	void startRendering(IDirect3DDevice9* device);
	void buildCubeMap(IDirect3DDevice9* device, D3DXVECTOR3 eyePos, DWORD i);
	void initDeviceObjects(IDirect3DDevice9* device);
	void finishRendering(IDirect3DDevice9* device, D3DXVECTOR3 eyeP, D3DXVECTOR3 lookAt);

	void restoreDeviceObjects(IDirect3DDevice9* device);
 	void destroyDeviceObjects();
    void invalidateDeviceObjects();
	IDirect3DCubeTexture9* getCubeTexture();
	
};
