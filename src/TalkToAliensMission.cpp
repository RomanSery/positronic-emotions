#include "dxstdafx.h"
#include ".\talktoaliensmission.h"
#include "Player.h"
#include "PrintText.h"
#include "JaneMissionNPC.h"
#include "SpaceShip.h"
#include "MissionManager.h"
#include "TriggerManager.h"

TalkToAliensMission::TalkToAliensMission(LPWSTR name, int id)
 : Mission(name, id)
{
  time = 0;
  isJaneFollowing = false;
  arrivedToShip = false;

  Mission::readInMissionText();

}

void TalkToAliensMission::isMissionTriggered()
{
  if(time > 5){
	PrintText::showMsg(text[0]);
	PrintText::showMsg(text[1]);

    Player::sPlayerInfo->isOnMission = true;
    isMissionStarted = true;
  }
}

void TalkToAliensMission::isMissionSuccessful()
{
   SpaceShip::startAnim = true;
   static bool b = false;
   if(b == false){
     JaneMissionNPC::setMsg(text[4]);
     b = true;
   }
   
   Player::sPlayerInfo->isOnMission = false;
   MissionManager::proccessMissions = false;
   isMissionSuccess = true;

}

void TalkToAliensMission::proccessMission()
{
  //tell Jane to follow player after player talks to her
  if(JaneMissionNPC::talkedToTransformedPlayer && isJaneFollowing == false){
	JaneMissionNPC::triggerFollowPlayer = true;
    isJaneFollowing = true;
  }
  
  //if Jane already following check if you've arrived to the ship
  if(isJaneFollowing){	
	D3DXVECTOR3 playerPos = Player::sPlayerInfo->vecPos;
	D3DXVECTOR3 diff = playerPos.operator -(SpaceShip::shipPos);
   
	if( D3DXVec3Length(&diff) < 80.0f ){
	  PrintText::showMsg(text[2]);
      PrintText::showMsg(text[3]);
      TriggerManager::triggers[0]->isOn = true;
	  arrivedToShip = true;
	}
  }

}

void TalkToAliensMission::frameMove(IDirect3DDevice9 *device)
{
   if( isMissionStarted == false)  
     isMissionTriggered();
   else if(arrivedToShip == false)
     proccessMission();
   else if(PrintText::msgOver)
     isMissionSuccessful();
   

   time = time + DXUTGetElapsedTime();
}

void TalkToAliensMission::render(IDirect3DDevice9 *device)
{
  //PrintText::printNum((float)time, 10, 10);

}


