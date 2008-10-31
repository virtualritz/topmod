# version info : code from -- http://wiki.qtcentre.org/index.php?title=Version_numbering_using_QMake
VERSION = $$system(svn info -r HEAD . | grep 'Changed\ Rev' | cut -b 19-)
!isEmpty(VERSION)
 { 
    VERSION = 3.0.$${VERSION}
    VERSTR = '\\"$${VERSION}\\"' # place quotes around the version string
    DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string
}

# qt version
QTVERSION = $$[QT_VERSION]
QT_VERSTR = '\\"$${QTVERSION}\\"' # place quotes around the version string
DEFINES += QT_VER=\"$${QT_VERSTR}\" # create a QT_VER macro containing the version string

# main stuff
QT += opengl \
    xml
CONFIG += qt \
    warn_off \
    link_prl \
    release 
    #debug

# Fenghui: disable pg option
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_CXXFLAGS += -fpermissive
QMAKE_LFLAGS_DEBUG += -pg

# exclude python or spacenav drivers
# or include them with CONFIG +=
CONFIG -= WITH_PYTHON \
    WITH_SPACENAV

# CONFIG += WITH_PYTHON
# to include the popup command line interface leave the following line uncommented
DEFINES *= QCOMPLETER

# DEFINES += TOPMOD_VERSION
DEFINES -= GPU_OK
TEMPLATE = app
SUBDIRS *= include
MOC_DIR = tmp
OBJECTS_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

# TopModd will be the name for the debug version,
# and TopMod will be the release version
CONFIG(debug, debug|release) { 
	TARGET = TopModd # -$${VERSION}
	DESTDIR = ../bin/debug/
}
else { 
	TARGET = TopMod # -$${VERSION}
	DESTDIR = ../bin/release/
}
	
DEPENDPATH += lang \
    include \
    vecmat \
    dlflcore \
    dlflaux \
    arcball
INCLUDEPATH += include \
    vecmat \
    arcball \
    dlflcore \
    dlflaux 
CONFIG(WITH_PYTHON) { 
    message("PYTHON support will be included")
    DEFINES *= WITH_PYTHON
}


# Operating System Specific Tasks - OS/X
macx { 
    # dave - testing the macx / linux / win32 define
    DEFINES *= MACX
    
    # mac icon when not using a custom info.plist file
    ICON = topmod.icns
    
    # either compile the mac version as an app bundle or a console app
    # tell it to load a custom info.plist file here
    QMAKE_INFO_PLIST = Info.plist
    MACOSX_DEPLOYMENT_TARGET = 10.2
    
    # compile release + universal binary
    CONFIG += x86 \
        ppc
    CONFIG(GPU_OK) { # for cg gpu shading
        INCLUDEPATH += /Library/Frameworks/Cg.framework/Versions/1.0
        QMAKE_LFLAGS += -L/Library/Frameworks/Cg.framework
        LIBS += -framework \
            Cg
    }
    
    # QMAKE_LFLAGS += -F./lib
    # LIBS += -framework vecmat -framework dlflcore -framework dlflaux
    QMAKE_LFLAGS += -L../lib
    LIBS += -lvecmat \
        -ldlflcore \
        -ldlflaux \
        -larcball \
        -framework \
        CoreFoundation
    
    # PRIVATE_FRAMEWORKS.files = ../lib/vecmat.framework ../lib/dlflcore.framework ../lib/dlflaux.framework
    # PRIVATE_FRAMEWORKS.path = Contents/Frameworks
    # QMAKE_BUNDLE_DATA += PRIVATE_FRAMEWORKS
    INCLUDEPATH += /usr/include
    QMAKE_LFLAGS += -L/usr/lib
    CONFIG(WITH_PYTHON) { 
        INCLUDEPATH += /Library/Frameworks/Python.framework/Versions/2.5/include/python2.5
        QMAKE_LFLAGS += -L/Library/Frameworks/Python.framework
        LIBS += -framework \
            Python
    }
}

# CONFIG(WITH_SPACENAV) {
# INCLUDEPATH += /Library/Frameworks/3DconnexionClient.framework/Versions/A/Headers
# LIBS += -framework \
# 3DconnexionClient
# QMAKE_LFLAGS += -L/Library/Frameworks/3DconnexionClient.framework
# }
# Operating System Specific Tasks - Linux
else:unix { 
    # dave - testing the macx / linux / win32 define
    DEFINES *= UNIX
    QMAKE_LFLAGS += -L../lib
    LIBS += -lvecmat \
        -ldlflcore \
        -ldlflaux \
        -larcball
    DEFINES *= LINUX
    CONFIG(WITH_PYTHON) { 
        INCLUDEPATH += /usr/include/python2.5
        LIBS += -lpython2.5 \
            -L/usr/lib/python2.5/config
    }
}

