/*** ***/

/* $Id: Vector2d.cc,v 1.2 2000/02/09 22:15:07 vinod Exp $ */

// Definitions for non-inline functions in Vector2d.h

#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"

void Vector2d :: copyFrom(const Vector3d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1];
}

void Vector2d :: copyFrom(const Vector4d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1];
}

/*
  $Log: Vector2d.cc,v $
  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:23  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:43  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:34  vinod
  Vectors using statically allocated arrays

*/
