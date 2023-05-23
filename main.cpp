#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QObject>
#include <QSettings>
#include <QCheckBox>
#include <QToolButton>
#include <QIcon>
#include <QDebug>
#include "iconManager.h"

void printIconMap(const IconManager &iconManager) {
  qDebug() << "Printing iconMap: ";
  for (auto it = iconManager.getIconMap().constBegin();
       it != iconManager.getIconMap().constEnd(); ++it) {
    const QString &iconName = it.key();
    const QString &iconPath = iconManager.getIconPaths().value(iconName);
    qDebug() << "Icon Name: " << iconName;
    qDebug() << "Icon Path: " << iconPath;
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;

  // load icons into the map
  IconManager iconManager;
  iconManager.loadIconsFromResource(":/icons");

  // print contents of iconMap to console
  printIconMap(iconManager);

  // create settings
  QSettings settings("settings.ini", QSettings::IniFormat);

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *layout    = new QVBoxLayout(centralWidget);

  // checkbox for toggling the theme
  QCheckBox *checkBox = new QCheckBox("Toggle", centralWidget);

  // load settings
  checkBox->setChecked(settings.value("checkBoxStatus", false).toBool());

  // connect stateChanged signal of checkBox to a lambda function that writes
  // the status into settings
  QObject::connect(checkBox, &QCheckBox::stateChanged,
                   [checkBox, &settings](int state) {
                     settings.setValue("checkBoxStatus", checkBox->isChecked());
                   });

  // create a toolButton containing an icon for testing
  QToolButton toolButton;
  QIcon testIcon = iconManager.getIcon("brush");
  QPixmap testPm = iconManager.recolorPixmap(
      testIcon.pixmap(QSize(32, 32), QIcon::Normal), QColor(Qt::red));
  QIcon testRecolorIcon;
  testRecolorIcon.addPixmap(testPm, QIcon::Normal);
  toolButton.setIcon(testRecolorIcon);
  toolButton.setIconSize(testRecolorIcon.actualSize(QSize(32, 32)));

  // build the layout
  layout->addWidget(checkBox);
  layout->addWidget(&toolButton);

  // build the window
  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