# Operating System Specific Tasks - Windows
else:win32 { 
    # dave - testing the macx / linux / win32 define
    DEFINES *= WIN32
    
    # application icon windows
    RC_FILE = topmod.rc
    INCLUDEPATH += ../lib include
    QMAKE_LFLAGS += -L../lib
    LIBS += -lvecmat -ldlflcore -ldlflaux -larcball
    
    # works with Python25
    CONFIG(WITH_PYTHON) { 
        INCLUDEPATH += C:/Python25/include
        QMAKE_LFLAGS += -LC:/Python25/libs
        LIBS += -lpython25
    }
}

# Input
HEADERS += \
		tools/insert_edge_tool.h \
		tools/delete_edge_tool.h \
		tools/collapse_edge_tool.h \
		tools/connect_edges_tool.h \
		tools/subdivide_edge_tool.h \
		tools/splice_corners_tool.h \
    tools/topmod_tool.h \
    tools/geometric_tool.h \
    tools/dup_component_tool.h \
    tools/create_torus_tool.h \
		tools/create_crust_tool.h \
    DLFLScriptEditor.h \
    TopModPreferences.h \
    TdxDeviceWrappers.h \
    CommandCompleter.h \
    DLFLLocator.h \
    GLWidget.h \
    TopMod.h \
    MainWindow.h \
    GeometryRenderer.h \
    DLFLLighting.h \
    qcumber.h \
    qshortcutdialog.h \
    qshortcutmanager.h \
    ui_shortcutdialog.h \
    ui_stylesheeteditor.h \
    editor.h \
    PythonHighlighter.h \
    BasicsMode.h \
    ExtrusionsMode.h \
    RemeshingMode.h \
    ConicalMode.h \
    HighgenusMode.h \
    TexturingMode.h \
    ExperimentalModes.h \
    DLFLSelection.h \
    #Viewport.h \
    TMPatchFace.h \
    TMPatchObject.h \
    TMPatch.h \
    DLFLRenderer.h \
    hermite_connect_faces.h \
    stylesheeteditor.h \
    include/BaseObject.h \
    include/Constants.h \
    include/Inlines.h \
    include/StreamIO.h \
    include/Color.h \
    include/Grid.h \
    include/Texture.h \
    include/Transform.h \
    include/AmbientLight.h \
    include/Light.h \
    include/PointLight.h \
    include/SpotLight.h \
    CgData.h \
    include/Camera3.h \
	vecmat/Matrix4x4.h
	
FORMS += shortcutdialog.ui \
    stylesheeteditor.ui
SOURCES += tools/insert_edge_tool.cc \
		tools/delete_edge_tool.cc \
		tools/collapse_edge_tool.cc \
		tools/connect_edges_tool.cc \
		tools/subdivide_edge_tool.cc \
		tools/splice_corners_tool.cc \
    tools/topmod_tool.cc \
    tools/dup_component_tool.cc \
    tools/geometric_tool.cc \
    tools/create_torus_tool.cc \
		tools/create_crust_tool.cc \
    DLFLScriptEditor.cc \
    TopModPreferences.cc \
    TdxDeviceWrappers.cc \
    GLWidget.cc \
    TopMod.cc \
    main.cc \
    MainWindow.cc \
    MainWindowCallbacks.cc \
    MainWindowRemeshingCallbacks.cc \
    GeometryRenderer.cc \
    qshortcutdialog.cc \
    qshortcutmanager.cc \
    editor.cc \
    PythonHighlighter.cc \
    BasicsMode.cc \
    ExtrusionsMode.cc \
    RemeshingMode.cc \
    ConicalMode.cc \
    HighgenusMode.cc \
    TexturingMode.cc \
    ExperimentalModes.cc \
    DLFLLighting.cc \
    DLFLRenderer.cc \
    hermite_connect_faces.cc \
    DLFLSelection.cc \
    DLFLUndo.cc \
    DLFLLocator.cc \
    TMPatchObject.cc \
    TMPatchFace.cc \
    stylesheeteditor.cc \
    CgData.cc \
    include/Camera3.cc \
    CommandCompleter.cc \
	vecmat/Matrix4x4.cc
	
RESOURCES += application.qrc
TRANSLATIONS += lang/topmod_de.ts \
    lang/topmod_it.ts \
    lang/topmod_en.ts \
    lang/topmod_ca.ts \
    lang/topmod_es.ts \
    lang/topmod_fr.ts \
    lang/topmod_hi.ts \
    lang/topmod_tr.ts
