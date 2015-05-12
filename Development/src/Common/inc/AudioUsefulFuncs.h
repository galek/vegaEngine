/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once


// various useful defines
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795 // I've defined my own Pi because Microsoft's PI is not always defined properly
#endif
#define ONEDEGINRAD 0.0174532925;

//! get the user's home directory on Linux
void getPathToHome(char* homeDir);

//! strip a string out of another string from an index to a max size, including whitespace
void stripString(char* instr, int from, char* outstr, int max);

//! get angle between two points (full 0-2pi angles)
float getAngleTo(const float& subjectX, const float& subjectZ, const float& objectX, const float& objectZ);

//! work out how much a subject needs to turn to face an object (one sided 0-PI angles)
float getChangeInHeadingTo(const float& subjectX, const float& subjectZ, const float& currentHeading, const float& objectX, const float& objectZ);

//! convert speed in meters per microsecond to sped in kilometers per hour
float mpusTokph(float mpus); // TODO const reference, replace with macro?

//! remove any characters from a string that aren't allowed in a file name
void legaliseFilename(const char* input, char* output);

//! put a radian in the 0-2 pi range
void normaliseRadian(float& radian); // TODO const reference

//! get the squared distance between two 2D positions
float sqDistanceBetween(const float& xi, const float& zi, const float& xf, const float& zf);

//! get the squared distance between two 3D positions
float sqDistanceBetween(const float& xi, const float& yi, const float& zi, const float& xf, const float& yf, const float& zf);

//! get real (non squared distance) between two 2D positions
float real2DDistanceBetween(const float& xi, const float& zi, const float& xf, const float& zf);

//! get real (non squared distance) between two 3D positions
float realDistanceBetween(const float& xi, const float& yi, const float& zi, const float& xf, const float& yf, const float& zf);

//! get angle in radians between 2 2D points
float getAngleBetween_rad(const float& xi, const float& zi, const float& xf, const float& zf);

//! rotate a 2D point around another point
void rotateAroundAPoint2D(float& x, float& z, const float& originX, const float& originZ, const float& theta);

//! convert an Tait-Bryant pitch/yaw/roll orientation into an x,y,z unit vector
void convertTaitBryanToUnitVector(const float& pitch_rad, const float& yaw_rad, const float& roll_rad, float& x, float& y, float& z);