#include "dxstdafx.h"
#include ".\printtext.h"
#include <stdio.h>
#include <mbstring.h>
#include "Player.h"


CD3DFont* PrintText::font;
CDXUTDialog* PrintText::ui;
bool PrintText::playerHitAdvance;
int PrintText::numTokens = 0;
int PrintText::currToken = 0;
int PrintText::numMsgWaiting = 0;
bool PrintText::proccessingMsg = false;
char* PrintText::currMsg;
char* PrintText::msgQueue[5];
ID3DXSprite* PrintText::sprite;
bool PrintText::showPic;
int PrintText::x;
int PrintText::y;
bool PrintText::msgOver = true;
IDirect3DTexture9* PrintText::texture;


PrintText::PrintText(CD3DFont *f, CDXUTDialog *d)
{
 font = f;
 ui = d;

 ui->SetFont(1, L"Times New Roman", 24, 50); //define a font for displaying the messages
 ui->AddStatic(1, L"", 30, 600, 800, 200);   //add a static msg label to display text
 
 //customize the static label
 ui->GetStatic(1)->GetElement(0)->SetFont(1);
 ui->GetStatic(1)->GetElement(0)->dwTextFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;
 ui->GetStatic(1)->SetTextColor(D3DCOLOR_ARGB(255, 0, 255, 0));
 
 
 proccessingMsg = false; 
}

char* PrintText::getToken(int n)
{
  char *token = "";
  char tempMsg[1000];
  strcpy(tempMsg, currMsg);
  int index = 0;
    
  token = strtok(tempMsg, "%");
  while(token != NULL && index < n){
    index++;
    token = strtok(NULL, "%");
  }
  if(token == NULL)
   return "null";
  else
   return token;
}

int PrintText::calcNumTokens()
{
  int count = 1;
  for(unsigned int i = 0; i < strlen(currMsg); i++){
    if(currMsg[i] == '%')
      count++;
  }

  return count;
}

void PrintText::showMsg(char *m)
{
  //proccess new message if not already proccessing
  if(proccessingMsg == false){
    currMsg = m;
    proccessingMsg = true;
	msgOver = false;
    currToken = 0;
    numTokens = calcNumTokens();
    Player::sPlayerInfo->isInDialogue = true;
    showToken();
  }
  else {
   msgQueue[numMsgWaiting] = m;
   numMsgWaiting++;
  }
 
  
}

void PrintText::showToken()
{
  CDXUTStatic *box = ui->GetStatic(1);
  
  if(currToken <= numTokens-1){
    wchar_t *outputString = (wchar_t *)malloc(2000);
    box->SetVisible(true);
    mbstowcs(outputString, getToken(currToken), 2000);
    box->SetText(outputString);
  }
  else{ //finished proccessing current msg
    box->SetText(L"");
    box->SetVisible(false); 
    proccessingMsg = false;
	msgOver = true;
    currToken = 0;
    Player::sPlayerInfo->isInDialogue = false;
    
    // switch to next msg on queue if it exists
	if(numMsgWaiting > 0){
     showMsg(msgQueue[numMsgWaiting - 1]);
     numMsgWaiting--;     
    }
	
  }
   
  
}

void PrintText::frameMove(float time)
{
  static bool k = false;
  if(Player::sPlayerInfo->isInDialogue){
	if(PlayerControl::isMouseDown == true && k == true)
		k = true;
	if(PlayerControl::isMouseDown && k == false){
        currToken++;
        showToken();
        k = true;
	}
	if(PlayerControl::isMouseDown == false && k == true)
		k = false;
  }
    
}

void PrintText::render(float time)
{
 ui->OnRender(time);
 //printInt(currToken, 50, 50);
 //printInt(numMsgWaiting, 200, 10);
 //if(proccessingMsg) printString("proccessing", 300, 10);
 //if( msgOver ) printString("msgOver = true", 300, 30);
 //else printString("msgOver = false", 300, 30);

 if(showPic){
   sprite->Begin(D3DXSPRITE_ALPHABLEND);
   sprite->Draw(texture, NULL, NULL, &D3DXVECTOR3(x,y, 0), 0xFFFFFFFF);
  sprite->End();
 }
 
}

void PrintText::onResetDevice(const D3DSURFACE_DESC* surfaceDesc)
{
  ui->SetLocation(0, 0);
  ui->SetSize( surfaceDesc->Width, surfaceDesc->Height );   
  sprite->OnResetDevice();
  if(texture != NULL)texture->Release();
}

void PrintText::showPicture(IDirect3DTexture9 *tex, int xx, int yy)
{
  texture = tex;
  showPic = true;
  x = xx;
  y = yy;
 
}

void PrintText::printNum(float n, int x, int y)
{
  char ch[100];
  sprintf(ch,"%.3f", n);
  
  font->DrawText((float)x,(float)y,D3DCOLOR_ARGB(255,255,0,0), ch);
}
void PrintText::printInt(int n, int x, int y)
{
  char ch[100];
  sprintf(ch,"%u", n);
  
  font->DrawText((float)x,(float)y,D3DCOLOR_ARGB(255,255,255,255), ch);
}
void PrintText::printString(char *text, int x, int y)
{
  
  font->DrawText((float)x,(float)y,D3DCOLOR_ARGB(255,255,0,0), text);
}

void PrintText::initDeviceObjects(IDirect3DDevice9* device)
{
  D3DXCreateSprite(device, &sprite);
}

void PrintText::invalidateDeviceObjects()
{
  sprite->OnLostDevice();
  sprite->Release();
}

void PrintText::destroyDeviceObjects()
{

}




