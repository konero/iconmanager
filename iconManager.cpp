#include "iconManager.h"
#include <QDirIterator>

IconManager::IconManager() {
    // Constructor implementation
    // Initialize iconMap or perform any other necessary initialization
}


void IconManager::loadIconsFromResource(const QString& resourcePath) {
    QDirIterator it(resourcePath, QStringList() << "*.svg" << "*.png", QDir::Files, QDirIterator::Subdirectories);

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

QPixmap IconManager::recolorPixmap(const QPixmap &pixmap, const QColor &color) {
  // Recolor black pixels
  QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
  QRgb targetColor = color.rgb();
  int height = img.height();
  int width = img.width();

  for (int y = 0; y < height; ++y) {
    QRgb *pixel = reinterpret_cast<QRgb *>(img.scanLine(y));
    QRgb *end = pixel + width;
    for (; pixel != end; ++pixel) {
      if (qGray(*pixel) == 0) {
        *pixel = (targetColor & 0x00FFFFFF) | (qAlpha(*pixel) << 24);
      }
    }
  }
  return QPixmap::fromImage(img);
}
