#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QMap>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QColor>

class IconManager {
public:
    IconManager();
    void loadIconsFromResource(const QString& resourcePath);
    QIcon getIcon(const QString& filename);

private:
    QMap<QString, QIcon> iconMap;
    QPixmap recolorPixmap(const QPixmap &pixmap, const QColor &color);
};

#endif
