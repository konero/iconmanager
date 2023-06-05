#include <QImage>
#include <QSvgRenderer>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QtXml>
#include <QIcon>
#include <QMap>
#include <QResource>
#include "iconManager.h"

//==============================================================================
// Core ========================================================================
//==============================================================================

int getHighestDevicePixelRatio() {
  static int highestDevPixRatio = 0;
  if (highestDevPixRatio == 0) {
    for (auto screen : QGuiApplication::screens())
      highestDevPixRatio =
          std::max(highestDevPixRatio, (int)screen->devicePixelRatio());
  }
  return highestDevPixRatio;
}

//==============================================================================
// Icon Manager ================================================================
//==============================================================================

IconMap::IconMap() {
  iconThemeColor = Qt::black;

  // Set icon brightness levels
  normalOpacity   = 0.8;
}

//------------------------------------------------------------------------------

IconMap &IconMap::getInstance() {
  static IconMap instance;
  return instance;
}

//------------------------------------------------------------------------------

QIcon IconMap::getIcon(const QString &filename) const {
  return iconMapDefault.value(filename);
}

//------------------------------------------------------------------------------

QColor IconMap::getIconThemeColor() const { return iconThemeColor; }

//------------------------------------------------------------------------------

qreal IconMap::getNormalOpacity() const { return normalOpacity; }

//------------------------------------------------------------------------------

void IconMap::processIcon(const QString &dirPath, const QString &baseName,
                          QImage &baseImage) {
  if (baseImage.isNull()) return;

  QIcon icon;

  icon.addPixmap(QPixmap::fromImage(baseImage));
  iconMapDefault.insert(baseName, icon);
}

//------------------------------------------------------------------------------

void IconMap::loadIconsFromResource(const QString &resourcePath) {
  QDirIterator it(resourcePath,
                  QStringList() << "*.svg"
                                << "*.png",
                  QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    it.next();

    QFileInfo fileInfo = it.fileInfo();
    QString fileName   = fileInfo.fileName();         // image.ext
    QString baseName   = fileInfo.baseName();         // image
    QString dirPath    = it.fileInfo().dir().path();  // :/path/to

    QString baseFilePath = dirPath + fileName;

    iconPaths.insert(baseName, it.filePath());

    // Load base icon image
    QImage baseImage = Utils::svgToImageWithColorChange(baseFilePath);

    // Build icon
    processIcon(dirPath, baseName, baseImage);
  }
}

//==============================================================================
// Icon Manager : Debug Tools ==================================================
//==============================================================================

const QMap<QString, QIcon> &IconMap::getIconMap() const {
  return iconMapDefault;
}

const QMap<QString, QString> &IconMap::getIconPaths() const {
  return iconPaths;
}

void IconMap::printIconMap() const {
  qDebug() << "Printing iconMap: ";
  for (auto it = this->getIconMap().constBegin();
       it != this->getIconMap().constEnd(); ++it) {
    const QString &iconName = it.key();
    const QString &iconPath = this->getIconPaths().value(iconName);
    qDebug() << "Icon Name:" << iconName;
    qDebug() << "Icon Path:" << iconPath;

    // For printing all available icon sizes
    QIcon icon(getIcon(iconName));
    qDebug() << "Available sizes:";
    for (const QSize &availableSize : icon.availableSizes()) {
      qDebug() << "Size:" << availableSize.width() << "x"
               << availableSize.height();
    }
  }
}

//==============================================================================
// Image Manipulation ==========================================================
//==============================================================================

GraphicUtils::GraphicUtils() {}

//------------------------------------------------------------------------------

QImage GraphicUtils::adjustOpacity(const QImage &input, qreal opacity) {
  if (input.isNull()) return QImage();

  QImage result(input.size(), QImage::Format_ARGB32_Premultiplied);

  QPainter painter(&result);
  if (!painter.isActive()) return QImage();

  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawImage(0, 0, input);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.fillRect(
      result.rect(),
      QColor(0, 0, 0, qBound(0, static_cast<int>(opacity * 255), 255)));
  painter.end();

  return result;
}

//------------------------------------------------------------------------------

QPixmap GraphicUtils::adjustOpacity(const QPixmap &input, qreal opacity) {
  if (input.isNull()) return QPixmap();

  QImage img    = input.toImage();
  QImage result = adjustOpacity(img, opacity);

  return QPixmap::fromImage(result);
}

//==============================================================================
// SvgUtils ====================================================================
//==============================================================================

