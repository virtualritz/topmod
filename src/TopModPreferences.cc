/*** ***/

#include "TopModPreferences.h"

TopModPreferences::TopModPreferences(QSettings *settings,
                                     StyleSheetEditor *sse,
                                     QShortcutManager *sm, QWidget *parent) :
  QDialog(parent) {

  settings_ = settings;
  parent_ = parent;
  setSizeGripEnabled(false);
  resize(700, 600);
  setFixedSize(700, 600);
  setWindowTitle(tr("TopMod Preferences"));
  // setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

  // mMainTab = new QWidget;
  mStyleSheetsTab = sse;
  mShortcutsManager = sm;
  mShortcutsTab = sm->getShortcutDialog();

  mPrefTabs = new QTabWidget;
  //setup layouts
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(mPrefTabs);
  QHBoxLayout *lowLayout = new QHBoxLayout;
  mainLayout->addLayout(lowLayout, 1);
  lowLayout->addStretch(1);

  mOkButton = new QPushButton(tr("OK"));
  // mOkButton->setDefault(true);
  connect(mOkButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
  mCancelButton = new QPushButton(tr("Cancel"));
  connect(mCancelButton, SIGNAL(clicked()), this, SLOT(discardSettings()));
  lowLayout->addWidget(mOkButton);
  lowLayout->addWidget(mCancelButton);
  setLayout(mainLayout);

  createTabs();
  readSettings();
  setupColors();
  setupMain();

}

TopModPreferences::~TopModPreferences() {

}

int TopModPreferences::display() {
  mShortcutsTab->exec();
  return exec();
}

void TopModPreferences::createTabs() {
  mMainTab = new QWidget;
  mColorsTab = new QWidget;
  mPrefTabs->addTab(mMainTab, tr("Main"));
  mPrefTabs->addTab(mColorsTab, tr("Colors"));
  mPrefTabs->addTab(((QWidget*) mShortcutsTab), tr("Shortcuts"));
  mPrefTabs->addTab(mStyleSheetsTab, tr("Stylesheets"));
#ifdef WITH_PYTHON
  mScriptEditTab = new QWidget;
  mPrefTabs->addTab(mScriptEditTab,tr("Script Editor"));
#endif
}

void TopModPreferences::saveSettings() {
  settings_->beginGroup("Colors");
  settings_->setValue("ViewportColor", mViewportColor);
  settings_->setValue("RenderColor", mRenderColor);
  settings_->setValue("CoolLightColor", mCoolLightColor);
  settings_->setValue("WarmLightColor", mWarmLightColor);
  settings_->setValue("WireframeColor", mWireframeColor);
  settings_->setValue("SilhouetteColor", mSilhouetteColor);
  settings_->setValue("PatchBoundaryColor", mPatchBoundaryColor);
  settings_->setValue("SelectedVertexColor", mSelectedVertexColor);
  settings_->setValue("SelectedEdgeColor", mSelectedEdgeColor);
  settings_->setValue("SelectedFaceColor", mSelectedFaceColor);
  settings_->setValue("VertexIDBgColor", mVertexIDBgColor);
  settings_->setValue("EdgeIDBgColor", mEdgeIDBgColor);
  settings_->setValue("NormalColor", mNormalColor);
  settings_->setValue("FaceCentroidColor", mFaceCentroidColor);
  settings_->setValue("FaceIDBgColor", mFaceIDBgColor);
  settings_->setValue("LightIntensity", mLightIntensity);
  settings_->endGroup();

  settings_->beginGroup("Display");
  settings_->setValue("NormalThickness", mNormalThickness);
  settings_->setValue("FaceCentroidThickness", mFaceCentroidThickness);
  settings_->setValue("NormalLength", mNormalLength);
  settings_->setValue("SilhouetteThickness", mSilhouetteThickness);
  settings_->setValue("VertexThickness", mVertexThickness);
  settings_->setValue("SelectedVertexThickness", mSelectedVertexThickness);
  settings_->setValue("SelectedEdgeThickness", mSelectedEdgeThickness);
  settings_->endGroup();

  settings_->beginGroup("Camera");
  settings_->setValue("NearPlane", mCameraNearPlaneSpinBox->value());
  settings_->setValue("FarPlane", mCameraFarPlaneSpinBox->value());
  settings_->setValue("Fov", mCameraFovSpinBox->value());
  settings_->endGroup();

  settings_->beginGroup("Save Options");
  settings_->setValue("SaveDirectory", mSaveDirectoryLineEdit->text());
  settings_->setValue("TextureSaveDirectory",
      mTextureSaveDirectoryLineEdit->text());
  settings_->setValue("AutoSave", mAutoSaveCheckBox->checkState());
  settings_->setValue("AutoSaveDelay", mAutoSaveDelaySpinBox->value());
  settings_->setValue("IncrementalSave", mIncrementalSaveCheckBox->checkState());
  settings_->setValue("IncrementalSaveMax", mIncrementalSaveMaxSpinBox->value());
  settings_->endGroup();

  settings_->beginGroup("MainWindow");
  settings_->setValue("pos", ((MainWindow*) parent_)->pos());
  settings_->setValue("size", ((MainWindow*) parent_)->size());
  settings_->setValue("showStartupDialogAtStartup",
      ((MainWindow*) parent_)->getShowStartupDialogAtStartup());
  settings_->setValue("toolOptionsPos",
      ((MainWindow*) parent_)->mToolOptionsDockWidget->pos());
  settings_->setValue("CommandCompleterIndex",
      mCommandCompleterIndexToggle->checkState());
  settings_->setValue("SingleClickExtrude",
      mSingleClickExtrudeCheckBox->checkState());
  settings_->setValue("FaceAreaTolerance",((MainWindow*) parent_)->getFaceAreaTolerance());
  settings_->setValue("Valence2SplitOffset",((MainWindow*) parent_)->getValence2SplitOffset());

#ifdef WITH_PYTHON
  settings_->setValue("scriptEditorPos", ((MainWindow*)parent_)->mScriptEditorDockWidget->pos());
  settings_->setValue("scriptEditorSize", ((MainWindow*)parent_)->mScriptEditorDockWidget->size());
#endif
  settings_->endGroup();

  accept();
}

void TopModPreferences::readSettings() {

  //load the settings file and set defaults if values don't exist
  settings_->beginGroup("Colors");
  mViewportColorDefault = QColor(67, 68, 88); //this is where you set defaults now. only one time in the code it
  mViewportColor
      = settings_->value("ViewportColor", mViewportColorDefault).value<QColor> ();
  mRenderColorDefault = QColor(255, 151, 92);
  mRenderColor = settings_->value("RenderColor", mRenderColorDefault).value<
      QColor> ();
  mCoolLightColorDefault = QColor(27, 26, 23);
  mCoolLightColor
      = settings_->value("CoolLightColor", mCoolLightColorDefault).value<QColor> ();
  mWarmLightColorDefault = QColor(255, 250, 226);
  mWarmLightColor
      = settings_->value("WarmLightColor", mWarmLightColorDefault).value<QColor> ();
  mWireframeColorDefault = QColor(0, 0, 0, 255);
  mWireframeColor
      = settings_->value("WireframeColor", mWireframeColorDefault).value<QColor> ();
  mSilhouetteColorDefault = QColor(0, 0, 0, 255);
  mSilhouetteColor = settings_->value("SilhouetteColor",
      mSilhouetteColorDefault).value<QColor> ();
  mPatchBoundaryColorDefault = QColor(0, 0, 0);
  mPatchBoundaryColor = settings_->value("PatchBoundaryColor",
      mPatchBoundaryColor).value<QColor> ();
  mSelectedVertexColorDefault = QColor(0, 255, 0, 127);
  mSelectedVertexColor = settings_->value("SelectedVertexColor",
      mSelectedVertexColorDefault).value<QColor> ();
  mSelectedEdgeColorDefault = QColor(255, 0, 0, 127);
  mSelectedEdgeColor = settings_->value("SelectedEdgeColor",
      mSelectedEdgeColorDefault).value<QColor> ();
  mSelectedFaceColorDefault = QColor(247, 0, 229, 127);
  mSelectedFaceColor = settings_->value("SelectedFaceColor",
      mSelectedFaceColorDefault).value<QColor> ();
  mVertexIDBgColorDefault = QColor(127, 0, 0, 127);
  mVertexIDBgColor = settings_->value("VertexIDBgColor",
      mVertexIDBgColorDefault).value<QColor> ();
  mEdgeIDBgColorDefault = QColor(0, 127, 0, 127);
  mEdgeIDBgColor
      = settings_->value("EdgeIDBgColor", mEdgeIDBgColorDefault).value<QColor> ();
  mNormalColorDefault = QColor(127, 127, 0);
  mNormalColor = settings_->value("NormalColor", mNormalColorDefault).value<
      QColor> ();
  mFaceCentroidColorDefault = QColor(127, 0, 127);
  mFaceCentroidColor = settings_->value("FaceCentroidColor",
      mFaceCentroidColorDefault).value<QColor> ();
  mFaceIDBgColorDefault = QColor(0, 0, 127, 127);
  mFaceIDBgColor
      = settings_->value("FaceIDBgColor", mFaceIDBgColorDefault).value<QColor> ();
  settings_->endGroup();

  settings_->beginGroup("Display");
  mLightIntensityDefault = 2.0;
  mLightIntensity
      = settings_->value("LightIntensity", mLightIntensityDefault).toDouble();
  mWireframeThicknessDefault = 0.1;
  mWireframeThickness = settings_->value("WireframeThickness",
      mWireframeThicknessDefault).toDouble();
  mSilhouetteThicknessDefault = 6.0;
  mSilhouetteThickness = settings_->value("SilhouetteThickness",
      mSilhouetteThicknessDefault).toDouble();
  mVertexThicknessDefault = 5.0;
  mVertexThickness = settings_->value("VertexThickness",
      mVertexThicknessDefault).toDouble();
  mSelectedVertexThicknessDefault = 8.0;
  mSelectedVertexThickness = settings_->value("SelectedVertexThickness",
      mSelectedVertexThicknessDefault).toDouble();
  mSelectedEdgeThicknessDefault = 5.0;
  mSelectedEdgeThickness = settings_->value("SelectedEdgeThickness",
      mSelectedEdgeThicknessDefault).toDouble();
  mNormalThicknessDefault = 0.1;
  mNormalThickness = settings_->value("NormalThickness",
      mNormalThicknessDefault).toDouble();
  mNormalLengthDefault = 1.0;
  mNormalLength
      = settings_->value("NormalLength", mNormalLengthDefault).toDouble();
  mFaceCentroidThicknessDefault = 5.0;
  mFaceCentroidThickness = settings_->value("FaceCentroidThickness",
      mFaceCentroidThicknessDefault).toDouble();
  settings_->endGroup();

  settings_->beginGroup("Camera");
  mCameraNearPlaneDefault = 1.0;
  mCameraNearPlane
      = settings_->value("NearPlane", mCameraNearPlaneDefault).toDouble();
  mCameraFarPlaneDefault = 200;
  mCameraFarPlane
      = settings_->value("FarPlane", mCameraFarPlaneDefault).toDouble();
  mCameraFovDefault = 60;
  mCameraFov = settings_->value("Fov", mCameraFovDefault).toDouble();
  settings_->endGroup();

  settings_->beginGroup("Save Options");
  mSaveDirectoryDefault = QDir::currentPath();
  mSaveDirectory
      = settings_->value("SaveDirectory", mSaveDirectoryDefault).toString();
  mTextureSaveDirectoryDefault = QDir::currentPath();
  mTextureSaveDirectory = settings_->value("TextureSaveDirectory",
      mTextureSaveDirectoryDefault).toString();
  mAutoSaveDefault = false;
  mAutoSave = settings_->value("AutoSave", mAutoSaveDefault).toBool();
  mAutoSaveDelayDefault = 5;
  mAutoSaveDelay
      = settings_->value("AutoSaveDelay", mAutoSaveDelayDefault).toDouble();
  mIncrementalSaveDefault = false;
  mIncrementalSave = settings_->value("IncrementalSave",
      mIncrementalSaveDefault).toBool();
  mIncrementalSaveMaxDefault = 20;
  mIncrementalSaveMax = settings_->value("IncrementalSaveMax",
      mIncrementalSaveMaxDefault).toDouble();
  settings_->endGroup();

  settings_->beginGroup("MainWindow");
  QPoint pos = settings_->value("pos", QPoint(100, 100)).toPoint();
  if (pos.y() == 0) pos.setY(pos.y() + 20);
  QSize size = settings_->value("size", QSize(800, 600)).toSize();
  QPoint toolOptionsPos =
      settings_->value("toolOptionsPos", QPoint()).toPoint();

  mCommandCompleterIndexDefault = false;
  mCommandCompleterIndex = settings_->value("CommandCompleterIndex",
      mCommandCompleterIndexDefault).toBool();

  mSingleClickExtrudeDefault = false;
  mSingleClickExtrude = settings_->value("SingleClickExtrude",
      mSingleClickExtrudeDefault).toBool();

  //selection stuff //dave
  face_area_tolerance_default_ = 0.010;
  face_area_tolerance_ = settings_->value("FaceAreaTolerance",
      face_area_tolerance_default_).toDouble();
  valence_2_split_offset_default_ = 0.200;
  valence_2_split_offset_ = settings_->value("Valence2SplitOffset",
      valence_2_split_offset_default_).toDouble();

#ifdef WITH_PYTHON
  QSize scriptEditorSize = settings_->value("scriptEditorSize", QSize(500,300)).toSize();
  QPoint scriptEditorPos = settings_->value("scriptEditorPos", QPoint(20, QApplication::desktop()->height()-500)).toPoint();
#endif
  mShowStartupDialogAtStartup = settings_->value("showStartupDialogAtStartup",
      false).toBool();
  settings_->endGroup();

  ((MainWindow*) parent_)->resize(size.width(), size.height());
  ((MainWindow*) parent_)->move(pos);
  ((MainWindow*) parent_)->setShowStartupDialogAtStartup(
      (int) mShowStartupDialogAtStartup);
  ((MainWindow*) parent_)->mToolOptionsDockWidget->setGeometry(10
      + ((MainWindow*) parent_)->x(), ((MainWindow*) parent_)->y() + 150,
      ((MainWindow*) parent_)->mToolOptionsDockWidget->width(),
      ((MainWindow*) parent_)->mToolOptionsDockWidget->height());
#ifdef WITH_PYTHON
  ((MainWindow*)parent_)->mScriptEditorDockWidget->resize(scriptEditorSize.width(),scriptEditorSize.height());
  ((MainWindow*)parent_)->mScriptEditorDockWidget->move(scriptEditorPos);
#endif

  //initialize the settings
  ((MainWindow*) parent_)->getActive()->setViewportColor(mViewportColor);
  ((MainWindow*) parent_)->getActive()->setRenderColor(mRenderColor);
  ((MainWindow*) parent_)->getActive()->setCoolLightColor(mCoolLightColor);
  ((MainWindow*) parent_)->getActive()->setWarmLightColor(mWarmLightColor);
  ((MainWindow*) parent_)->getActive()->setWireframeColor(mWireframeColor);
  ((MainWindow*) parent_)->getActive()->setSilhouetteColor(mSilhouetteColor);
  ((MainWindow*) parent_)->getActive()->setSelectedVertexColor(
      mSelectedVertexColor);
  ((MainWindow*) parent_)->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
  ((MainWindow*) parent_)->getActive()->setSelectedFaceColor(mSelectedFaceColor);
  // ((MainWindow*)parent_)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
  ((MainWindow*) parent_)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
  ((MainWindow*) parent_)->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
  ((MainWindow*) parent_)->getActive()->setNormalColor(mNormalColor);
  ((MainWindow*) parent_)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
  ((MainWindow*) parent_)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
  ((MainWindow*) parent_)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
  ((MainWindow*) parent_)->getActive()->setNormalColor(mNormalColor);
  ((MainWindow*) parent_)->getActive()->setNormalThickness(mNormalThickness);
  ((MainWindow*) parent_)->getActive()->setFaceCentroidThickness(
      mFaceCentroidThickness);
  ((MainWindow*) parent_)->getActive()->setNormalLength(mNormalLength);
  ((MainWindow*) parent_)->getActive()->setWireframeThickness(
      mWireframeThickness);
  ((MainWindow*) parent_)->getActive()->setSilhouetteThickness(
      mSilhouetteThickness);
  ((MainWindow*) parent_)->getActive()->setVertexThickness(mVertexThickness);
  ((MainWindow*) parent_)->getActive()->setSelectedVertexThickness(
      mSelectedVertexThickness);
  ((MainWindow*) parent_)->getActive()->setSelectedEdgeThickness(
      mSelectedEdgeThickness);

  ((MainWindow*) parent_)->getActive()->setNearPlane(mCameraNearPlane);
  ((MainWindow*) parent_)->getActive()->setFarPlane(mCameraFarPlane);
  ((MainWindow*) parent_)->getActive()->setFOV(mCameraFov);
  //save options
  ((MainWindow*) parent_)->setSaveDirectory(mSaveDirectory);
  ((MainWindow*) parent_)->setTextureSaveDirectory(mTextureSaveDirectory);
  ((MainWindow*) parent_)->setAutoSave(mAutoSave);
  ((MainWindow*) parent_)->setAutoSaveDelay(mAutoSaveDelay);
  ((MainWindow*) parent_)->setIncrementalSave(mIncrementalSave);
  ((MainWindow*) parent_)->setIncrementalSaveMax(mIncrementalSaveMax);
  ((MainWindow*) parent_)->setCommandCompleterIndexToggle(
      mCommandCompleterIndex);
  ((MainWindow*) parent_)->setSingleClickExtrude(mSingleClickExtrude);

  ((MainWindow*) parent_)->changeFaceAreaTolerance(face_area_tolerance_);
  ((MainWindow*) parent_)->changeValence2SplitOffset(valence_2_split_offset_);
}

void TopModPreferences::discardSettings() {
  reject();
}

void TopModPreferences::setButtonColor(QColor color, QPushButton *button) {
  // color = c;
  QPalette p = button->palette();
  p.setColor(QPalette::Button, color);
  button->setPalette(p);
}

/*!
 * \brief load all the default settings and attach this as a signal for a reset button
 */
void TopModPreferences::loadDefaults() {

  mViewportColor = mViewportColorDefault;
  ((MainWindow*) parent_)->getActive()->setViewportColor(mViewportColor);
  setButtonColor(mViewportColor, mViewportColorButton);

  mRenderColor = mRenderColorDefault;
  ((MainWindow*) parent_)->getActive()->setRenderColor(mRenderColor);
  setButtonColor(mRenderColor, mRenderColorButton);

  mCoolLightColor = mCoolLightColorDefault;
  ((MainWindow*) parent_)->getActive()->setCoolLightColor(mCoolLightColor);
  setButtonColor(mCoolLightColor, mCoolLightColorButton);

  mWarmLightColor = mWarmLightColorDefault;
  ((MainWindow*) parent_)->getActive()->setWarmLightColor(mWarmLightColor);
  setButtonColor(mWarmLightColor, mWarmLightColorButton);

  mWireframeColor = mWireframeColorDefault;
  ((MainWindow*) parent_)->getActive()->setWireframeColor(mWireframeColor);
  setButtonColor(mWireframeColor, mWireframeColorButton);

  mSilhouetteColor = mSilhouetteColorDefault;
  ((MainWindow*) parent_)->getActive()->setSilhouetteColor(mSilhouetteColor);
  setButtonColor(mSilhouetteColor, mSilhouetteColorButton);

  mPatchBoundaryColor = mPatchBoundaryColorDefault;
  // ((MainWindow*)parent_)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
  setButtonColor(mPatchBoundaryColor, mPatchBoundaryColorButton);

  mSelectedVertexColor = mSelectedVertexColorDefault;
  ((MainWindow*) parent_)->getActive()->setSelectedVertexColor(
      mSelectedVertexColor);
  setButtonColor(mSelectedVertexColor, mSelectedVertexColorButton);

  mSelectedEdgeColor = mSelectedEdgeColorDefault;
  ((MainWindow*) parent_)->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
  setButtonColor(mSelectedEdgeColor, mSelectedEdgeColorButton);

  mSelectedFaceColor = mSelectedFaceColorDefault;
  ((MainWindow*) parent_)->getActive()->setSelectedFaceColor(mSelectedFaceColor);
  setButtonColor(mSelectedFaceColor, mSelectedFaceColorButton);

  mVertexIDBgColor = mVertexIDBgColor;
  ((MainWindow*) parent_)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
  setButtonColor(mVertexIDBgColor, mVertexIDBgColorButton);

  mNormalColor = mNormalColorDefault;
  ((MainWindow*) parent_)->getActive()->setNormalColor(mNormalColor);
  setButtonColor(mNormalColor, mNormalColorButton);

  mFaceCentroidColor = mFaceCentroidColorDefault;
  ((MainWindow*) parent_)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
  setButtonColor(mFaceCentroidColor, mFaceCentroidColorButton);

  mFaceIDBgColor = mFaceIDBgColorDefault;
  ((MainWindow*) parent_)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
  setButtonColor(mFaceIDBgColor, mFaceIDBgColorButton);

  mLightIntensity = mLightIntensityDefault;
  mLightIntensitySpinBox->setValue(mLightIntensity);

  mWireframeThickness = mWireframeThicknessDefault;
  mWireframeThicknessSpinBox->setValue(mWireframeThickness);

  mSilhouetteThickness = mSilhouetteThicknessDefault;
  mSilhouetteThicknessSpinBox->setValue(mSilhouetteThickness);

  mNormalThickness = mNormalThicknessDefault;
  mNormalThicknessSpinBox->setValue(mNormalThickness);

  mNormalLength = mNormalLengthDefault;
  mNormalLengthSpinBox->setValue(mNormalLength);

  mFaceCentroidThickness = mFaceCentroidThicknessDefault;
  mFaceCentroidThicknessSpinBox->setValue(mFaceCentroidThickness);

  mVertexThickness = mVertexThicknessDefault;
  mVertexThicknessSpinBox->setValue(mVertexThickness);

  mSelectedVertexThickness = mSelectedVertexThicknessDefault;
  mVertexThicknessSpinBox->setValue(mSelectedVertexThickness);

  mSelectedEdgeThickness = mSelectedEdgeThicknessDefault;
  mSelectedEdgeThicknessSpinBox->setValue(mSelectedVertexThickness);

  //camera stuff
  mCameraNearPlane = mCameraNearPlaneDefault;
  ((MainWindow*) parent_)->getActive()->setNearPlane(mCameraNearPlane);

  mCameraFarPlane = mCameraFarPlaneDefault;
  ((MainWindow*) parent_)->getActive()->setFarPlane(mCameraFarPlane);

  mCameraFov = mCameraFovDefault;
  ((MainWindow*) parent_)->getActive()->setFOV(mCameraFov);

  //selection stuff //dave
  face_area_tolerance_ = face_area_tolerance_default_;
  ((MainWindow*) parent_)->changeFaceAreaTolerance(face_area_tolerance_);

  valence_2_split_offset_ = valence_2_split_offset_default_;
  ((MainWindow*) parent_)->changeValence2SplitOffset(valence_2_split_offset_);

  //save options!
  mSaveDirectory = mSaveDirectoryDefault;
  ((MainWindow*) parent_)->setSaveDirectory(mSaveDirectory);

  mTextureSaveDirectory = mTextureSaveDirectoryDefault;
  ((MainWindow*) parent_)->setTextureSaveDirectory(mTextureSaveDirectory);

  mAutoSave = mAutoSaveDefault;
  ((MainWindow*) parent_)->setAutoSave(mAutoSave);

  mAutoSaveDelay = mAutoSaveDelayDefault;
  ((MainWindow*) parent_)->setAutoSaveDelay(mAutoSaveDelay);

  mIncrementalSave = mIncrementalSaveDefault;
  ((MainWindow*) parent_)->setIncrementalSave(mIncrementalSave);

  mIncrementalSaveMax = mIncrementalSaveMaxDefault;
  ((MainWindow*) parent_)->setIncrementalSaveMax(mIncrementalSaveMax);

  mCommandCompleterIndex = mCommandCompleterIndexDefault;
  ((MainWindow*) parent_)->setCommandCompleterIndexToggle(
      mCommandCompleterIndex);

  mSingleClickExtrude = mSingleClickExtrudeDefault;
  ((MainWindow*) parent_)->setSingleClickExtrude(mSingleClickExtrude);

}

void TopModPreferences::setupMain() {

  QGridLayout *mMainLayout = new QGridLayout;

  // mResetCameraButton = new QPushButton(tr("Reset"));
  // connect(mResetCameraButton,SIGNAL(clicked()),
  // 				((MainWindow*)parent_), SLOT(switchPerspView()));

  // mShowStartupDialogAtStartupCheckBox = new QCheckBox(tr("Show Tutorial Dialog at Startup"));
  // mShowStartupDialogAtStartupCheckBox->setChecked(mShowStartupDialogAtStartup);
  // connect(mShowStartupDialogAtStartupCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setShowStartupDialogAtStartup(int)));

  QStyle* colorPickerStyle = new QPlastiqueStyle;
  QPalette p;

  //camera nearplane
  mCameraNearPlaneSpinBox = addSpinBoxPreference(mCameraNearPlaneLabel, tr(
      "Near Plane:"), 0.0, 100, 0.1, mCameraNearPlane, 1, mMainLayout, 0, 0);
  connect(mCameraNearPlaneSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setNearPlane(double)));

  //camera farplane
  mCameraFarPlaneSpinBox = addSpinBoxPreference(mCameraFarPlaneLabel, tr(
      "Far Plane:"), 10, 1000, 0.1, mCameraFarPlane, 1, mMainLayout, 1, 0);
  connect(mCameraFarPlaneSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setFarPlane(double)));

  //camera nearplane
  mCameraFovSpinBox = addSpinBoxPreference(mCameraFovLabel,
      tr("Field of View:"), 15, 100, 1, mCameraFov, 0, mMainLayout, 2, 0);
  connect(mCameraFovSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setFOV(double)));

  //auto save toggle
  mAutoSaveCheckBox = new QCheckBox(tr("Auto Save:"), this);
  mAutoSaveCheckBox->setChecked(mAutoSave);
  connect(mAutoSaveCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*) parent_), SLOT(setAutoSave(int)));
  mMainLayout->addWidget(mAutoSaveCheckBox, 3, 0);

  //auto save delay in minutes
  mAutoSaveDelaySpinBox = addSpinBoxPreference(mAutoSaveDelayLabel, tr(
      "Auto Save Delay\n(in minutes):"), 1, 30, 1, mAutoSaveDelay, 0,
      mMainLayout, 4, 0);
  connect(mAutoSaveDelaySpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_), SLOT(setAutoSaveDelay(double)));

  //incremental save toggle
  mIncrementalSaveCheckBox = new QCheckBox(tr("Incremental Save:"), this);
  mIncrementalSaveCheckBox->setChecked(mIncrementalSave);
  connect(mIncrementalSaveCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*) parent_), SLOT(setIncrementalSave(int)));
  mMainLayout->addWidget(mIncrementalSaveCheckBox, 5, 0);

  //max incremental save files on disk
  mIncrementalSaveMaxSpinBox = addSpinBoxPreference(mIncrementalSaveMaxLabel,
      tr("Max Incremental Saves:"), 2, 50, 1, mIncrementalSaveMax, 0,
      mMainLayout, 6, 0);
  connect(mIncrementalSaveMaxSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_), SLOT(setIncrementalSaveMax(double)));

  //default save directory
  mSaveDirectoryLabel = new QLabel(tr("Default Save Directory"), this);
  mSaveDirectoryLineEdit = new QLineEdit(this);
  std::cout << mSaveDirectory.toLatin1().data() << "\n";
  mSaveDirectoryLineEdit->setText(mSaveDirectory);
  connect(mSaveDirectoryLineEdit, SIGNAL(textEdited(QString)), ((MainWindow*) parent_), SLOT(setSaveDirectory(QString)));
  connect(mSaveDirectoryLineEdit, SIGNAL(editingFinished()), ((MainWindow*) parent_), SLOT(checkSaveDirectory()));
  mMainLayout->addWidget(mSaveDirectoryLabel, 7, 0);
  mMainLayout->addWidget(mSaveDirectoryLineEdit, 8, 0, 1, 3);

  //default save directory for textures
  mTextureSaveDirectoryLabel = new QLabel(tr(
      "Default Texture File\nSave Directory"), this);
  mTextureSaveDirectoryLineEdit = new QLineEdit(this);
  mTextureSaveDirectoryLineEdit->setText(mTextureSaveDirectory);
  connect(mTextureSaveDirectoryLineEdit, SIGNAL(textEdited(QString)), ((MainWindow*) parent_), SLOT(setTextureSaveDirectory(QString)));
  connect(mTextureSaveDirectoryLineEdit, SIGNAL(editingFinished()), ((MainWindow*) parent_), SLOT(checkTextureSaveDirectory()));
  mMainLayout->addWidget(mTextureSaveDirectoryLabel, 9, 0);
  mMainLayout->addWidget(mTextureSaveDirectoryLineEdit, 10, 0, 1, 3);

  //command completer word typing toggle
  mCommandCompleterIndexToggle = new QCheckBox(tr(
      "Command Completer\nSingle Word Completion"), this);
  mCommandCompleterIndexToggle->setChecked(mCommandCompleterIndex);
  connect(mCommandCompleterIndexToggle, SIGNAL(stateChanged(int)), ((MainWindow*) parent_), SLOT(setCommandCompleterIndexToggle(int)));
  mMainLayout->addWidget(mCommandCompleterIndexToggle, 11, 0);

  //single click extrude word typing toggle
  mSingleClickExtrudeCheckBox
      = new QCheckBox(tr("Single Click Extrusions"), this);
  mSingleClickExtrudeCheckBox->setChecked(mSingleClickExtrude);
  connect(mSingleClickExtrudeCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*) parent_), SLOT(setSingleClickExtrude(int)));
  mMainLayout->addWidget(mSingleClickExtrudeCheckBox, 12, 0);

  //column 2 of the main tab
  //face area selection tolerance
  face_area_tolerance_spin_ = addSpinBoxPreference(face_area_tolerance_label_,
      tr("Face Area Sel.\nTolerance:"), 0.0, 5.0, 0.001, face_area_tolerance_, 3,
      mMainLayout, 0, 3);
  connect(face_area_tolerance_spin_, SIGNAL(valueChanged(double)), ((MainWindow*) parent_), SLOT(changeFaceAreaTolerance(double)));

  //valence-2 vertex split offset
  valence_2_split_offeset_spin_ = addSpinBoxPreference(
      valence_2_split_offset_label_, tr("Valence-2 Split\nOffset:"), -0.1, 5.0,
      0.001, valence_2_split_offset_, 3, mMainLayout, 1, 3);
  connect(valence_2_split_offeset_spin_, SIGNAL(valueChanged(double)), ((MainWindow*) parent_), SLOT(changeValence2SplitOffset(double)));

  mMainLayout->setRowStretch(13, 2);
  mMainLayout->setColumnStretch(5, 2);

  mMainTab->setLayout(mMainLayout);
  // mMainLayout->addWidget(mResetCameraButton,0,0);

  // persp->switchTo(VPPersp);
}

