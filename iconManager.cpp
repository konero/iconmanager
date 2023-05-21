#include "iconManager.h"
#include <QDirIterator>

IconManager::IconManager() {
    // Constructor implementation
    // Initialize iconMap or perform any other necessary initialization
}

void IconManager::loadIconsFromResource(const QString& resourcePath) {
    QDirIterator it(resourcePath, QStringList() << "*.svg", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QIcon icon(it.filePath());
        QString iconName = it.fileInfo().baseName();
        iconMap.insert(iconName, icon);
        iconPaths.insert(iconName, it.filePath());
    }
}

QIcon IconManager::getIcon(const QString& filename) const {
    return iconMap.value(filename);
}

const QMap<QString, QIcon>& IconManager::getIconMap() const {
    return iconMap;
}

const QMap<QString, QString>& IconManager::getIconPaths() const {
    return iconPaths;
}
