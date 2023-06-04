#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QSize>
#include <QRectF>

struct SvgRenderParams {
  QSize size;
  QRectF rect;
};

class Utils : QObject
{
  Q_OBJECT

public:
  Utils();

  // Loading SVG
  SvgRenderParams calculateSvgRenderParams(const QSize &desiredSize,
                                           QSize &imageSize,
                                           Qt::AspectRatioMode aspectRatioMode);
  QImage svgToImage(const QString &svgFilePath, const QSize &size = QSize(),
           Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio,
           QColor bgColor                      = Qt::transparent);
};

#endif // ICONMANAGER_H
