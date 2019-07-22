//subclass of NPC
//represents the first inspector character

#include "Npc.h"

class InspectorMissionNPC : public Npc{
  private:
	static bool isInvolvedInMission;
	static bool talkedToPlayer;

    IDirect3DTexture9 *texture;
  public:
	InspectorMissionNPC(EntityType t, D3DXVECTOR3 pos);

	void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    void  talk();	
	float getDistanceFromPlayer();

	void initDeviceObjects(IDirect3DDevice9* device);
    void invalidateDeviceObjects();


   friend class FindInspectorMission;
};
