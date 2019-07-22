//represents the player's spaceship. Also controls the ships animation

#ifndef SPACE_SHIP
#define SPACE_SHIP

#include "MeshHeirarchy.h"
#include "Frustum.h"
#include "NPCStructs.h"

//player's ship managed by CityManager

class SpaceShip{
  public:
     static D3DXVECTOR3 shipPos;	//for easy outside use

  private:
     D3DXVECTOR3   pos;
     CXfileEntity  *mesh;
     sBoundingBox *bb;	

     //animation control
     static bool startAnim;
     bool animationOver;
     bool triggered;
     double time;

  public:
	 SpaceShip(float x, float y, float z);

     void initDeviceObjects(IDirect3DDevice9 *device);
	 void restoreDeviceObjects(IDirect3DDevice9 *device);
	 void invalidateDeviceObjects();
	 void destroyDeviceObjects();
     
     void frameMove(IDirect3DDevice9 *device);
	 void render(IDirect3DDevice9 *device, cFrustum *frustum);

	 bool isInVeiwingFrustum(cFrustum *frustum);
     D3DXVECTOR3 getPos() { return pos;}
	

   friend class CityManager;
   friend class TalkToAliensMission;
};

#endif