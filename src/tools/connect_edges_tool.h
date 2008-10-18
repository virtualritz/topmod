/*
 * connect_edges_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef CONNECT_EDGES_TOOL_H_
#define CONNECT_EDGES_TOOL_H_

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

// Declaration of the class ConnectEdgesTool.
class ConnectEdgesTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:

  ConnectEdgesTool(QWidget *parent);
  void RetranslateUi();
protected:

public slots:

  void Activate();
};

#endif /* CONNECT_EDGES_TOOL_H_ */
