/*** ***/

#ifndef _COLORABLE_RENDERER_HH_
#define _COLORABLE_RENDERER_HH_

/*
  ColorableRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, with material colors
*/

#include "../DLFLRenderer.h"

class ColorableRenderer;
typedef ColorableRenderer * ColorableRendererPtr;

class ColorableRenderer : public DLFLRenderer {

public :
  /* Default constructor */
  ColorableRenderer() : DLFLRenderer() { }

  ColorableRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
    {}//: DLFLRenderer(wc, wt, sc, st, vc, vt) { }
	  
  /* Copy constructor */
  ColorableRenderer(const ColorableRenderer& nr)
    : DLFLRenderer(nr) { }

  /* Assignment operator */
  ColorableRenderer& operator = (const ColorableRenderer& nr)	{
    DLFLRenderer::operator = (nr);
    return (*this);
  }

  /* Destructor */
  virtual ~ColorableRenderer() {}

  /* Implement render function */
  virtual int render(DLFLObjectPtr object) {
	
   	glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		glEnable(GL_BLEND);																			// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use

		if (DLFLRenderer::antialiasing){
	    glEnable( GL_LINE_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);									// Set Line Antialiasing
		}
		else {
	    glDisable( GL_LINE_SMOOTH );
		}
    //object->renderFaces();
    gr->render( object );
    drawOverlays( object );
    glDisable(GL_CULL_FACE);
    return 0;

  }

  virtual void setState( ) { 
    gr->useLighting = false;
		gr->useColorable = true;
    gr->useMaterial = false;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _COLORABLE_RENDERER_HH_ */
