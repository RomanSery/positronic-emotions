#include "dxstdafx.h"
#include ".\witnessmissionnpc.h"
#include "PrintText.h"

#define IDLE_ID 1
#define WALK_ID 2
#define EAT_ID 3

bool WitnessMissionNPC::gotToEatingPos;
bool WitnessMissionNPC::talkedToPlayer;
bool WitnessMissionNPC::isInvolvedInMission = false;

WitnessMissionNPC::WitnessMissionNPC(EntityType t, D3DXVECTOR3 p)
: Npc(t, 0, STATIONARY)
{
  pos = p;
  talkedToPlayer = startTimer = gotToEatingPos = false;
  time = 0;

  //setup destination node, where cop goes to eat donut
  addNode(p.x, p.y, p.z);
  addNode(0, 0, 150);
  finishAddingNodes();
  setLoopNodes(false);
 
  Npc::readInNpcText();    
}

void WitnessMissionNPC::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos)
{
  updateAnimStatus(entity);
  if(startTimer){
    time = time + DXUTGetElapsedTime();
    if(reachedEnd()){
      gotToEatingPos = true;
      startTimer = false;
    }
  }


  Npc::FrameMove(entity, m_fElapsedTime, m_pd3dDevice, playerPos);
}

void WitnessMissionNPC::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  Npc::render(device, entity);
}

void WitnessMissionNPC::talk()
{
  static bool b = true;  

  if(talkedToPlayer == false && isInvolvedInMission == false)
    PrintText::showMsg(text[0]);
  else if(talkedToPlayer == false && isInvolvedInMission == true){
    PrintText::showMsg(text[1]);
    b = false;
  }
  else if(gotToEatingPos)
    PrintText::showMsg(text[2]);

  if(b == false && isInvolvedInMission == true){
    talkedToPlayer = true;
    startTimer = true;
    b = true;
  }
}

void WitnessMissionNPC::updateAnimStatus(CXfileEntity *entity)
{
  static bool b = false;

  if(talkedToPlayer == false){
    if(entity->getCurrAnim() != IDLE_ID)
      entity->SetAnimationSet(IDLE_ID);
  }
  else if(PrintText::msgOver && gotToEatingPos == false){ //walking to eating position
    if(entity->getCurrAnim() != WALK_ID && state != BLOCKED)
      entity->SetAnimationSet(WALK_ID);
    if(b == false){
      state = WALKING;
      b = true;
    }
    
  }
  else if(gotToEatingPos){ //reached eating donut position
    if(entity->getCurrAnim() != EAT_ID)
      entity->SetAnimationSet(EAT_ID);
  }  

}
