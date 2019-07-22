#include "dxstdafx.h"
#include ".\mission.h"

Mission::Mission(LPWSTR name, int id)
{
  missionName = name;
  missionID = id;
  isMissionStarted = isMissionSuccess = isMissionFailure = false;
 
}

//Format for mission text: each block of dialogue ends with a /n
void Mission::readInMissionText()
{
   inFile = fopen("text/missions.txt", "r");

   //scan down the file until you get to the start of your mission
   char missionStr[500] = "id"; //needs newline char
   char *temp = (char *)malloc(500);
   itoa(missionID, temp, 10);
   strcat(missionStr, temp);
   strcat(missionStr, "\n");
   char s[500];
   while( strcmp(missionStr, s) != 0 )
     fgets(s, 10, inFile);
   //read in how many lines of dialogue there are for the mission
   int nTexts = 0;
   fscanf(inFile, "%d ", &nTexts);
   

   //allocate memory for each individual dialogue
   for(int i = 0; i < nTexts; i++)
     text[i] = (char *)malloc(1000);
   
   //read in succesive dialogues for the mission
   for(int i = 0; i < nTexts; i++)
     fgets(text[i], 1000, inFile);
   
   
   fclose(inFile);
}

void Mission::isMissionFailed(){}
void Mission::isMissionSuccessful(){}
void Mission::isMissionTriggered(){}
void Mission::frameMove(IDirect3DDevice9 *device){}
void Mission::render(IDirect3DDevice9 *device){}
void Mission::proccessMission(){}