QPushButton* TopModPreferences::addColorPreference(QLabel *label, QString text,
                                                   QColor color,
                                                   QGridLayout *layout,
                                                   QStyle *style, int row,
                                                   int col) {

  label = new QLabel(text);
  QPushButton *button = new QPushButton(mColorsTab);
  button->setMaximumSize(QSize(100, 25));
  button->setAutoFillBackground(true);
  button->setStyle(style);
  QPalette p = button->palette();
  p.setColor(button->backgroundRole(), color);
  button->setPalette(p);
  layout->addWidget(label, row, col);
  layout->addWidget(button, row, col + 1);
  return button;
}

QDoubleSpinBox* TopModPreferences::addSpinBoxPreference(QLabel *label,
                                                        QString text,
                                                        double low,
                                                        double high,
                                                        double step,
                                                        double value,
                                                        int decimals,
                                                        QGridLayout *layout,
                                                        int row, int col) {
  label = new QLabel(text);
  QDoubleSpinBox *spinbox = new QDoubleSpinBox;
  spinbox->setRange(low, high);
  spinbox->setSingleStep(step);
  spinbox->setValue(value);
  spinbox->setDecimals(decimals);
  spinbox->setMaximumSize(100, 25);
  layout->addWidget(label, row, col);
  layout->addWidget(spinbox, row, col + 1);
  return spinbox;
}

