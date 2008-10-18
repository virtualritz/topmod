/*
 * collapse_edge_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef COLLAPSE_EDGE_TOOL_H_
#define COLLAPSE_EDGE_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

// Declaration of the class CollapseEdgeTool.
class CollapseEdgeTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  CollapseEdgeTool(QWidget *parent);
  void RetranslateUi();
protected:

public slots:

  void Activate();
};

#endif /* COLLAPSE_EDGE_TOOL_H_ */
