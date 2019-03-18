#include <stdio.h>

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
            if(0 != rename(replacement.toStdString().c_str(),
                   replaced.toStdString().c_str()))
            {
                if(pError)
                    *pError = QString::fromStdString(strerror(errno));
                return false;
            }
            return true;
        }

        if(!backup.isEmpty())
        {
            // first rename(replaced, backup)
            if(0 != rename(replaced.toStdString().c_str(), backup.toStdString().c_str()))
            {
                if(pError)
                    *pError = QString::fromStdString(strerror(errno));
                return false;
            }
        }

        // Second, rename(replacement,replaced)
        if(0 != rename(replacement.toStdString().c_str(),
               replaced.toStdString().c_str()))
        {
            if(pError)
                *pError = QString::fromStdString(strerror(errno));
            return false;
        }
        return true;
    }
}