void TopModPreferences::setupColors() {

  //layout for colors tab page
  QGridLayout *mColorsLayout = new QGridLayout;
  //style for the color
  QStyle* colorPickerStyle = new QPlastiqueStyle;
  QPalette p;

  //viewport background color
  mViewportColorButton = addColorPreference(mViewportColorLabel, tr(
      "Viewport Background:"), mViewportColor, mColorsLayout, colorPickerStyle,
      0, 0);
  connect(mViewportColorButton, SIGNAL(clicked()), this, SLOT(setViewportColor()));
  //object render color
  mRenderColorButton = addColorPreference(mRenderColorLabel, tr(
      "Object Render Color:"), mRenderColor, mColorsLayout, colorPickerStyle,
      1, 0);
  connect(mRenderColorButton, SIGNAL(clicked()), this, SLOT(setRenderColor()));
  //CoolLight  color
  mCoolLightColorButton = addColorPreference(mCoolLightColorLabel, tr(
      "Cool Light Color:"), mCoolLightColor, mColorsLayout, colorPickerStyle,
      2, 0);
  connect(mCoolLightColorButton, SIGNAL(clicked()), this, SLOT(setCoolLightColor()));
  //WarmLight  color
  mWarmLightColorButton = addColorPreference(mWarmLightColorLabel, tr(
      "Warm Light Color:"), mWarmLightColor, mColorsLayout, colorPickerStyle,
      3, 0);
  connect(mWarmLightColorButton, SIGNAL(clicked()), this, SLOT(setWarmLightColor()));
  //Wireframe  color
  mWireframeColorButton = addColorPreference(mWireframeColorLabel, tr(
      "Wireframe Color:"), mWireframeColor, mColorsLayout, colorPickerStyle, 4,
      0);
  connect(mWireframeColorButton, SIGNAL(clicked()), this, SLOT(setWireframeColor()));
  //Silhouette  color
  mSilhouetteColorButton = addColorPreference(mSilhouetteColorLabel, tr(
      "Silhouette Color:"), mSilhouetteColor, mColorsLayout, colorPickerStyle,
      5, 0);
  connect(mSilhouetteColorButton, SIGNAL(clicked()), this, SLOT(setSilhouetteColor()));
  //PatchBoundary  color
  mPatchBoundaryColorButton = addColorPreference(mPatchBoundaryColorLabel, tr(
      "Patch Boundary Color:"), mPatchBoundaryColor, mColorsLayout,
      colorPickerStyle, 6, 0);
  connect(mPatchBoundaryColorButton, SIGNAL(clicked()), this, SLOT(setPatchBoundaryColor()));
  //SelectedVertex  color
  mSelectedVertexColorButton = addColorPreference(mSelectedVertexColorLabel,
      tr("Selected Vertex Color:"), mSelectedVertexColor, mColorsLayout,
      colorPickerStyle, 7, 0);
  connect(mSelectedVertexColorButton, SIGNAL(clicked()), this, SLOT(setSelectedVertexColor()));
  //SelectedEdge  color
  mSelectedEdgeColorButton = addColorPreference(mSelectedEdgeColorLabel, tr(
      "Selected Edge Color:"), mSelectedEdgeColor, mColorsLayout,
      colorPickerStyle, 8, 0);
  connect(mSelectedEdgeColorButton, SIGNAL(clicked()), this, SLOT(setSelectedEdgeColor()));
  //SelectedFace  color
  mSelectedFaceColorButton = addColorPreference(mSelectedFaceColorLabel, tr(
      "Selected Face Color:"), mSelectedFaceColor, mColorsLayout,
      colorPickerStyle, 9, 0);
  connect(mSelectedFaceColorButton, SIGNAL(clicked()), this, SLOT(setSelectedFaceColor()));
  //VertexIDBg  color
  mVertexIDBgColorButton = addColorPreference(mVertexIDBgColorLabel, tr(
      "Vertex ID Bg Color:"), mVertexIDBgColor, mColorsLayout,
      colorPickerStyle, 10, 0);
  connect(mVertexIDBgColorButton, SIGNAL(clicked()), this, SLOT(setVertexIDBgColor()));
  //FaceIDBg  color
  mFaceIDBgColorButton = addColorPreference(mFaceIDBgColorLabel, tr(
      "Face ID Bg Color:"), mFaceIDBgColor, mColorsLayout, colorPickerStyle,
      11, 0);
  connect(mFaceIDBgColorButton, SIGNAL(clicked()), this, SLOT(setFaceIDBgColor()));
  //EdgeIDBg  color
  mEdgeIDBgColorButton = addColorPreference(mEdgeIDBgColorLabel, tr(
      "Edge ID Bg Color:"), mEdgeIDBgColor, mColorsLayout, colorPickerStyle,
      12, 0);
  connect(mEdgeIDBgColorButton, SIGNAL(clicked()), this, SLOT(setEdgeIDBgColor()));
  //Normal  color
  mNormalColorButton
      = addColorPreference(mNormalColorLabel, tr("Normal Color:"),
          mNormalColor, mColorsLayout, colorPickerStyle, 13, 0);
  connect(mNormalColorButton, SIGNAL(clicked()), this, SLOT(setNormalColor()));
  //FaceCentroid  color
  mFaceCentroidColorButton = addColorPreference(mFaceCentroidColorLabel, tr(
      "Face Centroid Color:"), mFaceCentroidColor, mColorsLayout,
      colorPickerStyle, 14, 0);
  connect(mFaceCentroidColorButton, SIGNAL(clicked()), this, SLOT(setFaceCentroidColor()));

  //new column
  //light intensity
  mLightIntensitySpinBox = addSpinBoxPreference(mLightIntensityLabel, tr(
      "Light Intensity:"), 0.0, 10.0, 0.1, mLightIntensity, 1, mColorsLayout,
      0, 2);
  connect(mLightIntensitySpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setLightIntensity(double)));
  //wireframe thickness
  mWireframeThicknessSpinBox = addSpinBoxPreference(mWireframeThicknessLabel,
      tr("Wireframe Thickness:"), 0.1, 30.0, 0.1, mWireframeThickness, 1,
      mColorsLayout, 1, 2);
  connect(mWireframeThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setWireframeThickness(double)));
  //vertex thickness
  mVertexThicknessSpinBox
      = addSpinBoxPreference(mVertexThicknessLabel, tr("Vertex Size:"), 0.1,
          10.0, 0.5, mVertexThickness, 1, mColorsLayout, 2, 2);
  connect(mVertexThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setVertexThickness(double)));
  //silhouette thickness
  mSilhouetteThicknessSpinBox = addSpinBoxPreference(mSilhouetteThicknessLabel,
      tr("Silhouette Thickness:"), 0.1, 15.0, 0.5, mSilhouetteThickness, 1,
      mColorsLayout, 3, 2);
  connect(mSilhouetteThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setSilhouetteThickness(double)));
  //SelectedVertex thickness
  mSelectedVertexThicknessSpinBox = addSpinBoxPreference(
      mSelectedVertexThicknessLabel, tr("Selected Vertex Size:"), 0.1, 15.0,
      0.5, mSelectedVertexThickness, 1, mColorsLayout, 4, 2);
  connect(mSelectedVertexThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setSelectedVertexThickness(double)));
  //SelectedEdge thickness
  mSelectedEdgeThicknessSpinBox = addSpinBoxPreference(
      mSelectedEdgeThicknessLabel, tr("Selected Edge Thickness:"), 0.1, 15.0,
      0.5, mSelectedEdgeThickness, 1, mColorsLayout, 5, 2);
  connect(mSelectedEdgeThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setSelectedEdgeThickness(double)));
  //Normal Size
  mNormalThicknessSpinBox = addSpinBoxPreference(mNormalThicknessLabel, tr(
      "Normal Thickness:"), 0.1, 15.0, 0.5, mNormalThickness, 1, mColorsLayout,
      6, 2);
  connect(mNormalThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setNormalThickness(double)));
  //SelectedEdge thickness
  mNormalLengthSpinBox = addSpinBoxPreference(mNormalLengthLabel, tr(
      "Normal Length:"), 0.1, 15.0, 0.5, mNormalLength, 1, mColorsLayout, 7, 2);
  connect(mNormalLengthSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setNormalLength(double)));
  //SelectedEdge thickness
  mFaceCentroidThicknessSpinBox = addSpinBoxPreference(
      mFaceCentroidThicknessLabel, tr("Face Centroid Thickness:"), 0.1, 15.0,
      0.5, mFaceCentroidThickness, 1, mColorsLayout, 8, 2);
  connect(mFaceCentroidThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*) parent_)->getActive(), SLOT(setFaceCentroidThickness(double)));

  //reset button
  mResetColorsButton = new QPushButton(tr("Reset"));
  connect(mResetColorsButton, SIGNAL(clicked()), this, SLOT(loadDefaults()));
  mColorsLayout->addWidget(mResetColorsButton, 14, 3);

  mColorsLayout->setRowStretch(15, 17);
  mColorsLayout->setColumnStretch(4, 10);

  // mColorsLayout->addStretch(1);
  mColorsTab->setLayout(mColorsLayout);

}

