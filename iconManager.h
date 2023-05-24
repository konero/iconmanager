#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QMap>
#include <QString>
#include <QIcon>

class IconManager {
public:
  IconManager();

  void loadIconsFromResource(const QString& resourcePath);
  QIcon getIcon(const QString& filename) const;
  const QMap<QString, QIcon>& getIconMap() const;
  const QMap<QString, QString>& getIconPaths() const;
  static QPixmap recolorPixmap(const QPixmap& pixmap, const QColor& color);
  void switchTheme(bool isDarkTheme);

private:
  QMap<QString, QIcon> iconMapLight;
  QMap<QString, QIcon> iconMapDark;
  QMap<QString, QIcon>* currentIconMap;
  QMap<QString, QString> iconPaths;
};

#endif  // ICONMANAGER_H
