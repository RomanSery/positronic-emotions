//a utility class that provides functions for printing string and numbers to the screen
//The main purpose is to display the dialogue when the player talks to a character

#ifndef PRINT_TEXT
#define PRINT_TEXT

#include "d3dfont.h"


class PrintText{
  private:
    //variables for keeping track of messages
    static int         currToken;
    static int         numTokens;
    static char*       getToken(int n);
    static int		   calcNumTokens();
    
    static bool        proccessingMsg;
    static char        *currMsg;
    static bool		   playerHitAdvance;
	
    
    //message queue variables
    static char* msgQueue[5];
    static int   numMsgWaiting;

	static ID3DXSprite *sprite;
	static IDirect3DTexture9 *texture;

        
    static void  showToken();

  public:
    //UI and font objects      
    static CD3DFont *font;  
    static CDXUTDialog  *ui;
    
	static bool	msgOver;
	static bool showPic;
    static int  x, y;

  public:
	PrintText(CD3DFont *f, CDXUTDialog *d);

    static void	printNum(float,int,int);
    static void	printInt(int,int,int);
    static void	printString(char*,int,int);

    static void showMsg(char *msg);
    static void showPicture(IDirect3DTexture9 *tex, int x = 0, int y = 0);    

    static void onResetDevice(const D3DSURFACE_DESC* surfaceDesc);
    static void render(float time);
    static void frameMove(float time);

    void initDeviceObjects(IDirect3DDevice9* device);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
      
	
};

#endif
