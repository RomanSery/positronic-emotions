#include "dxstdafx.h"
#include ".\storeclerkmissionnpc.h"
#include "PrintText.h"
#include "FixVisionMission.h"

bool StoreClerkMissionNPC::talkedToPlayer = false;
bool StoreClerkMissionNPC::isInvolvedInMission = false;
char* StoreClerkMissionNPC::str;

StoreClerkMissionNPC::StoreClerkMissionNPC(EntityType t, float x, float y, float z, int m, int i, Status s)
: Npc(t, i, s)
{
  pos = D3DXVECTOR3(x, y, z);
  
  Npc::readInNpcText();
}


void StoreClerkMissionNPC::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos)
{


  Npc::FrameMove(entity, m_fElapsedTime, m_pd3dDevice, playerPos);
}

void StoreClerkMissionNPC::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  //if(talkedToPlayer) PrintText::printString("talked to player", 10, 100);
  Npc::render(device, entity);
}

void StoreClerkMissionNPC::talk()
{
  if(isInvolvedInMission){
    PrintText::showMsg(str);
    talkedToPlayer = true;
  }
  else{
    PrintText::showMsg(text[0]);
  }

}
