//subclass of NPC
//represents the guard next to the first inspector character

#include "Npc.h"

class WitnessMissionNPC : public Npc{
  private:
    static bool isInvolvedInMission;
	static bool talkedToPlayer;
    static bool gotToEatingPos;

    bool startTimer;
    double time;
    
	
	void  updateAnimStatus(CXfileEntity *entity);
  public:
	WitnessMissionNPC(EntityType t, D3DXVECTOR3 pos);

	void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    void  talk();	
	

    friend class InspectorMissionNPC;
    friend class FindInspectorMission;
};