void TopModPreferences::setViewportColor() {
  mViewportColor.setRgba(QColorDialog::getRgba(mViewportColor.rgba()));
  if (mViewportColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setViewportColor(mViewportColor);
    setButtonColor(mViewportColor, mViewportColorButton);
  }
}

void TopModPreferences::setRenderColor() {
  mRenderColor.setRgba(QColorDialog::getRgba(mRenderColor.rgba()));
  if (mRenderColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setRenderColor(mRenderColor);
    setButtonColor(mRenderColor, mRenderColorButton);
  }
}

void TopModPreferences::setCoolLightColor() {
  mCoolLightColor.setRgba(QColorDialog::getRgba(mCoolLightColor.rgba()));
  if (mCoolLightColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setCoolLightColor(mCoolLightColor);
    setButtonColor(mCoolLightColor, mCoolLightColorButton);
  }
}

void TopModPreferences::setWarmLightColor() {
  mWarmLightColor.setRgba(QColorDialog::getRgba(mWarmLightColor.rgba()));
  if (mWarmLightColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setWarmLightColor(mWarmLightColor);
    setButtonColor(mWarmLightColor, mWarmLightColorButton);
  }
}

void TopModPreferences::setWireframeColor() {
  mWireframeColor.setRgba(QColorDialog::getRgba(mWireframeColor.rgba()));
  if (mWireframeColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setWireframeColor(mWireframeColor);
    setButtonColor(mWireframeColor, mWireframeColorButton);
  }
}

