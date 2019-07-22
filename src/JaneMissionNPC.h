//subclass of NPC
//represents the Jane character

#include "Npc.h"

class JaneMissionNPC : public Npc{
  private:
    bool isFirstTimeTalkedTo;
    bool isArrivedToHouse;
    
    static bool talkedToTransformedPlayer;
    static bool triggerFollowPlayer;
    
    //for setting custom messages from outside
    static char *str;
    static bool customMsg;
   
  public:
	JaneMissionNPC(EntityType t, D3DXVECTOR3 v);

    void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    void  talk();
	    
	static void setMsg(char *s) { str = s; customMsg = true;}


	friend class TalkToAliensMission;
	
};
