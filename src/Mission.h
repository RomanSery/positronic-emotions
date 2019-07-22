//supreclass for all missions
//defines methods that are overriden by missions,
//and handels reading in the text from a file

#ifndef MISSION
#define MISSION

class Mission {
  protected:
    LPWSTR  missionName;
    int	    missionID;
    
    bool	isMissionStarted;
    bool	isMissionSuccess;
    bool	isMissionFailure;

    FILE    *inFile;
    char*   text[10];
    
	void  readInMissionText();
  public:
	Mission(LPWSTR name, int id);

    virtual void  isMissionTriggered();
    virtual void  isMissionSuccessful();
    virtual void  isMissionFailed();
        
    virtual void  frameMove(IDirect3DDevice9 *device);
    virtual void  render(IDirect3DDevice9 *device);
    virtual void  proccessMission();
  	
        
};

#endif