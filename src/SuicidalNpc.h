//sublcass of NPC
//used by NPCs that have a triggered sucide animation


#ifndef SuicidalNPC_CLASS
#define SuicidalNPC_CLASS

#include "Npc.h"
#include "player.h"

class SuicidalNpc : Npc {
 private:
   float rotation;
   D3DXVECTOR3 triggerPoint;
   bool triggered;
   bool animationOver;
   bool isAnimStarted;
   float time;
   
 public:
	SuicidalNpc(EntityType t, D3DXVECTOR3 tPt, D3DXVECTOR3 initial);

	//overridded functions
	void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    void  talk();
    float getDistanceFromPlayer();
    BOOL isInVeiwingFrustum(cFrustum* frustum);
    
     
};

#endif