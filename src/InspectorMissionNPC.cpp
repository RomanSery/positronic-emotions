#include "dxstdafx.h"
#include ".\inspectormissionnpc.h"
#include "PrintText.h"
#include "Player.h"
#include "WitnessMissionNPC.h"

bool InspectorMissionNPC::isInvolvedInMission;
bool InspectorMissionNPC::talkedToPlayer;

InspectorMissionNPC::InspectorMissionNPC(EntityType t, D3DXVECTOR3 p)
 : Npc(t, 0, STATIONARY)
{
  pos = p;

  Npc::readInNpcText();
}

void InspectorMissionNPC::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos)
{
  D3DXMATRIX matTrans;
  D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
  entity->FrameMove(m_fElapsedTime,matTrans);

  if(PrintText::showPic)
  {
	if(getDistanceFromPlayer() > 20){
     PrintText::showPic = false;
    }

  }
        
}

float InspectorMissionNPC::getDistanceFromPlayer()
{
  D3DXVECTOR3 diff = Player::sPlayerInfo->vecPos.operator -(pos);
  return D3DXVec3Length(&diff);

}

void InspectorMissionNPC::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  
  Npc::render(device, entity);
}


void InspectorMissionNPC::talk()
{
  if(isInvolvedInMission){
    if(WitnessMissionNPC::talkedToPlayer){
      PrintText::showPicture(texture);
      talkedToPlayer = true;
    }
    else
      PrintText::showMsg(text[1]);
    
  }
  else
    PrintText::showMsg(text[0]);
}

void InspectorMissionNPC::initDeviceObjects(IDirect3DDevice9* device)
{
  D3DXCreateTextureFromFile(device, L"media/inspector.jpg", &texture);
}

void InspectorMissionNPC::invalidateDeviceObjects()
{
  texture->Release();

}