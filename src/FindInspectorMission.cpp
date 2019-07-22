#include "dxstdafx.h"
#include ".\findinspectormission.h"
#include "PrintText.h"
#include "Player.h"
#include "InspectorMissionNPC.h"
#include "WitnessMissionNPC.h"
#include "SpaceShip.h"
#include "MissionManager.h"

FindInspectorMission::FindInspectorMission(LPWSTR name, int id)
: Mission(name, id)
{
  time = 0;
  finalLeg = false;

  Mission::readInMissionText();
}

void FindInspectorMission::isMissionTriggered()
{
  if(time > 5)
  {
	PrintText::showMsg(text[0]);
	
    InspectorMissionNPC::isInvolvedInMission = true;
    WitnessMissionNPC::isInvolvedInMission = true;
	Player::sPlayerInfo->isOnMission = true;
    isMissionStarted = true;
  }
}

void FindInspectorMission::isMissionSuccessful()
{
   D3DXVECTOR3 playerPos = Player::sPlayerInfo->vecPos;
   D3DXVECTOR3 diff = playerPos.operator -(SpaceShip::shipPos);
   
   if( D3DXVec3Length(&diff) < 100.0f ){
     PrintText::showMsg(text[2]);

	 //set mission variables and switch to next mission
     InspectorMissionNPC::isInvolvedInMission = false;
	 Player::sPlayerInfo->isOnMission = false;
     isMissionSuccess = true;
     MissionManager::currMission++;
     MissionManager::proccessMissions = false;     
   }


}



void FindInspectorMission::proccessMission()
{
  static bool b = false;
  static bool d = true;

  //show msg 10 sec after you talk to inspector
  if(b == false){
       time = 0;
	   b = true;
  }
  if(b && d && time > 5){
      PrintText::showMsg(text[1]);
      finalLeg = true;
      d = false;
  }


}

void  FindInspectorMission::frameMove(IDirect3DDevice9 *device)
{
  if( isMissionStarted == false)  
    isMissionTriggered();
  else if(InspectorMissionNPC::talkedToPlayer && finalLeg == false)
    proccessMission();
  else if(finalLeg)
    isMissionSuccessful();
  
  if(PrintText::msgOver)
    time = time + DXUTGetElapsedTime();

}

void  FindInspectorMission::render(IDirect3DDevice9 *device)
{
  //PrintText::printNum((float)time, 10, 10);
  //if(finalLeg)PrintText::printString("final leg",300,300);
  

}

