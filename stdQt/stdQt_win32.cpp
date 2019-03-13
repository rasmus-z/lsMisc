#include <Windows.h>

#include <QString>
#include <QDir>

#include "stdQt.h"

namespace AmbiesoftQt {

    bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError)
    {
        Q_ASSERT(!replaced.isEmpty());
        Q_ASSERT(!replacement.isEmpty());

        // maybe first save, original file not exist
        if(!QFile(replaced).exists())
        {
            if(!MoveFileW(replacement.toStdWString().c_str(),
                      replaced.toStdWString().c_str()))
            {
                if(pError)
                    *pError = QString::fromStdWString(std::to_wstring(GetLastError()));
                return false;
            }
            return true;
        }
        if(!ReplaceFile(replaced.toStdWString().c_str(),
                    replacement.toStdWString().c_str(),
                    backup.isEmpty() ? nullptr : backup.toStdWString().c_str(),
                    0, // flag
                    nullptr,nullptr // reserved
                    ))
        {
            if(pError)
                *pError = QString::fromStdWString(std::to_wstring(GetLastError()));
            return false;
        }
        return true;
    }
}
