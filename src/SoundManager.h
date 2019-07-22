//utility functions for playing sounds
//handels playing conversation sounds when player talks to an NPC,
//and blocking sounds when player is blocking an NPC


#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include "dxstdafx.h"
#include "DXUTsound.h"
#include "SoundStructs.h"

class SoundManager {
 private:
   static LPWSTR getRandomBlockedSound();
   static LPWSTR getRandomConversationSound();

   static LPWSTR npcBlockedSounds[NUM_BLOCKED_SOUNDS];
   static LPWSTR npcConversationSounds[NUM_CONVERSATION_SOUNDS];
 
 public:
   static CSoundManager *soundManager;
   static CSound        *soundBlocking;    //blocking sounds object 
   static CSound        *soundConversation;//conversation sounds object       

   static bool			isPlaying;         //is playing a blocking sound
   static bool          proceesNPCSounds;  //process blocking sounds?
   static bool			isPlayingConversation; //is playing conversation sound?
   static float			time;              //time elapsed since last blocking sound played



   static HWND		    h; //Window handle
 public:
	SoundManager(void);

    void init();
    void frameMove();

    static void playNPCSound();
    static void playNPCConversation();
    static bool isPlayingSound() { return isPlaying; }
    
};



#endif