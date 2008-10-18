/*** ***/

#include "TopMod.h"

TopMod::~TopMod() {

}

TopMod::TopMod(int & argc, char ** argv, bool GUIenabled) :
  QApplication(argc, argv, GUIenabled) {

  setApplicationName("TopMod");

  mainWindow = new MainWindow();

  QString locale = QLocale::system().name();
  QTranslator translator;
  translator.load(QString(":/topmod_") + locale);
  installTranslator(&translator);

  processEvents();

  mainWindow->resize(1000, 800);
  mainWindow->show();
}

MainWindow *TopMod::getMainWindow() {
  return mainWindow;
}

bool TopMod::event(QEvent *event) {

  switch (event->type()) {
    case 116:// case QEvent::FileOpen:
      mainWindow->loadFile(static_cast<QFileOpenEvent *> (event)->file());
      return true;
    default:
      return QApplication::event(event);
  };
}

// #ifdef __APPLE__
// bool TopMod::macEventFilter( EventHandlerCallRef caller, EventRef event ){
// 	// std::cout << event << "\n";
// 	return false;
// }
// #endif
