/*
 * insert_edge_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef INSERT_EDGE_TOOL_H_
#define INSERT_EDGE_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

// Declaration of the class InsertEdgeTool.
class InsertEdgeTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  InsertEdgeTool(QWidget *parent);
  void RetranslateUi();
protected:

public slots:

  void Activate();
};

#endif /* INSERT_EDGE_TOOL_H_ */
