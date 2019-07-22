#include "dxstdafx.h"
#include ".\transformmission.h"
#include "StoreClerkMissionNPC.h"
#include "PrintText.h"
#include "Player.h"
#include "MissionManager.h"

TransformMission::TransformMission(LPWSTR name, int id)
: Mission(name, id)
{
  time = 0;
  triggeredEffect = effectOver = false;

  Mission::readInMissionText();
}

void TransformMission::isMissionTriggered()
{
  if(time > 5){
	PrintText::showMsg(text[0]);
	
    //initiliaze the clerk's text and mission variables
	StoreClerkMissionNPC::setMsg(text[1]);
	StoreClerkMissionNPC::isInvolvedInMission = true;
    StoreClerkMissionNPC::talkedToPlayer = false;
	
    Player::sPlayerInfo->isOnMission = true;
    isMissionStarted = true;
  }

}

void TransformMission::isMissionSuccessful()
{
  if(StoreClerkMissionNPC::talkedToPlayer && PrintText::msgOver){
     Player::sPlayerInfo->isOnMission = false;
     Player::setInputControls(true, true);
     StoreClerkMissionNPC::isInvolvedInMission = false;
  
     isMissionSuccess = true;
     MissionManager::currMission++;
  }
}


void TransformMission::frameMove(IDirect3DDevice9 *device)
{
  static bool b = false;  

  if( isMissionStarted == false)  
    isMissionTriggered();
  else{
    if(StoreClerkMissionNPC::talkedToPlayer && triggeredEffect == false && effectOver == false){
      if(b == false){ time = 0; b = true; }
      if(PrintText::msgOver)
	    triggerSpecialEffect();
    }
  }  
  if( triggeredEffect )
    proccessSpecialEffect();

  if(effectOver)
    isMissionSuccessful();


  if(PrintText::msgOver)
    time = time + DXUTGetElapsedTime();

}

//determine when to end the transforming effect
void TransformMission::proccessSpecialEffect()
{
   if(time > 5){
     Player::transformToHuman();
     CXfileEntity::technique = "renderHuman";
     effectOver = true;
	 triggeredEffect = false;
     
     //dont let player leave store until he talks to clerk after transforming
     StoreClerkMissionNPC::talkedToPlayer = false;
     StoreClerkMissionNPC::setMsg(text[2]);
     Player::setInputControls(true, false);
   }

}

//called once to start the transforming effect
void TransformMission::triggerSpecialEffect()
{
  CXfileEntity::technique = "transform";
  //dont let player move while transforming
  Player::setInputControls(false, false);
  time = 0;
  triggeredEffect = true;
}

void TransformMission::render(IDirect3DDevice9 *device)
{
  //PrintText::printNum((float)time, 10, 10);

}

