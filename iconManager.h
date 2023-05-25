#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QMap>
#include <QString>
#include <QIcon>
#include <QObject>
#include <QToolButton>

class IconManager : public QObject {
  Q_OBJECT

public:
  IconManager();

  void loadIconsFromResource(const QString& resourcePath);
  QIcon getIcon(const QString& filename) const;
  const QMap<QString, QIcon>& getIconMap() const;
  const QMap<QString, QString>& getIconPaths() const;
  static QPixmap recolorPixmap(const QPixmap& pixmap, const QColor& color);

signals:
  void themeChanged();

public slots:
  void switchTheme(bool isDarkTheme);

private:
  QMap<QString, QIcon> iconMapLight;
  QMap<QString, QIcon> iconMapDark;
  QMap<QString, QIcon>* currentIconMap;
  QMap<QString, QString> iconPaths;
};

class TToolButton : public QToolButton {
  Q_OBJECT

public:
  TToolButton(IconManager* iconManager, const QString& iconName, QWidget* parent = nullptr)
    : QToolButton(parent), m_iconManager(iconManager), m_iconName(iconName) {
    setIcon(iconManager->getIcon(iconName));
    connect(iconManager, &IconManager::themeChanged, this, &TToolButton::updateIcon);
  }

public slots:
  void updateIcon() {
    setIcon(m_iconManager->getIcon(m_iconName));
  }

private:
  IconManager* m_iconManager;
  QString m_iconName;
};

#endif  // ICONMANAGER_H