void TopModPreferences::setSilhouetteColor() {
  mSilhouetteColor.setRgba(QColorDialog::getRgba(mSilhouetteColor.rgba()));
  if (mSilhouetteColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setSilhouetteColor(mSilhouetteColor);
    setButtonColor(mSilhouetteColor, mSilhouetteColorButton);
  }
}

void TopModPreferences::setPatchBoundaryColor() {
  mPatchBoundaryColor.setRgba(QColorDialog::getRgba(mPatchBoundaryColor.rgba()));
  if (mPatchBoundaryColor.isValid()) {
    // ((MainWindow*)parent_)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
    setButtonColor(mPatchBoundaryColor, mPatchBoundaryColorButton);
  }
}

void TopModPreferences::setSelectedVertexColor() {
  mSelectedVertexColor.setRgba(QColorDialog::getRgba(
      mSelectedVertexColor.rgba()));
  if (mSelectedVertexColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setSelectedVertexColor(
        mSelectedVertexColor);
    setButtonColor(mSelectedVertexColor, mSelectedVertexColorButton);
  }
}

void TopModPreferences::setSelectedEdgeColor() {
  mSelectedEdgeColor.setRgba(QColorDialog::getRgba(mSelectedEdgeColor.rgba()));
  if (mSelectedEdgeColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setSelectedEdgeColor(
        mSelectedEdgeColor);
    setButtonColor(mSelectedEdgeColor, mSelectedEdgeColorButton);
  }
}