Utils::Utils() {}

//------------------------------------------------------------------------------

SvgRenderParams Utils::calculateSvgRenderParams(
    const QSize &desiredSize, QSize &imageSize,
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

//------------------------------------------------------------------------------

QImage Utils::svgToImageWithColorChange(
    const QString &svgFilePath, const QSize &size,
    Qt::AspectRatioMode aspectRatioMode, QColor bgColor,
    QList<QPair<QColor, QColor>> colorReplacements) {
  QString modifiedSvg = loadAndModifySvg(svgFilePath, colorReplacements);

  QSvgRenderer svgRenderer;
  svgRenderer.load(modifiedSvg.toUtf8());

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
    qWarning() << "Failed to begin QPainter on image:" << svgFilePath;
    return QImage();
  }

  svgRenderer.render(&painter, params.rect);
  painter.end();
  return image;
}

//------------------------------------------------------------------------------

bool isColorEqual(const QString &colorString, const QColor &colorObject) {
  QColor colorFromSvg;

  if (colorString.startsWith("#")) {
    colorFromSvg.setNamedColor(colorString);
  } else if (colorString.startsWith("rgb(") && colorString.endsWith(")")) {
    // Remove "rgb(" from start and ")" from end
    QStringRef colorWithoutRgb =
        colorString.midRef(4, colorString.length() - 5);

    // Split into R, G, and B components
    QStringList rgbValues = colorWithoutRgb.toString().split(',');

    // Check we have exactly 3 components
    if (rgbValues.size() == 3) {
      colorFromSvg.setRed(rgbValues[0].trimmed().toInt());
      colorFromSvg.setGreen(rgbValues[1].trimmed().toInt());
      colorFromSvg.setBlue(rgbValues[2].trimmed().toInt());
    }
  }

  // Compare the colors
  return colorFromSvg == colorObject;
}

//------------------------------------------------------------------------------

void modifySvgElement(QDomElement element,
                      const QList<QPair<QColor, QColor>> &colorReplacements) {
  // Check for inline fill and stroke color
  if (element.hasAttribute("style")) {
    QString styleString         = element.attribute("style");
    QStringList styleAttributes = styleString.split(";");

    for (int j = 0; j < styleAttributes.count(); j++) {
      QStringList styleSubAttributes = styleAttributes[j].split(":");
      if (styleSubAttributes.size() < 2) {
        continue;
      }

      QString prop  = styleSubAttributes[0].trimmed();
      QString value = styleSubAttributes[1].trimmed();

      if (prop == "fill" || prop == "stroke") {
        for (const auto &colorReplacement : colorReplacements) {
          if (isColorEqual(value, colorReplacement.first)) {
            styleAttributes[j] =
                QString("%1:%2").arg(prop, colorReplacement.second.name());
            break;
          }
        }
      }
    }
    element.setAttribute("style", styleAttributes.join(";"));
  }

  // Check for explicit fill and stroke attributes
  for (const auto &colorReplacement : colorReplacements) {
    if (element.hasAttribute("fill") &&
        isColorEqual(element.attribute("fill"), colorReplacement.first)) {
      element.setAttribute("fill", colorReplacement.second.name());
    }
    if (element.hasAttribute("stroke") &&
        isColorEqual(element.attribute("stroke"), colorReplacement.first)) {
      element.setAttribute("stroke", colorReplacement.second.name());
    }
  }

  // Process child elements
  for (int i = 0; i < element.childNodes().count(); i++) {
    if (element.childNodes().at(i).isElement()) {
      QDomElement childElement = element.childNodes().at(i).toElement();
      modifySvgElement(childElement, colorReplacements);
    }
  }
}

//------------------------------------------------------------------------------

QString Utils::loadAndModifySvg(
    const QString &svgFilePath,
    const QList<QPair<QColor, QColor>> &colorReplacements) {
  QFile file(svgFilePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Failed to open SVG file:" << svgFilePath;
    return QString();
  }

  QDomDocument doc;
  if (!doc.setContent(&file)) {
    qWarning() << "Failed to parse SVG file:" << svgFilePath;
    return QString();
  }

  // Replace elements recursively
  modifySvgElement(doc.documentElement(), colorReplacements);

  // Save updated SVG data back to string
  QString modifiedSvg;
  QTextStream stream(&modifiedSvg);
  doc.save(stream, 4);  // Indentation width of 4

  return modifiedSvg;
}
