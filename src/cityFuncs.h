//functions for rendering the city buildings, windows, and glow lines

void renderWindows(IDirect3DDevice9* device, CDXUTMesh *mesh);
void renderGlowLines(IDirect3DDevice9* device, CDXUTMesh *mesh);
void renderLitWindows(IDirect3DDevice9* device, CDXUTMesh *mesh);
void renderUnLitWindows(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						IDirect3DCubeTexture9 *cubeTex);

void renderMeshWithBump(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						D3DXVECTOR3 lightPos);

void renderStreetLights(IDirect3DDevice9* device, CDXUTMesh *mesh);
void renderLightHalo(IDirect3DDevice9* device, CDXUTMesh *mesh, IDirect3DTexture9 *tex);

void renderWithNoBump(IDirect3DDevice9* device, CDXUTMesh *mesh);