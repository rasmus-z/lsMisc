TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
		googletest/googletest \
		googletest/googletest/include \


SOURCES += \
        main.cpp \
    googletest/googletest/src/gtest-all.cc \
    gtestStdosd.cpp \
    gtestOpParser.cpp \
    ../stdosd/stdosd.cpp \
    ../stdosd/stdosd_win.cpp \
    gtestOpParser.cpp \
    gtestPathUtil_win.cpp \


HEADERS += \
    ../stdosd/OpParser.h \
    ../stdosd/ResolveLink.h \
    ../stdosd/PathUtil.h

win32 {
   SOURCES += ../stdosd/ResolveLink_win.cpp \
	../stdosd/PathUtil_win.cpp

    win32-g++ {
        LIBS += -lOle32 -lShlwapi -luuid
    }
    win32-msvc* {
        LIBS += Ole32.lib Shlwapi.lib uuid.lib
    }
}
linux-g++ {
    SOURCES += ../stdosd/ResolveLink_linux.cpp
}
