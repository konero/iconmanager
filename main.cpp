#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolButton>
#include <QIcon>
#include "iconManager.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *layout    = new QVBoxLayout(centralWidget);

  Utils util;
  QImage img = util.svgToImage(":/icons/brush.svg");
  QIcon icon;
  icon.addPixmap(QPixmap::fromImage(img), QIcon::Normal, QIcon::Off);

  QToolButton *toolButton = new QToolButton();
  toolButton->setIcon(icon);
  toolButton->setIconSize(QSize(64, 64));
  toolButton->setFixedSize(QSize(70, 70));

  // build the layout
  layout->addWidget(toolButton);

  // build the window
  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
