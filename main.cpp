#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolButton>
#include <QIcon>
#include <QPushButton>
#include <QAction>
#include "iconManager.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;

  // Load icons into icon map
  IconMap::getInstance().loadIconsFromResource(":/icons");

  // Print icon map to console
  IconMap::getInstance().printIconMap();

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *layout    = new QVBoxLayout(centralWidget);

  // Create a color picker widget
  ColorPickerWidget *colorPicker1 = new ColorPickerWidget();
  ColorPickerWidget *colorPicker2 = new ColorPickerWidget();

  // Apply button
  QPushButton *applyButton = new QPushButton("Apply");

  Utils util;

  QList<QPair<QColor, QColor>> colorReplacements{{QColor(0, 0, 255), QColor()},
                                                 {QColor(1, 1, 1), QColor()}};

  // Get icon from map for testing
  IconMap &iconMap = IconMap::getInstance();
  QIcon iconTest;
  iconTest = iconMap.getIcon("brush");

  QToolButton *toolButton = new QToolButton();
  toolButton->setIcon(iconTest);
  toolButton->setIconSize(QSize((300 * 2), (64 * 2)));
  toolButton->setFixedSize(QSize(500, 164));

  // Define a lambda function that updates the image color
  // auto updateImageColor = [&]() {
  //   QColor newColor1            = colorPicker1->getColor();
  //   colorReplacements[0].second = newColor1.name();

  //   QColor newColor2            = colorPicker2->getColor();
  //   colorReplacements[1].second = newColor2.name();

  //   QImage img = util.svgToImageWithColorChange(
  //       ":/icons/brush.svg", QSize((300 * 2), (64 * 2)), Qt::IgnoreAspectRatio,
  //       Qt::transparent, colorReplacements);
  //   icon.addPixmap(QPixmap::fromImage(img), QIcon::Normal, QIcon::Off);
  //   toolButton->setIcon(iconTest);
  // };

  // Connect applyButton's clicked signal to updateImageColor
  // QObject::connect(applyButton, &QPushButton::clicked, updateImageColor);

  // Connect colorPicker1's colorChanged signal to updateImageColor
  // QObject::connect(colorPicker1, &ColorPickerWidget::colorChanged,
                  //  updateImageColor);

  // Connect colorPicker2's colorChanged signal to updateImageColor
  // QObject::connect(colorPicker2, &ColorPickerWidget::colorChanged,
                  //  updateImageColor);

  // Loop through map and display all icons
  QMap<QString, QIcon> iconsLibrary = iconMap.getIconMap();
  for (auto it = iconsLibrary.begin(); it != iconsLibrary.end(); ++it) {
    const QString &actionText = it.key();
    const QIcon actionIcon    = it.value();

    // Create a QAction with icon and text
    QAction *action = new QAction(actionIcon, actionText);
    action->setIcon(actionIcon);

    // Create a button because QAction is not a widget
    QToolButton *button = new QToolButton;
    button->setDefaultAction(action);

    // Add to layout
    layout->addWidget(button);
  }

  // build the layout
  layout->addWidget(toolButton);
  layout->addWidget(colorPicker1);
  layout->addWidget(colorPicker2);
  layout->addWidget(applyButton);

  // build the window
  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
