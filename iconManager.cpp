#include "iconManager.h"
#include <QDirIterator>

IconManager::IconManager()
{
}

void IconManager::loadIconsFromResource(const QString& resourcePath)
{
    QDirIterator it(resourcePath, QStringList() << "*.svg", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();
        QIcon icon(it.filePath());
        QString iconName = it.fileInfo().baseName();
        iconMap.insert(iconName, icon);
    }
}

QIcon IconManager::getIcon(const QString& filename)
{
    return iconMap.value(filename);
}
