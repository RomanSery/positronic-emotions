#include "dxstdafx.h"
#include ".\startgameevent.h"
#include "PrintText.h"
#include "PlayerControl.h"
#include "TriggerManager.h"

bool	StartGameEvent::isFinished = false;

StartGameEvent::StartGameEvent(char *en)
 : Event(en)
{
}


void StartGameEvent::proccessEvent()
{
  static bool init = false;

  if(init == false){
    D3DXCreateTextureFromFile(DXUTGetD3DDevice(), L"media/intro.jpg", &tex);
    init = true;
  }
  
  PrintText::showPicture(tex, 0, -45);

  if(PlayerControl::isMouseDown){
    PrintText::showPic = false;
    tex->Release();
    TriggerManager::eventId = -1;
    isFinished = true;
  }  


}


void StartGameEvent::render(IDirect3DDevice9 *device)
{

  //PrintText::printString(eventName, 10, 10);

}
