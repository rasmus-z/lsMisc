#include <QString>
#include <QDir>


QString rstrip(const QString& str, QChar c)
{
    int n = str.size() - 1;
    for (; n >= 0; --n)
    {
        if (str.at(n) != c)
        {
            return str.left(n + 1);
        }
    }
    return "";
}

QString pathCombine(const QString& path1, const QString& path2)
{
    if(QDir::isAbsolutePath(path2))
        return path2;

    return rstrip(path1, '/') + '/' + rstrip(path2, '/');
}

QString normalizeDir(const QString& dir)
{
    if (!dir.endsWith('/'))
        return dir + '/';
    return rstrip(dir, '/') + '/';
}
