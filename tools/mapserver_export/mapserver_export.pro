######################################################################
# Automatically generated by qmake (2.01a) Mon Dec 11 12:01:35 2006
######################################################################

include(../../settings.pro)
TEMPLATE = app
TARGET = ms_export
DEPENDPATH += .
INCLUDEPATH += . $${PYTHONINCLUDE}
LIBS += $${PYTHONLIBADD}
LIBS += $${QGISCORELIBADD}
QT += qt3support svg core gui xml network
# Input
HEADERS += qgsmapserverexport.h
FORMS += qgsmapserverexportbase.ui
SOURCES += main.cpp msexport_wrap.cxx qgsmapserverexport.cpp
