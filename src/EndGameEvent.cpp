#include "dxstdafx.h"
#include ".\endgameevent.h"
#include "PrintText.h"
#include "Player.h"
#include "TriggerManager.h"
#include "NPCManager.h"

EndGameEvent::EndGameEvent(char *en)
: Event(en)
{
  ct = new CameraTransition();
  ct->addTransitionPoint(D3DXVECTOR3(-481, 41, 184), D3DXVECTOR3(-394,18,68), 5);
  ct->addTransitionPoint(D3DXVECTOR3(-488, 38, -413), D3DXVECTOR3(-529, 20, -544), 5);
  ct->addTransitionPoint(D3DXVECTOR3(54,48,-417), D3DXVECTOR3(160, 19, -495), 5);
  ct->addTransitionPoint(D3DXVECTOR3(362, 29, 77), D3DXVECTOR3(338, 19, -2), 5);
  
}

//when finished proccessing set TirggerManager::eventId = -1
void EndGameEvent::proccessEvent()
{
  static bool init = false;
  static bool showedMsg = false;
  
  if(init == false){
    Player::setInputControls(false, false, false);
    NPCManager::occCulling = false;
    ct->addTransitionPoint(D3DXVECTOR3(-427, 98, 131), Player::sPlayerInfo->vecPos, 5);
    init = true;
  }
  
  bool isFinished = ct->frameMove();
  if(isFinished && showedMsg == false){
   
   PrintText::showMsg("JANE: Honey, we need some more money to finance our operations.%YOU: You're "
					  "right. That bank is still open, I'm sure we can convince them to part with some money.%"
					  "JANE: Oh I'm sure, with the right persuasion. Let's go.%THE END...%FOR NOW...%THANKS FOR PLAYING");

   showedMsg = true;      
  }
  
  if(showedMsg && PrintText::msgOver)
    exit(0);    

}

void EndGameEvent::render(IDirect3DDevice9 *device)
{
  PrintText::printString(eventName, 10, 10);
  

}


