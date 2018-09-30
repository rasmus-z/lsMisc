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




HEADERS += \
    ../stdosd/OpParser.h \
    ../stdosd/PathUtil.h \
    ../stdosd/stdosd.h

win32 {
   SOURCES += \
        ../stdosd/PathUtil_win.cpp \
        gtestPathUtil_win.cpp \
        ../stdosd/stdosd_win.cpp \

    win32-g++ {
        LIBS += -lOle32 -lShlwapi -luuid
    }
    win32-msvc* {
        LIBS += Ole32.lib Shlwapi.lib uuid.lib
    }
}
linux-g++ {
    SOURCES += \
        ../stdosd/PathUtil_linux.cpp \
        gtestPathUtil_linux.cpp \
        ../stdosd/stdosd_linux.cpp \

    LIBS += -lpthread

}
