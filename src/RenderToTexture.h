//Encapsulates functionality for rendering to a render target
//Can generate copies of the front buffer and display it.

#ifndef RENDER_TO_TEXTURE
#define RENDER_TO_TEXTURE

class RenderToTexture
{
  public:
  //render to texture variables
  IDirect3DTexture9   *renderTexture;
  IDirect3DTexture9   *generatedTexture;	//copy of front buffer
  IDirect3DSurface9   *renderSurface, *originalSurface;
  IDirect3DSurface9   *destSurface;  

  ID3DXSprite		  *sprite;

public:
	RenderToTexture(void);
	void initDeviceObjects(IDirect3DDevice9* device);
	void restoreDeviceObjects(IDirect3DDevice9* device);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
	
	void startRendering(IDirect3DDevice9* device, bool clear = true);
	void finishRendering(IDirect3DDevice9* device);
	void renderSprite(IDirect3DDevice9* device);
	void renderSprite(IDirect3DDevice9* device, IDirect3DTexture9 *tex);
	void generateTexture(IDirect3DDevice9* device);
    void clear(IDirect3DDevice9 *device);
	IDirect3DTexture9* getRenderTexture() { return renderTexture; }
	IDirect3DTexture9* getGeneratedTexture() { return generatedTexture; }
	
	
};
#endif