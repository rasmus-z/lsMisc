#ifndef TESTQSTRING_H
#define TESTQSTRING_H

#include <QObject>

class TestQString: public QObject
{
    Q_OBJECT

private slots:
    void toUpper_data();
    void toUpper();
};


#endif // TESTQSTRING_H
