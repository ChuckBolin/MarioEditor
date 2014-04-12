//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//  Update: June 15, 2008     Added extra views. Chuck Bolin
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\include\camera.h"

Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	_rightCam = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_upCam    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_lookCam  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

  _tilt = 0.0f;
  _pan = 0.0f;
  _zoom = 1.0f;
  _yaw = 0.0f;
  _yawRate = 0.0f;
  _roll = 0.0f;
  _rollRate = 0.0f;
}

void Camera::reset(){
	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
  _rightCam = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_upCam    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_lookCam  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

  _tilt = 0.0f;
  _pan = 0.0f;
  _zoom = 1.0f;
  _yaw = 0.0f;
  _yawRate = 0.0f;
  _roll = 0.0f;
  _rollRate = 0.0f;

}

void Camera::reset(float a, float b, float c){
	_pos   = D3DXVECTOR3(a, b, c);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

void Camera::getPosition(D3DXVECTOR3* pos)
{
  *pos = _pos;
}

void Camera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}

void Camera::getLookPort(D3DXVECTOR3* look){
  *look = _lookPort;
}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	//if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	//if( _cameraType == AIRCRAFT )
	//	_pos += _look * units;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	//if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	//if( _cameraType == AIRCRAFT )
	//	_pos += _right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}


void Camera::tiltCam(float angle){
  _tilt += angle;
}

void Camera::panCam(float angle){
  _pan += angle;
}
void Camera::zoomCam(float angle){
  _zoom += angle;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);
/*
  if(getPitchDegrees() > MAX_PITCH){
    D3DXMatrixRotationAxis(&T, &_right,	-angle);
    //return;
  }
  else if(getPitchDegrees() < MIN_PITCH){
    D3DXMatrixRotationAxis(&T, &_right,	-angle);
    //return;
  }
*/
	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::Update(float timeDiff){

  D3DXMATRIX T;
  //yaw
  D3DXMatrixRotationAxis(&T, &_up, _yaw * timeDiff );
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);

  //roll
  //if(_roll > 0.28f)
  //  _roll = 0.28f;
  //if(_roll < -0.28f)
  //  _roll = -0.28f;
  //D3DXVec3Normalize(&_look, &_look);
  //D3DXMatrixRotationAxis(&T, &_look, _roll * timeDiff);
  //D3DXVec3TransformCoord(&_right,&_right, &T);
	//D3DXVec3TransformCoord(&_up,&_up, &T);


}

void Camera::yaw(float angle){
  //_yaw = angle;
  D3DXMATRIX T;
  //yaw
  D3DXMatrixRotationAxis(&T, &_up, angle);// * timeDiff );
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
  /*
  _yawRate += angle;
  _yaw += _yawRate;
  if(_yaw > 0.5)
    _yaw = 0.5f;
  if(_yaw < -0.5f);
    _yaw = -0.5f;
*/
  /*

  D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, _yaw);

	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, _yaw);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
  */
}

void Camera::roll(float angle)
{
  _rollRate += angle;
  _roll += _rollRate;
  if(_roll > 0.28f)
    _roll = 0.28f;
  if(_roll < -0.28f)
    _roll = -0.28f;

	// only roll for aircraft type
	//if( _cameraType == AIRCRAFT )	{
/*
    D3DXVec3Normalize(&_look, &_look);

		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
    */
//	}
}

