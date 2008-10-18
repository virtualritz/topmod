/*** ***/

#ifndef _TOPMOD_H
#define _TOPMOD_H

#include <QApplication>
#include <QEvent>
//
#include "MainWindow.h"

class TopMod : public QApplication {
  Q_OBJECT

private:
  MainWindow *mainWindow;
  // void loadFile(const QString &fileName);

public:
  TopMod( int & argc, char ** argv, bool GUIenabled );
  ~TopMod();
  MainWindow *getMainWindow();

  // #ifdef __APPLE__
  // bool macEventFilter( EventHandlerCallRef caller, EventRef event );
  // #endif

protected:
  bool event(QEvent *event);

};

#endif
