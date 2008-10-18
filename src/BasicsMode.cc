/*!
 \ingroup gui
 @{

 \class BasicsMode
 \brief Basics Operations including insert-edge and delete-edge.

 \note

 \see BasicsMode
 */

#include <QtGui>

#include "BasicsMode.h"

/*!
 * \brief Constructor
 *
 * @param parent the MainWindow widget
 * @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
 * @param actionList the master list of actions for use with the CommandCompleter class
 *
 */
BasicsMode::BasicsMode(QWidget *parent, QShortcutManager *sm,
                       QWidget *actionList) :
  QWidget(parent) {

  setParent(0);
  parent_ = parent;
  insert_edge_tool_ = new InsertEdgeTool(parent);
  delete_edge_tool_ = new DeleteEdgeTool(parent);
  collapse_edge_tool_ = new CollapseEdgeTool(parent);
  subdivide_edge_tool_ = new SubdivideEdgeTool(parent);
  connect_edges_tool_ = new ConnectEdgesTool(parent);
  splice_corners_tool_ = new SpliceCornersTool(parent);
  geometric_tool_ = GeometricTool::GetInstance(parent);
  // geometric_tool_ = new GeometricTool(parent);
  dup_component_tool_ = new DupComponentTool(parent);

  actionList->addAction(insert_edge_tool_->action_);
  actionList->addAction(delete_edge_tool_->action_);
  actionList->addAction(collapse_edge_tool_->action_);
  actionList->addAction(subdivide_edge_tool_->action_);
  actionList->addAction(connect_edges_tool_->action_);
  actionList->addAction(splice_corners_tool_->action_);
  actionList->addAction(geometric_tool_->action_);
  actionList->addAction(dup_component_tool_->action_);
}

QMenu* BasicsMode::getMenu() {
  mBasicsMenu = new QMenu(tr("Basics"));
  mBasicsMenu->addAction(insert_edge_tool_->action_);
  mBasicsMenu->addAction(delete_edge_tool_->action_);
  mBasicsMenu->addAction(collapse_edge_tool_->action_);
  mBasicsMenu->addAction(subdivide_edge_tool_->action_);
  mBasicsMenu->addAction(connect_edges_tool_->action_);
  mBasicsMenu->addAction(splice_corners_tool_->action_);
  mBasicsMenu->addAction(geometric_tool_->action_);
  mBasicsMenu->addAction(dup_component_tool_->action_);
  return mBasicsMenu;
}

void BasicsMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar,
                            QStackedWidget *stackedWidget) {

  actionGroup->addAction(insert_edge_tool_->action_);
  actionGroup->addAction(delete_edge_tool_->action_);
  actionGroup->addAction(collapse_edge_tool_->action_);
  actionGroup->addAction(subdivide_edge_tool_->action_);
  actionGroup->addAction(connect_edges_tool_->action_);
  actionGroup->addAction(splice_corners_tool_->action_);
  actionGroup->addAction(geometric_tool_->action_);
  actionGroup->addAction(dup_component_tool_->action_);

  toolBar->addAction(insert_edge_tool_->action_);
  toolBar->addAction(delete_edge_tool_->action_);
  toolBar->addAction(collapse_edge_tool_->action_);
  toolBar->addAction(subdivide_edge_tool_->action_);
  toolBar->addAction(connect_edges_tool_->action_);
  toolBar->addAction(splice_corners_tool_->action_);
  toolBar->addAction(geometric_tool_->action_);
  toolBar->addAction(dup_component_tool_->action_);

  stackedWidget->addWidget(insert_edge_tool_->widget_);
  stackedWidget->addWidget(delete_edge_tool_->widget_);
  stackedWidget->addWidget(collapse_edge_tool_->widget_);
  stackedWidget->addWidget(subdivide_edge_tool_->widget_);
  stackedWidget->addWidget(connect_edges_tool_->widget_);
  stackedWidget->addWidget(splice_corners_tool_->widget_);
  stackedWidget->addWidget(geometric_tool_->widget_);
  stackedWidget->addWidget(dup_component_tool_->widget_);

}

void BasicsMode::retranslateUi() {
  mBasicsMenu->setTitle(tr("Basics"));
}
