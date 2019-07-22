//functions for rendering the player either as a robot or human

#include "MeshHeirarchy.h"

void renderPlayerRobotForm(ID3DXEffect *effect, D3DXMESHCONTAINER_DERIVED *pMeshContainer,
							D3DXFRAME_DERIVED *pFrame, IDirect3DCubeTexture9 *cubeTexture,
							D3DXHANDLE technique);

void renderPlayerHumanForm(ID3DXEffect *effect, D3DXMESHCONTAINER_DERIVED *pMeshContainer,
							D3DXFRAME_DERIVED *pFrame);