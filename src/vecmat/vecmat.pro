message (Compiler = $$QMAKE_CXX)
# QMAKE_CXX=
TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib #dll # build shared library
# CONFIG += debug warn_off create_prl
CONFIG += release warn_off create_prl
TARGET = vecmat
DESTDIR = ../../lib

macx {
 # compile release + universal binary
 CONFIG += x86 ppc
 #CONFIG += lib_bundle
 #QMAKE_BUNDLE_EXTENSION = .framework
}

HEADERS += \
	Matrix3x3.h \
	Matrix4x4.h \
	Quaternion.h \
	Vector.h \
	Vector2d.h \
	Vector3d.h \
	Vector4d.h

SOURCES += \
	Matrix3x3.cc \
	Matrix4x4.cc \
	Vector.cc \
	Vector2d.cc \
	Vector3d.cc \
	Vector4d.cc
