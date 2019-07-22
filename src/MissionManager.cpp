#include "dxstdafx.h"
#include ".\missionmanager.h"
#include "FixVisionMission.h"
#include "FindInspectorMission.h"
#include "TransformMission.h"
#include "TalkToAliensMission.h"
#include "PrintText.h"
#include "Player.h"

int MissionManager::currMission;
int MissionManager::numPrimaryMissions;
bool MissionManager::proccessMissions = true;


MissionManager::MissionManager(void)
{
  primaryMissions[0] = new FixVisionMission(L"Fix Vision", 1);
  primaryMissions[1] = new FindInspectorMission(L"Find Inspector", 2);
  primaryMissions[2] = new TransformMission(L"Transform to human", 3);
  primaryMissions[3] = new TalkToAliensMission(L"Talk to Aliens", 4);
  numPrimaryMissions = 4;
  currMission = 0;

  /////REMOVE////
  //proccessMissions = false;
}


void MissionManager::frameMove(IDirect3DDevice9 *device)
{
  if(currMission < numPrimaryMissions && proccessMissions)
    primaryMissions[currMission]->frameMove(device);  

  
}


void MissionManager::render(IDirect3DDevice9 *device)
{
  //if(currMission < numPrimaryMissions && proccessMissions)  
    //primaryMissions[currMission]->render(device);

  //if(Player::sPlayerInfo->isOnMission) PrintText::printString("on mission", 500, 500);
  //else PrintText::printString("", 500, 500);
  PrintText::printString("a", 500, 1000);
}

