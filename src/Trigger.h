//a trigger with a world position and radius and a cooresponding index
//of the event to trigger

#ifndef TRIGGER_CLASS
#define TRIGGER_CLASS

#include "Event.h"

class Trigger {
  private:
    D3DXVECTOR3 triggerPt;
    float		radius;

    bool		isOn;            //is trigger enabled
    bool		isTriggered;     //has the trigger been triggered
    bool		alwaysOn;		 //if true trigger is enabled from the start
    int		    eventToTrigger; //Event to start when trigger is triggered

  public:
	Trigger(D3DXVECTOR3 pt, float r, bool alwaysOn, int e);
    bool testTriggered();

  friend class TriggerManager;
  friend class TalkToAliensMission;
	
};

#endif