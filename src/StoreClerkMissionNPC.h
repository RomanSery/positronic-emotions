//subclass of NPC
//represents the store clerk character

#include "Npc.h"

class StoreClerkMissionNPC : public Npc{
  private:
    static bool talkedToPlayer;
    static bool isInvolvedInMission;
    static char *str;
    
  public:
	StoreClerkMissionNPC(EntityType t, float x, float y, float z, int m, int i = 0, Status s = STATIONARY);

    void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    void  talk();
    
    static void setMsg(char *s) { str = s; } 


	friend class FixVisionMission;
    friend class TransformMission;
};
