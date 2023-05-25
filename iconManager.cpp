#include "iconManager.h"
#include <QDirIterator>

IconManager::IconManager() {
  currentIconMap = &iconMapDark;  // default theme
}

void IconManager::loadIconsFromResource(const QString &resourcePath) {
  QDirIterator it(resourcePath,
                  QStringList() << "*.svg"
                                << "*.png",
                  QDir::Files, QDirIterator::Subdirectories);

  while (it.hasNext()) {
    it.next();
    QString iconName = it.fileInfo().baseName();
    iconPaths.insert(iconName, it.filePath());

    QPixmap pixmap(it.filePath());

    QIcon iconLight(recolorPixmap(pixmap, QColor(Qt::blue)));
    QIcon iconDark(recolorPixmap(pixmap, QColor(Qt::red)));

    iconMapLight.insert(iconName, iconLight);
    iconMapDark.insert(iconName, iconDark);
  }
}

QIcon IconManager::getIcon(const QString &filename) const {
  return currentIconMap->value(filename);
}

const QMap<QString, QIcon> &IconManager::getIconMap() const {
  return *currentIconMap;
}

void IconManager::switchTheme(bool isDarkTheme) {
  currentIconMap = isDarkTheme ? &iconMapDark : &iconMapLight;
  emit themeChanged();
}

const QMap<QString, QString> &IconManager::getIconPaths() const {
  return iconPaths;
}

QPixmap IconManager::recolorPixmap(const QPixmap &pixmap, const QColor &color) {
  // repaints black pixels
  QImage img       = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
  QRgb targetColor = color.rgb();
  int height       = img.height();
  int width        = img.width();

  for (int y = 0; y < height; ++y) {
    QRgb *pixel = reinterpret_cast<QRgb *>(img.scanLine(y));
    QRgb *end   = pixel + width;
    for (; pixel != end; ++pixel) {
      if (qGray(*pixel) == 0) {
        *pixel = (targetColor & 0x00FFFFFF) | (qAlpha(*pixel) << 24);
      }
    }
  }
  return QPixmap::fromImage(img);
}
