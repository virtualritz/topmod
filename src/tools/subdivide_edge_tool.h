/*
 * subdivide_edge_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef SUBDIVIDE_EDGE_TOOL_H_
#define SUBDIVIDE_EDGE_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

class SubdivideEdgeTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  SubdivideEdgeTool(QWidget *parent);
  void RetranslateUi();

  QLabel *num_subdivs_label_;
  QDoubleSpinBox *num_subdivs_spin_;

protected:

public slots:

  void Activate();
};

#endif /* SUBDIVIDE_EDGE_TOOL_H_ */
