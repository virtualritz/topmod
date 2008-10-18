/*** ***/

/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef EXPERIMENTALMODE_H
#define EXPERIMENTALMODE_H

#include <QWidget>

#include <QGridLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.h"

class MainWindow;

class ExperimentalMode : public QWidget {
	Q_OBJECT

public:
	ExperimentalMode(QWidget *parent, QShortcutManager *sm,  QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();
	
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);

	QPushButton* addColorPreference(QLabel *label, QString text, QColor color, QGridLayout *layout, QStyle *style, int row, int col);

	void setButtonColor(QColor color, QPushButton *button);
	void setPaintBucketColor(QColor c);

	QWidget *mPaintBucketWidget;
	QAction *mPaintBucketAct;
	QGridLayout *mPaintBucketLayout;
	
protected:
	void setupPaintBucket();
	
public slots:
	// void changeCutOffsetE(double value);
	void triggerPaintBucket();
	void setPaintBucketColor();

private: 

	QWidget *mParent;
	QMenu *mExperimentalMenu;

	//Face Painting Color
	QColor mPaintBucketColor, mPaintBucketColorDefault;
	QPushButton *mPaintBucketColorButton;
	QPushButton *mPaintSelectedFacesButton;
	QPushButton *mResetMaterialsButton;
	QLabel *mPaintBucketColorLabel;


	QStyle* colorPickerStyle;
	QPalette p;

};
#endif
