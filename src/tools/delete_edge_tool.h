/*
 * insert_edge_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef DELETE_EDGE_TOOL_H_
#define DELETE_EDGE_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

// Declaration of the class DeleteEdgeTool.
class DeleteEdgeTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  DeleteEdgeTool(QWidget *parent);
  void RetranslateUi();

  QCheckBox *cleanup_check_;

protected:

public slots:

  void Activate();
};

#endif /* DELETE_EDGE_TOOL_H_ */
