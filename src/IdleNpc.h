//subclass of NPC
//Used by NPCs that are idle with one animation,
// and have the option of always facing the player

#ifndef IDLENPC_CLASS
#define IDLENPC_CLASS

#include "Npc.h"

class IdleNpc : Npc {
 private:
   bool  facePlayer;   
   bool  isAnimStarted;
   float rotation;


   //animation functions
   void  calcAngle(D3DXVECTOR3 playerPos);

 public:
	IdleNpc(EntityType t, bool fp, float x, float y, float z, int i = 0, float rot = 0);

	//application called functions
	void FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void render(IDirect3DDevice9 *device, CXfileEntity *entity);
      
};

#endif