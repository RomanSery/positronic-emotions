#include "dxstdafx.h"
#include ".\FixVisionMission.h"
#include "PrintText.h"
#include "SpaceShip.h"
#include "Player.h"
#include "MissionManager.h"
#include "StoreClerkMissionNPC.h"
#include "StartGameEvent.h"


FixVisionMission::FixVisionMission(LPWSTR name, int id)
: Mission(name, id)
{
  time = 0;
  
  Mission::readInMissionText();
}

void FixVisionMission::isMissionFailed()
{}

void FixVisionMission::isMissionSuccessful()
{
   D3DXVECTOR3 playerPos = Player::sPlayerInfo->vecPos;
   D3DXVECTOR3 diff = playerPos.operator -(SpaceShip::shipPos);
   
   if( D3DXVec3Length(&diff) < 100.0f ){
     //show mission success dialogue
     PrintText::showMsg(text[4]);

	 //set mission variables and switch to next mission
     StoreClerkMissionNPC::isInvolvedInMission = false;
     StoreClerkMissionNPC::talkedToPlayer = false;
     Player::sPlayerInfo->isOnMission = false;
     isMissionSuccess = true;
     MissionManager::currMission++;
   }

}

void FixVisionMission::isMissionTriggered()
{
  if(time > 4 && StartGameEvent::isFinished){
   PrintText::showMsg(text[0]);
   
   //initialize mission NPC status and dialogue
   StoreClerkMissionNPC::setMsg(text[1]);
   StoreClerkMissionNPC::isInvolvedInMission = true;
   
   //init mission variables
   Player::sPlayerInfo->isOnMission = true;
   isMissionStarted = true;
  }
  
}

void FixVisionMission::proccessMission()
{
  static bool varsSet = false;
  if(StoreClerkMissionNPC::talkedToPlayer && varsSet == false){
     
      StoreClerkMissionNPC::setMsg(text[2]);
	  
      PrintText::showMsg(text[3]);
	  
      varsSet = true;
  }
}
void FixVisionMission::frameMove(IDirect3DDevice9 *device)
{
  if( isMissionStarted == false)  
    isMissionTriggered();
  else{
    if(StoreClerkMissionNPC::talkedToPlayer)
      isMissionSuccessful();
     
    proccessMission();
  }


  time = time + DXUTGetElapsedTime();


  
}


void FixVisionMission::render(IDirect3DDevice9 *device)
{
  //PrintText::printNum((float)time, 10, 10);  
    

}