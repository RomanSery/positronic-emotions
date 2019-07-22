#pragma once

class Event{
 protected: 
    char *eventName;

 public:
	Event(char *en);

	virtual void proccessEvent();
	virtual void render(IDirect3DDevice9 *device);
};
