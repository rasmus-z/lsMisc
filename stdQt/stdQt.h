
#ifndef STDQT_H
#define STDQT_H

#include <QString>

QString rstrip(const QString& str, QChar c);
QString pathCombine(const QString& path1, const QString& path2);
QString pathCombine(const QString& path1, const QString& path2, const QString& path3);
QString normalizeDir(const QString& dir);

#endif // STDQT_H
