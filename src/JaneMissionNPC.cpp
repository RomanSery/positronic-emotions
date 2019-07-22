#include "dxstdafx.h"
#include ".\janemissionnpc.h"
#include "PrintText.h"
#include "Player.h"
#include "SpaceShip.h"
#include "MissionManager.h"

bool JaneMissionNPC::talkedToTransformedPlayer = false;
bool JaneMissionNPC::triggerFollowPlayer = false;
bool JaneMissionNPC::customMsg = false;
char* JaneMissionNPC::str;

JaneMissionNPC::JaneMissionNPC(EntityType t, D3DXVECTOR3 vec)
 : Npc(t, 0, STATIONARY)
{
  isFirstTimeTalkedTo = true;
  isArrivedToHouse = false;
  pos = vec;
  
  
  //setup destination node, where Jane goes to after you talk to her the first time
  addNode(pos.x, pos.y, pos.z);
  addNode(0, 0, -200);
  finishAddingNodes();
  setLoopNodes(false);
  setRepressBlockingSound(true);

  Npc::readInNpcText();

  //////////////////REMOVE LATER- for starting from Mission4////////////
  /*MissionManager::proccessMissions = true;
  isFirstTimeTalkedTo = false;
  isArrivedToHouse = true;
  Player::sPlayerInfo->isHuman = true;*/
  //////////////////////////////////////////
}


void JaneMissionNPC::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos)
{
   static bool b = false;   

   if(isFirstTimeTalkedTo == false){
	 //tell Jane to walk to house after you finish talking
	 if(PrintText::msgOver && b == false){ 
       state = WALKING;
       b = true;
     }
	 //check if Jane arrived at house
     if(reachedEnd() && isArrivedToHouse == false){
       isArrivedToHouse = true;
       state = STATIONARY;  
     } 
   }

   if( triggerFollowPlayer ){
     setFollowDistance(20.0f);
     state = FOLLOW;
   }

  
   Npc::FrameMove(entity, m_fElapsedTime, m_pd3dDevice, playerPos);
}


void JaneMissionNPC::talk()
{
    static bool b = false;

    if(customMsg == false){  
		//first time talked to
		if(isFirstTimeTalkedTo){
			PrintText::showMsg(text[0]);
			isFirstTimeTalkedTo = false;
		}
	    
		//Arrived at house, but player didnt transform yet
		if( isArrivedToHouse && Player::sPlayerInfo->isHuman == false && b == false){
			PrintText::showMsg(text[1]); 
			///////////////// trigger transform mission ///////////
			MissionManager::proccessMissions = true;            
            b = true;
		}

		//Arrived at house and player is human
		if( isArrivedToHouse && Player::sPlayerInfo->isHuman){
		   PrintText::showMsg(text[2]);
		   talkedToTransformedPlayer = true;
		}
    }
    else{ //handle custom msg set by a mission
		PrintText::showMsg(str);
        //customMsg = false;
    }

}


void JaneMissionNPC::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  Npc::render(device, entity);
  //if(customMsg) PrintText::printString("true", 30, 30);
  //else PrintText::printString("false", 30, 30);
}