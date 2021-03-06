/*** ***/

/*! \mainpage TopMod Source Code Documentation
 *
 * \section intro_sec Introduction
 *
 * Welcome to the TopMod source code documentation. Here you will find descriptions of 
 * all the Namespaces, Classes, and other variables defined in the TopMod source code. 
 * This documentation was generated by 
 * Doxygen directly from the comments in the source code. Please see the "Related Pages"
 * section for a Todo List of things that currently need to be implemented or completed
 * in the TopMod source code.
 * 
 * For other more current updates to the development, please click visit the 
 * TopMod Wiki and the Discussion Forum.
 *
 */


#include <QApplication>
#include <QSplashScreen>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileOpenEvent>
// #include <QWaitCondition>

#include "MainWindow.h"
#include "TopMod.h"

#define max(a, b)   (((a) < (b)) ? (b) : (a))
#define min(x1,x2) ((x1)<(x2)?(x1):(x2))

// #ifdef TOPMOD_VERSION
// TOPMOD_VERSION 2.1
// #endif

#ifdef WITH_SPACENAV
#ifdef __APPLE__
/*
#include "TdxDeviceWrappers.h"

int TdxComputeAxes(const TdxDeviceAxes inArrayPtr)
{
    OSStatus err;
    // CFStringRef txt;
    // HIViewRef transField, rotatField;
	// const HIViewID viewID1 = { '3Dm1', k3DxCarbonDemoTextFieldID1 };
	// const HIViewID viewID2 = { '3Dm1', k3DxCarbonDemoTextFieldID2 };
    char st[128];
    
    //fprintf(stderr, "TdxComputeAxes...\n"); 
    
    // // find the refs of the EditText controls we want to write into 
    // err = HIViewFindByID(HIViewGetRoot(gWindow), viewID1, &transField);
    // assert(err == noErr);
    // err = HIViewFindByID(HIViewGetRoot(gWindow), viewID2, &rotatField);
    // assert(err == noErr);
    
    // write the values into the 1st EditText control 
    // sprintf(st, "(%hd, %hd, %hd)", inArrayPtr[0], inArrayPtr[1], inArrayPtr[2]);
    // txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
    // err = SetControlData(transField,
    //                      kControlEntireControl,
    //                      kControlEditTextCFStringTag,
    //                      sizeof(txt),
    //                      &txt);
    // CFRelease(txt);
    // 
    // // write the values into the 2nd EditText control 
    // sprintf(st, "(%hd, %hd, %hd)", inArrayPtr[3], inArrayPtr[4], inArrayPtr[5]);
    // txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
    // err = SetControlData(rotatField,
    //                      kControlEntireControl,
    //                      kControlEditTextCFStringTag,
    //                      sizeof(txt),
    //                      &txt);
    // CFRelease(txt);
    
    return err;
}

int TdxComputeButtons(UInt16 btnPressed, SInt16 btnState)
{
    OSStatus err;
		// QMessageBox::about(0, "About TopMod","SPACE NAV");

	//     CFStringRef txt;
	//     HIViewRef txtField;
	// const HIViewID viewID3 = { '3Dm1', k3DxCarbonDemoTextFieldID3 };
	//     char st[128];
	//     
	//     fprintf(stderr, "TdxComputeButtons...\n"); 
	//     
	// 
	//      find the ref of the EditText control we want to write into 
	//     err = HIViewFindByID(HIViewGetRoot(gWindow), viewID3, &txtField);
	//     assert(err == noErr);
	// 
	//     // write the values into the EditText control 
	//     sprintf(st, "last:%hu  status:%hd", btnPressed, btnState);
	//     txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
	//     err = SetControlData(txtField,
	//                          kControlEntireControl,
	//                          kControlEditTextCFStringTag,
	//                          sizeof(txt),
	//                          &txt);
	//     assert(err == noErr);
	//     CFRelease(txt);
    
    return err;
}


int TdxComputeEventZero()
{
    static SInt16 zero[6];
    memset(zero, 0, sizeof(zero));
    
  //fprintf(stderr, "TdxComputeEventZero...\n");
    

    // in this case, just process the zero event like any other axes event. 
    return TdxComputeAxes(zero);
}//*/
#endif
#endif 

#include <iostream>
int main( int argc, char **argv ) {
	TopMod app( argc, argv, true );
	QApplication::setWindowIcon(QIcon(":/images/topmod.png"));

	//this is a preliminary version of a splash screen functionality.
	//the app opens so quickly it is hardly shown, so i will consider adding a delay
	QPixmap pixmap(":/images/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap, Qt::WindowStaysOnTopHint);
	splash->setMask(pixmap.mask());
	splash->setGeometry(300,200,splash->width(),splash->height());
	splash->show();
	// splash->showMessage("doin stuff...");
	app.processEvents();
	//artificial delay for now to debug, and to give mad props to the developers
	// splash->finish(app.getMainWindow());
	QTimer::singleShot(1000, splash, SLOT(hide()));

 	#ifdef WITH_SPACENAV
		#ifdef __APPLE__
			//3d connexion stuff ... isn't working right now
			// OSStatus err;
			// err = TdxInitDevice('TopM', TRUE, kConnexionClientModeTakeOver, kConnexionMaskAll);
			//pass kConnexionClientModeTakeOver as the mode and try kConnexionMaskAxis | kConnexionMaskButtons as the mask. kConnexionMaskAll should also work.
		#endif
	#endif

	return app.exec( );
}
