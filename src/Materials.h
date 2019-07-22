// Materials.h: interface for the Materials class.
//
//////////////////////////////////////////////////////////////////////

#include "Newton.h"

#if !defined(AFX_MATERIALS_H__C8152D99_8176_4CE4_BF86_9D241D3B54ED__INCLUDED_)
#define AFX_MATERIALS_H__C8152D99_8176_4CE4_BF86_9D241D3B54ED__INCLUDED_


struct NewtonWorld;

extern int woodID; 
extern int metalID; 
extern int levelID; 
extern int worldInteriorID; 
extern int vehicleID; 

extern int characterID; 


void SetupMaterials (NewtonWorld* nWorld);
void CleanUpMaterials (NewtonWorld* nWorld);

// structure use to hold game play especial effects
struct SpecialEffectStruct
{
	void *soundHandle;
	void *particleHandle;

	// create the resources for this materials effect here
	void Init (char* effectName)
	{
	}

	// destroy all resouces for this material interaction here
	void Detroy ()
	{
	}

	// play the impact sound
	void PlayImpactSound (float volume)
	{
	}

	// play scratch sound
	void PlayScratchSound (float volume)
	{
	}

	NewtonBody* m_body0;
	NewtonBody* m_body1;
	D3DXVECTOR3 m_position;
	float m_contactMaxNormalSpeed;
	float m_contactMaxTangentSpeed;
};

#endif 
