//a collection of camera positions and corresponding look at points to define a "cut scene"
//the camera goes through each point stoping for the specified time

#ifndef CAMERA_TRANSITION
#define CAMERA_TRANSITION


struct sTransitionPoint {
  D3DXVECTOR3 cameraPos;
  D3DXVECTOR3 lookAtPt;
  float	      length;
  
  sTransitionPoint(D3DXVECTOR3 eye, D3DXVECTOR3 at, float l){
   cameraPos = eye;
   lookAtPt = at;
   length = l;

  }
};

class CameraTransition {
  private:
    sTransitionPoint *transitionPoints[10];
    int			     numPoints;
	int			     currPoint;
    float		     time;
    bool			 isFinished;
    

    void switchToNextTransitionPoint();
  public:
	CameraTransition(void);
	
    void addTransitionPoint(D3DXVECTOR3 eye, D3DXVECTOR3 at, float l);
    bool frameMove();
	
};

#endif