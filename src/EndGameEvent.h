#pragma once
#include "event.h"
#include "CameraTransition.h"

class EndGameEvent : public Event {
  private:
    CameraTransition *ct;
    
  public:
	EndGameEvent(char *en);

	void proccessEvent();
	void render(IDirect3DDevice9 *device);
	
};
