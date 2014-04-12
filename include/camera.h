//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cameraH__
#define __cameraH__

#include <d3d9.h>
#include <d3dx9.h>

const float MAX_ROLL = 45.0f;
const float MIN_ROLL = -45.0f;
const float MAX_PITCH = 45.0f;
const float MIN_PITCH = -45.0f;

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V, int view); 
	void setCameraType(CameraType cameraType); 
	void getPosition(D3DXVECTOR3* pos); 
	void setPosition(D3DXVECTOR3* pos); 

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

  void getLookPort(D3DXVECTOR3* look);
  
  void tiltCam(float angle);
  void panCam(float angle);
  void zoomCam(float angle);
  
  void Update(float timeDiff);

  float getHeadingDegrees();
  float getHeadingRadians();
  float getRollRadians();
  float getRollDegrees();
  float getPitchRadians();
  float getPitchDegrees();
  void reset();
  void reset(float a, float b, float c);

private:
	CameraType  _cameraType;
  D3DXVECTOR3 _pos;
  
  //cockpit view
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;

  //port view
	D3DXVECTOR3 _rightPort;
	D3DXVECTOR3 _upPort;
	D3DXVECTOR3 _lookPort;

  //aft view
	D3DXVECTOR3 _rightAft;
	D3DXVECTOR3 _upAft;
	D3DXVECTOR3 _lookAft;

  //starboard view
  D3DXVECTOR3 _rightStarboard;
	D3DXVECTOR3 _upStarboard;
	D3DXVECTOR3 _lookStarboard;

  //bottom view
	D3DXVECTOR3 _rightBottom;
	D3DXVECTOR3 _upBottom;
	D3DXVECTOR3 _lookBottom;

	D3DXVECTOR3 _rightCam;
	D3DXVECTOR3 _upCam;
	D3DXVECTOR3 _lookCam;
  D3DXVECTOR3 _posCam;
  float _zoom;
  float _pan;
  float _tilt;

  float _yawRate;
  float _rollRate;
  float _pitchRate;
  float _yaw;
  float _roll;
  float _pitch;
};
#endif // __cameraH__