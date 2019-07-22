//manages all the NPCs in the world
//handels talk request from the player, and culling for the NPCs

#include "Npc.h"
#include "IdleNpc.h"
#include "SuicidalNpc.h"
#include "Frustum.h"

class NPCManager {
 public:
   static bool playerRequestedTalk; 
   static CXfileEntity	*entities[20];
   static bool occCulling;

 private:
    int d; bool inRange;
	int   numNPCs, numInteriorNPCs;
    int   numEntities;

	
    Npc             *characters[15];      
    Npc				*interiorCharacters[6];
    
    void processTalkRequest(D3DXVECTOR3 playerPos);
    void processInteriorTalkRequest();    
 public:
	NPCManager(void);

	//application called functions
	void InitDeviceObjects(IDirect3DDevice9 *device);
	void RestoreDeviceObjects(IDirect3DDevice9 *device);
	void invalidateDeviceObjects() {};
	void destroyDeviceObjects();
	void FrameMove(float m_fElapsedTime, IDirect3DDevice9 *device);
	void render(IDirect3DDevice9 *device, cFrustum *frustum);

	int numDrawn;
		
};
