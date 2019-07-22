//subclass of Mission
//The first mission to fix your blurred vision, interacts with spaceship and
//store clerk mission NPC

#ifndef FIX_VISION
#define FIX_VISION

#include "Mission.h"


class FixVisionMission : public Mission{
  private:
    double time;
        
  public:
	FixVisionMission(LPWSTR name, int id);

    void  isMissionTriggered();
    void  isMissionSuccessful();
    void  isMissionFailed();

    void  frameMove(IDirect3DDevice9 *device);
    void  render(IDirect3DDevice9 *device);
    void  proccessMission();
    	
};

#endif