#include "Event.h"

class StartGameEvent : public Event{
  public:
    static bool isFinished;
  private:
    IDirect3DTexture9  *tex;

  public:
	StartGameEvent(char *en);
	
    void proccessEvent();
	void render(IDirect3DDevice9 *device);
};
