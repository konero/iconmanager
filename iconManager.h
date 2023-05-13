#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QMap>
#include <QString>
#include <QIcon>

class IconManager
{
public:
    IconManager();
    void loadIconsFromResource(const QString& resourcePath);
    QIcon getIcon(const QString& filename);

private:
    QMap<QString, QIcon> iconMap;
};

#endif
