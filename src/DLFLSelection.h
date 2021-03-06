/*** ***/

#ifndef _DLFLSELECTION_H_
#define _DLFLSELECTION_H_

#include <DLFLObject.h>

using namespace DLFL;

/*!
	\file DLFLSelection.h
	\brief Code for selection of vertices, edges and faces
	
	\see MainWindow
*/

void renderVerticesForSelect( DLFLObjectPtr obj ); 		//!< Render the vertices for selection
void renderEdgesForSelect( DLFLObjectPtr obj );				//!< Render the edges for selection
void renderFacesForSelect(DLFLObjectPtr obj, bool transparent = false);				//!< Render the faces for selection
void renderFaceVerticesForSelect( DLFLFacePtr fp );		//!< Render the face vertices of a face for selection

#endif // _DLFLSELECTION_H_
