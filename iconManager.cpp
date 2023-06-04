#include <QImage>
#include <QSvgRenderer>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QDebug>
#include "iconManager.h"

int getHighestDevicePixelRatio() {
  static int highestDevPixRatio = 0;
  if (highestDevPixRatio == 0) {
    for (auto screen : QGuiApplication::screens())
      highestDevPixRatio =
          std::max(highestDevPixRatio, (int)screen->devicePixelRatio());
  }
  return highestDevPixRatio;
}

Utils::Utils() {}

SvgRenderParams Utils::calculateSvgRenderParams(const QSize &desiredSize,
                                         QSize &imageSize,
                                         Qt::AspectRatioMode aspectRatioMode) {
  SvgRenderParams params;
  if (desiredSize.isEmpty()) {
    params.size = imageSize;
    params.rect = QRectF(QPointF(), QSizeF(params.size));
  } else {
    params.size = desiredSize;
    if (aspectRatioMode == Qt::KeepAspectRatio ||
        aspectRatioMode == Qt::KeepAspectRatioByExpanding) {
      QPointF scaleFactor(
          (float)params.size.width() / (float)imageSize.width(),
          (float)params.size.height() / (float)imageSize.height());
      float factor = (aspectRatioMode == Qt::KeepAspectRatio)
                         ? std::min(scaleFactor.x(), scaleFactor.y())
                         : std::max(scaleFactor.x(), scaleFactor.y());
      QSizeF renderSize(factor * (float)imageSize.width(),
                        factor * (float)imageSize.height());
      QPointF topLeft(
          ((float)params.size.width() - renderSize.width()) * 0.5f,
          ((float)params.size.height() - renderSize.height()) * 0.5f);
      params.rect = QRectF(topLeft, renderSize);
    } else {  // Qt::IgnoreAspectRatio:
      params.rect = QRectF(QPointF(), QSizeF(params.size));
    }
  }
  return params;
}

QImage Utils::svgToImage(const QString &svgFilePath, const QSize &size,
                  Qt::AspectRatioMode aspectRatioMode, QColor bgColor) {
  QSvgRenderer svgRenderer(svgFilePath);

  // Check if SVG file was loaded correctly
  if (!svgRenderer.isValid()) {
    qWarning() << "Invalid SVG file:" << svgFilePath;
    return QImage();
  }

  static int devPixRatio = getHighestDevicePixelRatio();

  QSize imageSize = svgRenderer.defaultSize() * devPixRatio;
  SvgRenderParams params =
      calculateSvgRenderParams(size, imageSize, aspectRatioMode);
  QImage image(params.size, QImage::Format_ARGB32_Premultiplied);
  QPainter painter;
  image.fill(bgColor);

  if (!painter.begin(&image)) {
    qWarning() << "Failed to begin QPainter on image";
    return QImage();
  }

  svgRenderer.render(&painter, params.rect);
  painter.end();
  return image;
}
