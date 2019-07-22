//subclass of Mission
//The third mission, interacts with Jane Mission NPC, and storeclerk mission NPC

#ifndef TRANSFORM_MISSION
#define TRANSFORM_MISSION


#include "Mission.h"

class TransformMission : public Mission{
  private:
	double time;
    bool   triggeredEffect;
    bool   effectOver;

	void  triggerSpecialEffect();
    void  proccessSpecialEffect();
  public:
	TransformMission(LPWSTR name, int id);

	void  isMissionTriggered();
    void  isMissionSuccessful();
    
    void  frameMove(IDirect3DDevice9 *device);
    void  render(IDirect3DDevice9 *device);
	
};

#endif