//manages all the missions
//handels detecting when mission are finished and
//assigning the next mission

#include "Mission.h"

class MissionManager{
  public:
    static int numPrimaryMissions;
    static int currMission;  //for easy access from missions
	static bool proccessMissions;    
    
  private:
    Mission  *primaryMissions[5];
    
    
  public:
	MissionManager(void);

    void  frameMove(IDirect3DDevice9 *device);
    void  render(IDirect3DDevice9 *device);
	
};
