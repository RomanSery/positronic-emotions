//subclass of Mission
//the last mission, interacts with spaceship, jane mission npc, and end game event

#include "Mission.h"

class TalkToAliensMission : public Mission{
  private:
	double time;
    bool   isJaneFollowing;
    bool   arrivedToShip;


  public:
	TalkToAliensMission(LPWSTR name, int id);
	
    void  isMissionTriggered();
    void  isMissionSuccessful();
    
    void  frameMove(IDirect3DDevice9 *device);
    void  render(IDirect3DDevice9 *device);

    void  proccessMission();
	
};
