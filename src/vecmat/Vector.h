/*** ***/

/* $Id: Vector.h,v 1.2 2000/02/09 22:15:07 vinod Exp $ */

#ifndef _VECTOR_HH_

#define _VECTOR_HH_

// Common stuff for all the static Vector classes
// Forward declarations for all the classes is included here, along with
// common include files and prototypes for functions that use different kinds
// of Vectors

// Common include files
#include "../include/BaseObject.h"
#include "../include/StreamIO.h"
#include "../include/Constants.h"
#include "../include/Inlines.h"

#include <cstdlib>
#include <cmath>

// Forward declarations
class Vector2d;
class Vector3d;
class Vector4d;

// Function prototypes

   // Cross products for 2d and 3d Vectors
Vector3d operator % (const Vector2d& vec1, const Vector2d& vec2);
Vector3d operator % (const Vector3d& vec1, const Vector2d& vec2);
Vector3d operator % (const Vector2d& vec1, const Vector3d& vec2);

#endif /* #ifndef _VECTOR_HH_ */


/*
  $Log: Vector.h,v $
  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:23  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:45  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:35  vinod
  Vectors using statically allocated arrays

*/
