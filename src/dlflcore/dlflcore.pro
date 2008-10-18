TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib #dll # build shared library
CONFIG += release warn_off create_prl
# CONFIG += debug warn_off create_prl
TARGET = dlflcore
INCLUDEPATH += ../include ../vecmat
DESTDIR = ../../lib

macx {
 # compile release + universal binary
 #QMAKE_LFLAGS += -F../../lib
 #LIBS += -framework vecmat
 CONFIG += x86 ppc
 #CONFIG += lib_bundle
 #QMAKE_BUNDLE_EXTENSION = .framework
#} else:unix {
 QMAKE_LFLAGS += -L../../lib
 #LIBS += -lvecmat
 QMAKE_CXXFLAGS_DEBUG += -pg
 QMAKE_LFLAGS_DEBUG += -pg
}
else:win32 {

}

HEADERS +=  \
          	DLFLCommon.h \
          	DLFLCore.h \
          	DLFLCoreExt.h \
          	DLFLEdge.h \
          	DLFLFace.h \
          	DLFLFaceVertex.h \
          	DLFLMaterial.h \
          	DLFLObject.h \
          	DLFLVertex.h

SOURCES +=  \
          	DLFLCommon.cc \
          	DLFLCore.cc \
          	DLFLCoreExt.cc \
          	DLFLEdge.cc \
          	DLFLFace.cc \
          	DLFLFaceVertex.cc \
          	DLFLFile.cc \
            DLFLFileAlt.cc \
          	DLFLObject.cc \
          	DLFLVertex.cc
