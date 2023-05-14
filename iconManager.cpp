#include <QPainter>
#include <QSvgRenderer>
#include <QDirIterator>
#include "iconManager.h"

IconManager::IconManager() {
}

void IconManager::loadIconsFromResource(const QString& resourcePath)
{
    QDirIterator it(resourcePath, QStringList() << "*.svg", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();
        
        QSvgRenderer renderer(it.filePath());
        QPixmap pixmap(renderer.defaultSize());
        pixmap.fill(Qt::transparent); // Ensure transparent background

        QPainter painter(&pixmap);
        renderer.render(&painter);

        QPixmap recoloredPixmap = recolorPixmap(pixmap, Qt::red);
        QIcon icon(recoloredPixmap);
        QString iconName = it.fileInfo().baseName();
        iconMap.insert(iconName, icon);
    }
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

QIcon IconManager::getIcon(const QString& filename) {
    return iconMap.value(filename);
}
