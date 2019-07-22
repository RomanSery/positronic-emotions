#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"


/*
	Our 3rd person camera has three main functions which setup what's it is looking at.  The
	first rotateY(), rotates the look at vector (the camera's local +Z axis) around the world's
	Y axis, which always points in "kWorldUp" (0,1,0) direction.  The next function pitch() 
	rotates the camera's view around the camera's local X axis.  Lastly the setTarget() function
	builds the camera's axes based on the camera's eye position and what we want the camera to
	look at (it's target).
	
*/

const float kCamDist = 60.0f; // Default amount for the eye to be away from the camera's target
const CVector kWorldUp(0,1,0); // Direction of up (+Y axis) in world space


const static float kTurnAmt = 5.0f; // Amount to turn camera by (mouse input)
const static float kPitchAmt = 15.0f; 	

// Width and height of the window (specifically the client rect)
const static int kWinWid = 1024;
const static int kWinHgt = 768;

	
// The camera
class CCamera
{
	public:
		CCamera();
		
		void rotateY(float angle, const CPos &target); // Rotates around world Y-axis
		void pitch(float angle, const CPos &target); // Rotates around the camera's X-axis
						
		CPos    getTarget() const { return eye + (forward * kCamDist); } 
		CPos    getEye() const { return eye; }
		CVector getCamForward()  { return forward; } // Returns camera's forward vector
		CVector getCamUp() const { return up; } // Returns camera's up vector
		CVector getCamRight() const { return right; } // Returns camera's right vector

		static  void  setEye(const CPos &eyePos) { eye = eyePos; } // Sets the eye position of the camera
		static  void  setTarget(const CPos &target); // Sets the camera to look at "target"
		
		
	private:
		
		static CVector right; // The local normalized axis that points to the "right"
		static CVector up; // The local normalized axis that points "up"
		static CVector forward; // The local normalized axis that points "forward"
		
		static CPos eye; // Where we are looking from

   friend class CameraTransition;
};

#endif