TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
		googletest/googletest \
		googletest/googletest/include \


SOURCES += \
        main.cpp \
    googletest/googletest/src/gtest-all.cc \
    gtestStdosd.cpp
