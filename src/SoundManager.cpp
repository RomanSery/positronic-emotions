#include "dxstdafx.h"
#include ".\soundmanager.h"

//define in global scope
CSoundManager* SoundManager::soundManager;
CSound* 	   SoundManager::soundBlocking;    
CSound* 	   SoundManager::soundConversation; 
HWND		   SoundManager::h;
bool		   SoundManager::isPlaying;
bool		   SoundManager::proceesNPCSounds;
bool		   SoundManager::isPlayingConversation;
float		   SoundManager::time;
LPWSTR		   SoundManager::npcBlockedSounds[NUM_BLOCKED_SOUNDS];
LPWSTR		   SoundManager::npcConversationSounds[NUM_CONVERSATION_SOUNDS];


SoundManager::SoundManager(void)
{
  soundManager = new CSoundManager();
  soundManager->Initialize( h, DSSCL_PRIORITY );
  soundManager->SetPrimaryBufferFormat(2, 22050, 16);
  
  time = 0.0f;
  
  npcBlockedSounds[0] = L"sounds/xm-sob.wav";
  npcBlockedSounds[1] = L"sounds/greeting.wav"; 

  npcConversationSounds[0] = L"sounds/driving.wav";
  
}

void SoundManager::init(){}

LPWSTR SoundManager::getRandomConversationSound()
{
 static int index = -1;
 if(index != NUM_CONVERSATION_SOUNDS - 1)
  index++;
 else
  index = 0;
 return npcConversationSounds[index];

}
LPWSTR SoundManager::getRandomBlockedSound()
{
 static int index = -1;
 if(index != NUM_BLOCKED_SOUNDS - 1)
  index++;
 else
  index = 0;
 return npcBlockedSounds[index];
}

void SoundManager::frameMove()
{
  isPlaying = false;  

  if(soundBlocking != NULL){
    isPlaying = soundBlocking->IsSoundPlaying();
         
  }
  if(isPlaying || proceesNPCSounds == false)
    time = 0.0f;
  
  if(soundConversation != NULL && !soundConversation->IsSoundPlaying())
   isPlayingConversation = false;
}

void SoundManager::playNPCConversation()
{
  if(isPlayingConversation == false){
      if(soundBlocking != NULL &&soundBlocking->IsSoundPlaying())
        soundBlocking->Stop();
      soundManager->Create( &soundConversation, getRandomConversationSound(), 0, GUID_NULL );
      soundConversation->Play( 0, 0);
      isPlayingConversation = true;
  }
  
}


void SoundManager::playNPCSound()
{
    if(isPlaying == false && (time >= NPCSOUND_WAIT_TIME || time == 0.0f)){
      
      soundManager->Create( &soundBlocking, getRandomBlockedSound(), 0, GUID_NULL );
      soundBlocking->Play( 0, 0);
      isPlaying = true;
	  time = 0.0f;
    }
    else
     time += DXUTGetElapsedTime();    
	
}