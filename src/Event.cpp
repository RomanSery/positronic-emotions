#include "dxstdafx.h"
#include ".\event.h"

Event::Event(char *en)
{
  eventName = en;
}

void Event::proccessEvent(){}
void Event::render(IDirect3DDevice9 *device){}
