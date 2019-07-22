#include "dxstdafx.h"
#include ".\trigger.h"
#include "Player.h"
#include "TriggerManager.h"

Trigger::Trigger(D3DXVECTOR3 pt, float r, bool an, int e)
{
  triggerPt = pt;  
  eventToTrigger = e;
  radius = r;  

  alwaysOn = an;
  alwaysOn ? isOn = true : isOn = false;
  isTriggered = false;
}

bool Trigger::testTriggered()
{
  D3DXVECTOR3 diff = triggerPt.operator -(Player::sPlayerInfo->vecPos);

  if(D3DXVec3Length(&diff) <= radius){
    isTriggered = true;
    TriggerManager::eventId = eventToTrigger;
    return true;
  }

  return false;
}
