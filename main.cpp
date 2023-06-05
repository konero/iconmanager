#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QDebug>
#include "iconManager.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;

  // Load icons into icon map
  IconMap& iconObj = IconMap::getInstance();
  iconObj.loadIconsFromResource(":/icons");
  QMap<QString, QIcon> iconMap = iconObj.getIconMap();
  iconObj.printIconMap();

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *vertLayout    = new QVBoxLayout(centralWidget);
  vertLayout->setObjectName("vertLayout");
  QHBoxLayout *horizLayout = new QHBoxLayout(centralWidget);
  horizLayout->setObjectName("horizLayout");

  // Apply button
  QPushButton *applyButton = new QPushButton("Apply");
  applyButton->setIcon(QIcon(IconMap::getInstance().getIcon("brush")));
  applyButton->setIconSize(QSize(64, 64));

  // Color replacement for SVG list
  QList<QPair<QColor, QColor>> colorReplacements{{QColor(0, 0, 255), QColor()},
                                                 {QColor(1, 1, 1), QColor()}};

  // Show all icons as tool buttons in the app
  for (auto it = iconMap.begin(); it != iconMap.end(); ++it) {
    const QString& text = it.key();
    const QIcon& icon = it.value();

    QToolButton* toolBtn = new QToolButton;
    toolBtn->setText("test");
    toolBtn->setIcon(icon);

    horizLayout->addWidget(toolBtn);
  }

  // Color slider
  ColorPickerWidget* colorSlider1 = new ColorPickerWidget;

  QLabel* label = new QLabel();
  label->setText("Test");
  label->setAlignment(Qt::AlignCenter);
  QIcon temp;
  temp = iconObj.getIcon("logo_blue");
  QPixmap pm;
  pm = temp.pixmap(QSize(300, 64));
  label->setPixmap(pm);
  label->setFixedSize(pm.size());

  // Layout
  vertLayout->addWidget(label);
  vertLayout->addLayout(horizLayout);
  vertLayout->addWidget(colorSlider1);
  vertLayout->addWidget(applyButton);

  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
