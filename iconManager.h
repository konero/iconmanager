#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QSize>
#include <QRectF>
#include <QSlider>
#include <QWidget>
#include <QHBoxLayout>
#include <QMap>
#include <QIcon>
#include <QtXml>

//==============================================================================
// Icon Manager ================================================================
//==============================================================================

class IconMap : QObject {  // Singleton pattern
  Q_OBJECT

public:
  static IconMap &getInstance();

  void loadIconsFromResource(const QString &resourcePath);
  void processIcon(const QString &baseName, QImage &baseImage);

  // Debug
  void printIconMap() const;
  const QMap<QString, QIcon> &getIconMap() const;
  const QMap<QString, QString> &getIconPaths() const;

  // Params
  QColor getIconThemeColor() const;
  qreal getNormalOpacity() const;

  // Value getters
  QIcon getIcon(const QString &filename) const;

  // Prevent copy
  IconMap(IconMap const &)        = delete;
  void operator=(IconMap const &) = delete;

  // Set
  void setIconThemeColor(QColor color);

  QColor iconThemeColor;
private:
  IconMap();

  // Default library
  QMap<QString, QIcon> iconMapDefault;
  QMap<QString, QString> iconPaths;

  // Params
  qreal normalOpacity;
};

//==============================================================================
// Color Picker Slider Widget ==================================================
//==============================================================================

class ColorPickerWidget : public QWidget {
  Q_OBJECT

public:
  ColorPickerWidget(QWidget *parent = nullptr) : QWidget(parent) {
    // Create sliders
    redSlider   = createSlider();
    greenSlider = createSlider();
    blueSlider  = createSlider();

    // Set layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(redSlider);
    layout->addWidget(greenSlider);
    layout->addWidget(blueSlider);

    // Connect slider signals
    connect(redSlider, &QSlider::valueChanged, this,
            &ColorPickerWidget::onColorSliderChanged);
    connect(greenSlider, &QSlider::valueChanged, this,
            &ColorPickerWidget::onColorSliderChanged);
    connect(blueSlider, &QSlider::valueChanged, this,
            &ColorPickerWidget::onColorSliderChanged);
  }

  QColor getColor() const {
    int redValue   = redSlider->value();
    int greenValue = greenSlider->value();
    int blueValue  = blueSlider->value();

    return QColor(redValue, greenValue, blueValue);
  }

  void setColor(const QColor &color) {
    redSlider->setValue(color.red());
    greenSlider->setValue(color.green());
    blueSlider->setValue(color.blue());
  }

signals:
  void colorChanged(const QColor &color);

private slots:
  void onColorSliderChanged() {
    QColor color = getColor();
    emit colorChanged(color);
  }

private:
  QSlider *createSlider() {
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(255);
    return slider;
  }

  QSlider *redSlider;
  QSlider *greenSlider;
  QSlider *blueSlider;
};

//==============================================================================
// SvgRenderer =================================================================
//==============================================================================

struct SvgRenderParams {
  QSize size;
  QRectF rect;
};

//------------------------------------------------------------------------------

class Utils : QObject {
  Q_OBJECT

public:
  Utils();

  static SvgRenderParams calculateSvgRenderParams(
      const QSize &desiredSize, QSize &imageSize,
      Qt::AspectRatioMode aspectRatioMode);

  static QImage svgToImageWithColorChange(
      const QString &svgFilePath, const QSize &size = QSize(),
      Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio,
      QColor bgColor                      = QColor(Qt::transparent),
      QList<QPair<QColor, QColor>> colorReplacements =
          QList<QPair<QColor, QColor>>());

  static QString loadAndModifySvg(
      const QString &svgFilePath,
      const QList<QPair<QColor, QColor>> &colorReplacements);

  static QImage svgToImage(
      const QString &svgFilePath, const QSize &size = QSize(),
      Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio,
      QColor bgColor                      = QColor(Qt::transparent));

  static bool isColorEqual(const QString &colorString,
                           const QColor &colorObject);

  static void modifySvgElement(
      QDomElement element,
      const QList<QPair<QColor, QColor>> &colorReplacements);
};

//==============================================================================
// Image Utils =================================================================
//==============================================================================

class GraphicUtils {
public:
  GraphicUtils();

  static QImage adjustOpacity(const QImage &input, qreal opacity);
  static QPixmap adjustOpacity(const QPixmap &input, qreal opacity);
};

#endif  // ICONMANAGER_H
