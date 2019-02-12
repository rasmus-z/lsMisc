#ifndef TESTSTDQT_H
#define TESTSTDQT_H

#include <QObject>

class TestStdQt: public QObject
{
    Q_OBJECT

private slots:
    void test_rstrip();
    void test_pathCombine();
    void test_normalizeDir();
    void test_doublequoteIfNecessary();
    void test_undoublequoteIfNecessary();
};

#endif // TESTSTDQT_H
