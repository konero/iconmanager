#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QDebug>
#include <QObject>
#include <QColorDialog>
#include "iconManager.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  
  MainWindow mainWindow;
  IconMap& iconObj = IconMap::getInstance();

  // Load icons into icon map
  iconObj.loadIconsFromResource(":/icons");
  QMap<QString, QIcon> iconMap = iconObj.getIconMap();
  iconObj.printIconMap();
  qWarning() << "What is iconThemeColor in icon map?:" << iconObj.iconThemeColor.name();

  // central widget layout
  QWidget* centralWidget  = new QWidget;
  QVBoxLayout* vertLayout = new QVBoxLayout(centralWidget);
  vertLayout->setObjectName("vertLayout");
  QHBoxLayout* horizLayout = new QHBoxLayout(centralWidget);
  horizLayout->setObjectName("horizLayout");

  // Apply button
  QPushButton* applyButton = new QPushButton("Apply");
  applyButton->setIcon(QIcon(IconMap::getInstance().getIcon("brush")));
  applyButton->setIconSize(QSize(64, 64));

  // Color replacement for SVG list
  QList<QPair<QColor, QColor>> colorReplacements{{QColor(0, 0, 255), QColor()},
                                                 {QColor(1, 1, 1), QColor()}};

  // Show all icons as tool buttons in the app
  for (auto it = iconMap.begin(); it != iconMap.end(); ++it) {
    const QIcon& icon   = it.value();

    QToolButton* toolBtn = new QToolButton;
    toolBtn->setText("test");
    toolBtn->setIcon(icon);

    horizLayout->addWidget(toolBtn);
  }

  // Color slider
  ColorPickerWidget* colorSlider1 = new ColorPickerWidget;

  QSettings *settings = new QSettings("config.ini", QSettings::IniFormat);

  QObject::connect(applyButton, &QPushButton::clicked, [&]() {
    QColor color = QColorDialog::getColor(Qt::white);
    if (color.isValid()) {
      iconObj.iconThemeColor = color;
      QString colorString = color.name();
      settings->setValue("iconThemeColor", colorString);
      qWarning() << "iconThemeColor is:" << iconObj.iconThemeColor;
    }
  });

  QLabel* label = new QLabel();
  QIcon temp;
  temp = iconObj.getIcon("logo_blue");
  QPixmap pm;
  pm = temp.pixmap(QSize(300, 64));
  label->setPixmap(pm);
  label->setFixedSize(pm.size());
  label->setAlignment(Qt::AlignCenter);

  // Layout
  vertLayout->addWidget(label);
  vertLayout->addLayout(horizLayout);
  vertLayout->addWidget(colorSlider1);
  vertLayout->addWidget(applyButton);

  mainWindow.setCentralWidget(centralWidget);
  mainWindow.show();
  return a.exec();
}
