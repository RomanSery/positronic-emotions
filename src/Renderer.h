//calls render methods of all the things that need to be rendered.
//also renders the special effects

#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#include "NodeTree.h"
#include "Lights.h"
#include "Player.h"
#include "CubeTexture.h"
#include "RenderToTexture.h"
#include "FilterBlit.h"
#include "NPCManager.h"
#include "CityManager.h"


class Renderer {
  public:
    static bool SUPPORTS_SHADERS_1_1;

  private:
	//things to render
	CDXUTMesh			*playerShadow;
	CDXUTMesh			*ground;
	CDXUTMesh			*house;
	CDXUTMesh			*car;
    Lights				*lights;
    NPCManager			*npcs;
	CityManager		    *city;	
 		
	//texture classes
    RenderToTexture		*renderToTex;
    CubeTexture			*cubeTexture;
    IDirect3DCubeTexture9 *cubeTex;

	//special effect rendering
    FilterBlit			*filter;
	ID3DXEffect         *effect;
    ID3DXEffect         *effectNormalMap;
	
	//viewing and device vars
	IDirect3DDevice9    *pd3dDevice;
	Player				*p;
	D3DXMATRIX			matWorld;
    	
    //private rendering functions
	void renderGlow();
    void renderSceneElements(bool renderingToCube);
    void renderToCubeMap();

    cNodeTreeMesh       *nodeTreeGround;
	cNodeTreeMesh       *nodeTreeBuildings;		
	
  public:
	Renderer(Player *p);
	
	void initDeviceObjects(IDirect3DDevice9* device);
	void restoreDeviceObjects();
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
    void frameMove(float fElapsedTime);
	
	void render();
			
		
};

#endif