//sublcass of Mission
//The second mission to find the other inspector, interacts with witness NPC and
//Inspector mission npc

#ifndef FIND_INSPECTOR
#define FIND_INSPECTOR

#include "Mission.h"

class FindInspectorMission : public Mission{
  private:
    double time;
	bool finalLeg;    

    
  public:
	FindInspectorMission(LPWSTR name, int id);
	
    void  isMissionTriggered();
    void  isMissionSuccessful();
    
    void  frameMove(IDirect3DDevice9 *device);
    void  render(IDirect3DDevice9 *device);
    void  proccessMission();

	
};

#endif