void Camera::getViewMatrix(D3DXMATRIX* V, int view)
{
  //sets base limits
  if(_pos.y < 0.1f){ //1.0f
    _pos.y = 0.1f;
    pitch(-0.1f);
  }

  //sets ceiling limits
  if(_pos.y > 150.0f){
    _pos.y = 150.0f;
    pitch(0.1f);
  }

	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);
	D3DXVec3Normalize(&_lookCam, &_lookCam);
	D3DXVec3Cross(&_upCam, &_lookCam, &_rightCam);
	D3DXVec3Normalize(&_upCam, &_upCam);
	D3DXVec3Cross(&_rightCam, &_upCam, &_lookCam);
	D3DXVec3Normalize(&_rightCam, &_rightCam);

	// Build the view matrix:
	float x;
	float y;
	float z;

  D3DXMATRIX mat;
  D3DXMatrixIdentity(&mat);

  //cockpit
  if(view == 0){
    x = -D3DXVec3Dot(&_right, &_pos);
	  y = -D3DXVec3Dot(&_up, &_pos);
	  z = -D3DXVec3Dot(&_look, &_pos);

	  (*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

  //port view
  else if(view == 1){
    _lookPort = -_right;
    _rightPort = _look;
    _upPort = _up;
    x = -D3DXVec3Dot(&_rightPort, &_pos);
	  y = -D3DXVec3Dot(&_upPort, &_pos);
	  z = -D3DXVec3Dot(&_lookPort, &_pos);

	  (*V)(0,0) = _rightPort.x; (*V)(0, 1) = _upPort.x; (*V)(0, 2) = _lookPort.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _rightPort.y; (*V)(1, 1) = _upPort.y; (*V)(1, 2) = _lookPort.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _rightPort.z; (*V)(2, 1) = _upPort.z; (*V)(2, 2) = _lookPort.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

  //aft view
  else if(view == 2){
    _lookAft = -_look;
    _rightAft = -_right;
    _upAft= _up;
    x = -D3DXVec3Dot(&_rightAft, &_pos);
	  y = -D3DXVec3Dot(&_upAft, &_pos);
	  z = -D3DXVec3Dot(&_lookAft, &_pos);

	  (*V)(0,0) = _rightAft.x; (*V)(0, 1) = _upAft.x; (*V)(0, 2) = _lookAft.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _rightAft.y; (*V)(1, 1) = _upAft.y; (*V)(1, 2) = _lookAft.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _rightAft.z; (*V)(2, 1) = _upAft.z; (*V)(2, 2) = _lookAft.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

  //starboard view
  else if(view == 3){
    _lookStarboard = _right;
    _rightStarboard = -_look;
    _upStarboard = _up;
    x = -D3DXVec3Dot(&_rightStarboard, &_pos);
	  y = -D3DXVec3Dot(&_upStarboard, &_pos);
	  z = -D3DXVec3Dot(&_lookStarboard, &_pos);

	  (*V)(0,0) = _rightStarboard.x; (*V)(0, 1) = _upStarboard.x; (*V)(0, 2) = _lookStarboard.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _rightStarboard.y; (*V)(1, 1) = _upStarboard.y; (*V)(1, 2) = _lookStarboard.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _rightStarboard.z; (*V)(2, 1) = _upStarboard.z; (*V)(2, 2) = _lookStarboard.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

  //bottom view
  else if(view == 4){
    _lookBottom = -_up;
    _rightBottom = _right;
    _upBottom = _look;
    x = -D3DXVec3Dot(&_rightBottom, &_pos);
	  y = -D3DXVec3Dot(&_upBottom, &_pos);
	  z = -D3DXVec3Dot(&_lookBottom, &_pos);

	  (*V)(0,0) = _rightBottom.x; (*V)(0, 1) = _upBottom.x; (*V)(0, 2) = _lookBottom.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _rightBottom.y; (*V)(1, 1) = _upBottom.y; (*V)(1, 2) = _lookBottom.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _rightBottom.z; (*V)(2, 1) = _upBottom.z; (*V)(2, 2) = _lookBottom.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

  //controllable camera
  else if(view == 5){
    _rightCam.x = 1.0f; _rightCam.y = 0.0f; _rightCam.z = 0.0f;
    _upCam.x = 0.0f; _upCam.y = 1.0f; _upCam.z = 0.0f;
    _lookCam.x = 0.0f;  _lookCam.y = 0.0f; _lookCam.z = 1.0f;

	  D3DXMATRIX T;
    D3DXMatrixRotationX(&T, _tilt);
    D3DXVec3TransformCoord(&_upCam, &_upCam, &T);
    D3DXMatrixRotationY(&T, _pan);
    D3DXVec3TransformCoord(&_rightCam, &_rightCam, &T);

    _posCam = _pos;// + (_lookCam * _zoom);//1.5f);
    _posCam.y -= 1.0f;
    x = _posCam.x;// -D3DXVec3Dot(&_rightCam, &_posCam);
	  y = _posCam.y;//-D3DXVec3Dot(&_upCam, &_posCam);
	  z = _posCam.z;//-D3DXVec3Dot(&_lookCam, &_posCam);

	  (*V)(0,0) = _rightCam.x; (*V)(0, 1) = _upCam.x; (*V)(0, 2) = _lookCam.x; (*V)(0, 3) = 0.0f;
	  (*V)(1,0) = _rightCam.y; (*V)(1, 1) = _upCam.y; (*V)(1, 2) = _lookCam.y; (*V)(1, 3) = 0.0f;
	  (*V)(2,0) = _rightCam.z; (*V)(2, 1) = _upCam.z; (*V)(2, 2) = _lookCam.z; (*V)(2, 3) = 0.0f;
	  (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
  }

}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}

//returns heading in radians
float Camera::getHeadingRadians(){
  return getHeadingDegrees() * D3DX_PI/180.0f;
}

//returns heading in degrees
float Camera::getHeadingDegrees(){
  float hdg = 0;

  if(_look.z > 0 && _look.x > 0 ){ //0 to 90 deg
    hdg = acos(_look.z) * 180.0f/3.14159f;
  }
  if(_look.z < 0 && _look.x > 0 ){ //90 to 180 deg
    hdg = 90 + asin(_look.z) * -180.0f/3.14159f;
  }
  if(_look.z < 0 && _look.x < 0 ){ //180 to 270 deg
    hdg = 360 + acos(_look.z) * -180.0f/3.14159f;
  }
  if(_look.z > 0 && _look.x < 0 ){ //180 to 270 deg
    hdg = 270 + asin(_look.z) * 180.0f/3.14159f;
  }

  return hdg;
}


//returns roll in radians
float Camera::getRollRadians(){
  return getRollDegrees() * D3DX_PI/180.0f;
}

//returns heading in degrees
float Camera::getRollDegrees(){
  float roll = 0;

  if(_right.y > 0 && _right.x > 0)//top-right quad
    roll = acos(_right.x) * -180.0f/D3DX_PI;
  if(_right.y > 0 && _right.x < 0)//top-right quad
    roll = acos(_right.x) * -180.0f/D3DX_PI;

  if(_right.y < 0 && _right.x > 0)//bottom-right quad
    roll = acos(_right.x) * 180.0f/D3DX_PI;
  if(_right.y < 0 && _right.x < 0)//bottom-left quad
    roll = acos(_right.x) * 180.0f/D3DX_PI;

  //else if(_right.y < 0 && _right.x < 0)
  //  roll = 360 + acos(_right.x) * -180.0f/D3DX_PI;
  //else if(_right.y > 0 && _right.x < 0)
  //  roll = 270 + asin(_right.x) * 180.0f/D3DX_PI;
/*
  if(_right.y > 0 && _right.x > 0)//roll right
    roll = acos(_right.x) * 180.0f/D3DX_PI;
  else if(_right.y < 0 && _right.x > 0)
    roll = acos(_right.x) * -180.0f/D3DX_PI;
*/

  return roll;
}

//returns pitch in radians
float Camera::getPitchRadians(){
  return getPitchDegrees() * D3DX_PI/180.0f;
}

//returns heading in degrees
float Camera::getPitchDegrees(){
  float pitch = 0;

  if(_look.z > 0 && _look.y > 0 )//pitch up
    pitch = acos(_look.z) * 180.0f/3.14159f;
  if(_look.z > 0 && _look.y < 0 ) //pitch down
    pitch = acos(_look.z) * -180.0f/3.14159f;
  if(_look.z < 0 && _look.y > 0 )//pitch
    pitch = acos(_look.z) * 180.0f/3.14159f;
  if(_look.z < 0 && _look.y < 0 ) //pitch
    pitch = acos(_look.z) * -180.0f/3.14159f;

  return pitch;
}