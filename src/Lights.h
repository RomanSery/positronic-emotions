//Defines and renders all the lights in the world

//struct to represent a single light
struct sLight{
 D3DXVECTOR3 pos;
 bool  isOn;
 
 //default constructer
 sLight(){}

 sLight(D3DXVECTOR3 p, bool o = true){
  pos.x = p.x;
  pos.y = p.y;
  pos.z = p.z;
  isOn = o;
 }
};


class Lights
{
  CDXUTMesh			  *light;
  IDirect3DTexture9   *texture;

  sLight		  streetLights[50];
  sLight		  smallLights[50];
  
  int			numBigLights;
  int			numSmallLights;


public:
  Lights(void);

  void	restoreDeviceObjects(IDirect3DDevice9* device);
  void  initDeviceObjects(IDirect3DDevice9* device);
  void  renderLights(IDirect3DDevice9* device);
  void  frameMove();
  void  destroyDeviceObjects();
  void  invalidateDeviceObjects();

  D3DXVECTOR3 getClosestLight(D3DXVECTOR3 pos);
  D3DXVECTOR3 getLight(int i) { return streetLights[i].pos; }

	
};
