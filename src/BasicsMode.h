/*!
 \file BasicsMode.h
 \brief Definition of the BasicsMode class

 \see BasicsMode
 */

#ifndef BASICSMODE_H
#define BASICSMODE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "MainWindow.h"
#include "tools/insert_edge_tool.h"
#include "tools/delete_edge_tool.h"
#include "tools/collapse_edge_tool.h"
#include "tools/connect_edges_tool.h"
#include "tools/subdivide_edge_tool.h"
#include "tools/splice_corners_tool.h"
#include "tools/dup_component_tool.h"
#include "tools/geometric_tool.h"

class MainWindow;
class InsertEdgeTool;
class DeleteEdgeTool;
class CollapseEdgeTool;
class ConnectEdgesTool;
class SubdivideEdgeTool;
class SpliceCornersTool;
class DupComponentTool;
class GeometricTool;

class QComboBox;
class QPushButton;
class QGroupBox;
class QCheckBox;
class QLabel;

class BasicsMode : public QWidget {
Q_OBJECT

public:
	BasicsMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)  ;
	void  addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	void retranslateUi();
protected:

private:

	QWidget *parent_;
	QMenu *mBasicsMenu;
  GeometricTool *geometric_tool_;
  DupComponentTool *dup_component_tool_;
  InsertEdgeTool *insert_edge_tool_;
  DeleteEdgeTool *delete_edge_tool_;
  CollapseEdgeTool *collapse_edge_tool_;
  ConnectEdgesTool *connect_edges_tool_;
  SubdivideEdgeTool *subdivide_edge_tool_;
  SpliceCornersTool *splice_corners_tool_;

public slots:

};

#endif