void TopModPreferences::setSelectedFaceColor() {
  mSelectedFaceColor.setRgba(QColorDialog::getRgba(mSelectedFaceColor.rgba()));
  if (mSelectedFaceColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setSelectedFaceColor(
        mSelectedFaceColor);
    setButtonColor(mSelectedFaceColor, mSelectedFaceColorButton);
  }
}

void TopModPreferences::setFaceIDBgColor() {
  mFaceIDBgColor.setRgba(QColorDialog::getRgba(mFaceIDBgColor.rgba()));
  if (mFaceIDBgColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
    setButtonColor(mFaceIDBgColor, mFaceIDBgColorButton);
  }
}

void TopModPreferences::setVertexIDBgColor() {
  mVertexIDBgColor.setRgba(QColorDialog::getRgba(mVertexIDBgColor.rgba()));
  if (mVertexIDBgColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
    setButtonColor(mVertexIDBgColor, mVertexIDBgColorButton);
  }
}

void TopModPreferences::setEdgeIDBgColor() {
  mEdgeIDBgColor.setRgba(QColorDialog::getRgba(mEdgeIDBgColor.rgba()));
  if (mEdgeIDBgColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
    setButtonColor(mEdgeIDBgColor, mEdgeIDBgColorButton);
  }
}

void TopModPreferences::setNormalColor() {
  mNormalColor.setRgba(QColorDialog::getRgba(mNormalColor.rgba()));
  if (mNormalColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setNormalColor(mNormalColor);
    setButtonColor(mNormalColor, mNormalColorButton);
  }
}

void TopModPreferences::setFaceCentroidColor() {
  mFaceCentroidColor.setRgba(QColorDialog::getRgba(mFaceCentroidColor.rgba()));
  if (mFaceCentroidColor.isValid()) {
    ((MainWindow*) parent_)->getActive()->setFaceCentroidColor(
        mFaceCentroidColor);
    setButtonColor(mFaceCentroidColor, mFaceCentroidColorButton);
  }
}

void TopModPreferences::setSaveDirectory(QString s) {
  mSaveDirectory = s;
  mSaveDirectoryLineEdit->setText(s);
}

void TopModPreferences::setTextureSaveDirectory(QString s) {
  mTextureSaveDirectory = s;
  mTextureSaveDirectoryLineEdit->setText(s);
}
