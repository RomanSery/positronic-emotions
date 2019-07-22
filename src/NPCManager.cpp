#include "dxstdafx.h"
#include ".\npcmanager.h"
#include "PrintText.h"
#include "Player.h"
#include "CityManager.h"
#include "JaneMissionNPC.h"
#include "StoreClerkMissionNPC.h"
#include "InspectorMissionNPC.h"
#include "WitnessMissionNPC.h"

bool          NPCManager::playerRequestedTalk;
bool		  NPCManager::occCulling = true;
CXfileEntity* NPCManager::entities[20];


NPCManager::NPCManager(void)
{
  characters[0] = new SuicidalNpc(DOCTOR, D3DXVECTOR3(547, 0, 79), D3DXVECTOR3(568, 260, 74) );
  characters[1] = new SuicidalNpc(VINCENT_SUICIDE, D3DXVECTOR3(-667, 0, -10), D3DXVECTOR3(-739, 260, -11) );
  characters[2] = new IdleNpc(ROBBERY, false, -394, 2.5, 68);
  characters[3] = new IdleNpc(BAT_ATTACK, false, -529, 2.5, -544);
  characters[4] = new IdleNpc(SWORD_FIGHT, false, 160, 2.5, -495);
  characters[5] = new IdleNpc(BEAT_UP, false, 338, 2.5, -2);
  characters[6] = new IdleNpc(COP, true, -235, 2.5, 84);
  characters[7] = new IdleNpc(COP, true, 412, 2.5, -454, 1);

  characters[8] = new Npc(VINCENT);  //8
  characters[8]->addNode(24, 0, -207);
  characters[8]->addNode(24, 0, 69);
  characters[8]->addNode(358, 0, 69);
  characters[8]->addNode(358, 0, -210);
  characters[8]->finishAddingNodes();
  
  characters[9] = new Npc(ANITA);  
  characters[9]->addNode(-678, 0, -441);
  characters[9]->addNode(-220, 0, -440);
  characters[9]->addNode(-220, 0, -209);
  characters[9]->addNode(-678, 0, -208);
  characters[9]->finishAddingNodes();

  characters[10] = new Npc(ANITA, 1);  
  characters[10]->addNode(538, 0, -440);
  characters[10]->addNode(538, 0, 303);
  characters[10]->addNode(33, 0, 303);
  characters[10]->addNode(352, 0, 69);
  characters[10]->finishAddingNodes();

  characters[11] = new JaneMissionNPC(JANE, D3DXVECTOR3(0, 0, 0));
  characters[12] = new InspectorMissionNPC(INSPECTOR, D3DXVECTOR3(-821, 0, 193));
  characters[13] = new WitnessMissionNPC(WITNESS, D3DXVECTOR3(-780, 0, 163));
    

  numNPCs = 14;

  //////////////// interior NPCs /////////////////////////////
  //gun store
  interiorCharacters[0] = new IdleNpc(COP, true, 244, 0, -187);
  CityManager::interiors[0]->addNpc(interiorCharacters[0]);
  //hardware store
  interiorCharacters[1] = new StoreClerkMissionNPC(CLERK, 226, 0, -36, -1);
  CityManager::interiors[1]->addNpc(interiorCharacters[1]);
 
  numInteriorNPCs = 2;
  numEntities = 0;
  
}


void NPCManager::FrameMove(float m_fElapsedTime, IDirect3DDevice9 *device)
{
  for(int i = 0; i < numNPCs; i++)
    characters[i]->FrameMove(entities[characters[i]->getEntityType() + characters[i]->getIndex()],
						     m_fElapsedTime, device, Player::sPlayerInfo->vecPos);
  
  if(NPCManager::playerRequestedTalk && Player::sPlayerInfo->isInDialogue == false)
   Player::sPlayerInfo->isInInterior ? processInteriorTalkRequest() : processTalkRequest(Player::sPlayerInfo->vecPos);
  
}

void NPCManager::processInteriorTalkRequest()
{
  
  CityManager::interiors[Player::sPlayerInfo->interiorID]->processTalkRequest();

}
void NPCManager::processTalkRequest(D3DXVECTOR3 playerPos)
{
  float min = characters[0]->getDistanceFromPlayer();
  int index = 0;  //index of NPC player is talking to
 
  for(int i = 1; i < numNPCs; i++)
  {
     float distance = characters[i]->getDistanceFromPlayer();
     if(min > distance){
       min = distance;
	   index = i;
     } 
  }
  d = index;

  if(min < 20){
    characters[index]->talk();
    SoundManager::proceesNPCSounds = true;
  }
  else{
    SoundManager::proceesNPCSounds = false;
  }
}


void NPCManager::render(IDirect3DDevice9 *device, cFrustum *frustum)
{
    numDrawn = 0;
	
	for(int i = 0; i < numNPCs; i++){
		if( characters[i]->isInVeiwingFrustum(frustum) == TRUE ){
	      characters[i]->render(device, entities[characters[i]->getEntityType() + characters[i]->getIndex()]);
		  numDrawn++;
		}
	}
    
    //PrintText::printString("num drawn ", 10, 10);
	//PrintText::printInt(numDrawn, 100, 10); 
      
}


void NPCManager::InitDeviceObjects(IDirect3DDevice9 *device)
{
  const int numFiles = 14;
  sFileName files[numFiles] = {
    {L"models/doctorSuicide.x", 1},		
    {L"models/robbery.x",	1},	
    {L"models/batAttack.x",	1},	
    {L"models/swordFight.x", 1},
    {L"models/beatup.x", 1},		
    {L"models/vincentSuicide.x", 1},		
    {L"models/clerk.x", 1},		
    {L"models/inspector.x", 1},
    {L"models/witness.x", 1},
    {L"models/jane.x", 1},
    {L"models/vincent.x", 1},		
    {L"models/anita.x", 3},		
    {L"models/guard.x", 1},		
    {L"models/cop.x", 2},
  };
    
    
  for(int i = 0; i < numFiles; i++)
    numEntities  += files[i].numEntities;
  
  
  for(int i = 0; i < numEntities; i++)
    entities[i] = new CXfileEntity();
  

  int counter = 0;
  for(int j = 0; j < numFiles; j++){
    for(int x = 0; x < files[j].numEntities; x++){
      entities[counter]->LoadXFile(device, files[j].fileName);		
      counter++;
    }
  }

    
  for(int i = 0; i < numEntities; i++)
   entities[i]->initDeviceObjects(device);

  for(int i = 0; i < numNPCs; i++)
   characters[i]->initDeviceObjects(device);
  
}

void NPCManager::RestoreDeviceObjects(IDirect3DDevice9 *device)
{
  for(int i = 0; i < numEntities; i++)
    entities[i]->restoreDeviceObjects(device);

  
}

void NPCManager::destroyDeviceObjects()
{
  for(int i = 0; i < numEntities; i++)  
    entities[i]->DeleteDeviceObjects();
  
  for(int i = 0; i < numNPCs; i++)
   characters[i]->invalidateDeviceObjects();
}

