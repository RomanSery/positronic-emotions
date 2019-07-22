//manages the rendering of the city buildings and the ground
//renders special effects such as the reflective and glowing windows,
//and normal mapped lighting for the buildings
//handels enter/exit requests by player

#ifndef CITY_MANAGER
#define CITY_MANAGER

#include "Frustum.h"
#include "Interior.h"
#include "SpaceShip.h"
#include "TriggerManager.h"

class CityManager {
  public:
	static bool       playerRequestedEnter;
    static bool		  isInteriorBeingRendered[3];
    static Interior   *interiors[3];    

  private: 
    CDXUTMesh    *blocks[7];
	D3DXVECTOR3  positions[7];//[NUM_BLOCKS];
	D3DXVECTOR3  sizes[7];//[NUM_BLOCKS];
	LPCWSTR      fileNames[7];//[NUM_BLOCKS];
	int			 numBlocks;
    long		 numFaces;
    int		     numInteriors;

    SpaceShip	 *ship;
    CDXUTMesh	 *streetLights;    
    TriggerManager *triggerManager;
    
    void		 processEnterRequest();
    
 public:
	CityManager(void);

	//application called functions
	void InitDeviceObjects(IDirect3DDevice9 *device);
	void RestoreDeviceObjects(IDirect3DDevice9 *device);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();

	void frameMove(float m_fElapsedTime, IDirect3DDevice9 *device);
	void render(IDirect3DDevice9 *device, cFrustum *frustum, ID3DXEffect *effect,
				D3DXVECTOR3 lightPos);

    CDXUTMesh* getBlocks() { return blocks[0]; }
    
	bool isBlockDrawn[7];    
	
};

#endif