//manages all triggers and events in the world

#ifndef TRIGGER_MANAGER
#define TRIGGER_MANAGER

#include "Trigger.h"
#include "Event.h"

class TriggerManager {
 private:
    static Trigger *triggers[5];
    static int	   numTriggers;

	static Event   *events[5];
    static int	   numEvents;

	//id of the current event that is proccessing
    //set to -1 to mean no event is currently proccessing
	static int	   eventId; 
							
    
 public:
	TriggerManager(void);

    void frameMove();
    void render(IDirect3DDevice9* device);
	
 friend class Trigger;
 friend class EndGameEvent;
 friend class StartGameEvent;
 friend class TalkToAliensMission;
};

#endif