/*** ***/

#include "CgData.h"

#ifdef GPU_OK

namespace Cg {

	/*!
	* \brief private Constructor
	*/
	CgData::CgData(){
		
	}

	CgData* CgData::mInstance = 0; //!< initialize the singleton pointer to 0

	/*!
	* \brief ensures only 1 instance (singleton)
	*/
	CgData* CgData::instance( ) {
	  if ( !mInstance )
	    mInstance = new CgData;
	  return mInstance;
	}

	void checkCgError( CGcontext &context, int id ) {
		CGerror err = cgGetError();
		if (err != CG_NO_ERROR) {
			printf("CG error %d: %s\n", id, cgGetErrorString(err));
			if ( context ) {
				printf("%s\n", cgGetLastListing( context ));
			}
		}
	};
	
	void checkForCgError(const char *situation)	{
	  CGerror error;
	  const char *string = cgGetLastErrorString(&error);

	  if (error != CG_NO_ERROR) {
	    printf("%s: %s\n", situation, string);
	    if (error == CG_COMPILER_ERROR) {
	      printf("%s\n", cgGetLastListing(CgData::instance()->context));
	    }
	    // exit(1);
	  }
	}
	

} // end namespace Cg

#endif // GPU_OK
