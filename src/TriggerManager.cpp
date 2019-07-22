#include "dxstdafx.h"
#include ".\triggermanager.h"
#include "EndGameEvent.h"
#include "StartGameEvent.h"

int		 TriggerManager::numEvents;
int		 TriggerManager::numTriggers;
int		 TriggerManager::eventId;
Trigger* TriggerManager::triggers[5];
Event*   TriggerManager::events[5];

TriggerManager::TriggerManager(void)
{
  events[0] = new EndGameEvent("End game event");
  events[1] = new StartGameEvent("Start game event");
  numEvents = 1;  

  triggers[0] = new Trigger(D3DXVECTOR3(-445, 0, 357), 33.0f, false, 0); //end game trigger  
  triggers[1] = new Trigger(D3DXVECTOR3(0, 0, 0), 500.0f, true, 1); //start game trigger  
  numTriggers = 2;

  eventId = -1;  
}

void TriggerManager::frameMove()
{
   for(int i = 0; i < numTriggers; i++){
     if(triggers[i]->isOn == false || triggers[i]->isTriggered)
       continue;
     
     triggers[i]->testTriggered();     
   }

   
   if(eventId != -1)
     events[eventId]->proccessEvent();

   
}

void TriggerManager::render(IDirect3DDevice9* device)
{
  /*ID3DXMesh *s;
  for(int i = 0; i < numTriggers; i++){
    if(triggers[i]->isOn == false || triggers[i]->isTriggered)
      continue;

    D3DXCreateSphere(device, triggers[i]->radius, 10, 10, &s, NULL);
    
    D3DXMATRIX mWorld;
    D3DXMatrixTranslation(&mWorld, triggers[i]->triggerPt.x, triggers[i]->triggerPt.y, triggers[i]->triggerPt.z);
	device->SetTransform(D3DTS_WORLD, &mWorld);
    s->DrawSubset(0);

    s->Release();
  }*/
  
  if(eventId != -1)
     events[eventId]->render(device);

}

