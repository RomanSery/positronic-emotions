//does post proccessing effects such as blur and luminance by rendering
//a screen aligned polygon

#ifndef FILTERBLIT_H
#define FILTERBLIT_H

#include "RenderToTexture.h"

#define kMaxNumPasses 1
class FilterBlit {
  private: 

    typedef struct tagQuadVertex
    {
        D3DXVECTOR3 Position;
        D3DXVECTOR2 Tex;
    } QuadVertex;
	
	// data members
    LPD3DXEFFECT			 m_pEffect;
    LPDIRECT3DVERTEXBUFFER9  m_pVertexBuffer;
	RenderToTexture			 *renderToTex;
	
	//special effect variables
	bool	isOn;
    char	*technique;
	int		intensity;
		  
  public:  
    FilterBlit();
	
	HRESULT InvalidateDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice); // called just before device is Reset
    HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);    // called when device is restored
    HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice, IDirect3DTexture9 *tex);
	HRESULT RenderGlow(LPDIRECT3DDEVICE9 pd3dDevice, IDirect3DTexture9 *tex, char *tech, int i = 6);
    	
	void	initDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	void    CreateAndWriteUVOffsets(int width, int height);

	//special effect controls
	void	toggle() { isOn = !isOn; }
	void	setSpecialEffect(char *e, int i = 1) { technique = e; intensity = i; }    
};

#endif