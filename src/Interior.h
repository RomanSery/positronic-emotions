//represents an interior in the city
//handels defining it's collision geometry, defining entering and exiting points
//and defines all NPCs that inhabit this interior

#include "Npc.h"
#include "IdleNpc.h"
#include "Newton/newton.h"

class Interior {
  private:
    D3DXVECTOR3   entryPoint;
    char          *name;
    int			  interiorID;
    LPCWSTR		  meshFile, collisionFile;
    bool		  isInRangeOfPlayer;
    bool		  isInhabitedByPlayer;
    
    CDXUTMesh     *mesh;
    CDXUTMesh     *collisionMesh;
    NewtonBody    *levelBody;

    Npc			  *characters[3];
    int			  numNpcs;
  public:
	Interior(int id, D3DXVECTOR3 entry, char* n, LPCWSTR m, LPCWSTR c);

    //general application called functions
	void InitDeviceObjects(IDirect3DDevice9 *device);
	void RestoreDeviceObjects(IDirect3DDevice9 *device);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
	void frameMove(float m_fElapsedTime, IDirect3DDevice9 *device);
	bool render(IDirect3DDevice9 *device);

    //specific to interior functions
    void  readInCollisionGeometry();
    float getDistanceFromPlayer();
    void  processEnterRequestByPlayer();
    void  addNpc(Npc *npc);
    void  processTalkRequest();
    
    friend class NPCManager;

};
