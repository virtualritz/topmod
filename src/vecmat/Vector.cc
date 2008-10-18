/*** ***/

/* $Id: Vector.cc,v 1.1 2000/04/30 02:50:51 vinod Exp $ */

#include "Vector.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"

   // Cross product between 2D vectors
Vector3d operator % (const Vector2d& vec1, const Vector2d& vec2)
{
  Vector3d crossp, v1(vec1), v2(vec2);

  crossp = v1 % v2;
  return crossp;
}

   // Cross product between 3D and 2D vectors
Vector3d operator % (const Vector3d& vec1, const Vector2d& vec2)
{
  Vector3d crossp, v2(vec2);

  crossp = vec1 % v2;
  return crossp;
}

   // Cross product between 2D and 3D vectors
Vector3d operator % (const Vector2d& vec1, const Vector3d& vec2)
{
  Vector3d crossp, v1(vec1);

  crossp = v1 % vec2;
  return crossp;
}

/*
  $Log: Vector.cc,v $
  Revision 1.1  2000/04/30 02:50:51  vinod
  Common subroutines for the Vector classes

*/
