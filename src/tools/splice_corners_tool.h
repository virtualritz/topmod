/*
 * splice_corners_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef SPLICE_CORNERS_TOOL_H_
#define SPLICE_CORNERS_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

class SpliceCornersTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  SpliceCornersTool(QWidget *parent);
  void RetranslateUi();
protected:

public slots:

  void Activate();
};

#endif /* SPLICE_CORNERS_TOOL_H_ */
