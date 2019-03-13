
#ifndef STDQT_H
#define STDQT_H

#include <QString>

namespace AmbiesoftQt {

QString rstrip(const QString& str, QChar c);
QString pathCombine(const QString& path1, const QString& path2);
QString pathCombine(const QString& path1, const QString& path2, const QString& path3);
QString normalizeDir(const QString& dir);
QString doublequoteIfNecessary(const QString& s);
QString undoublequoteIfNecessary(QString s);
bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError = nullptr);
}

#endif // STDQT_H
