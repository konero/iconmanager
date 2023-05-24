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

  // create settings
  QSettings settings("settings.ini", QSettings::IniFormat);
  bool isDarkTheme = settings.value("checkBoxStatus", false).toBool();

  // load icons into the map
  IconManager iconManager;
  iconManager.switchTheme(isDarkTheme);  // switch theme based on settings
  iconManager.loadIconsFromResource(":/icons");

  // print contents of iconMap to console for debug
  printIconMap(iconManager);

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *layout    = new QVBoxLayout(centralWidget);

  // checkbox for toggling the theme
  QCheckBox *checkBox = new QCheckBox("Toggle", centralWidget);
  checkBox->setChecked(isDarkTheme);  // load settings

  QObject::connect(checkBox, &QCheckBox::stateChanged,
                   [&iconManager, checkBox, &settings](int state) {
                     iconManager.switchTheme(state == Qt::Checked);
                     settings.setValue("checkBoxStatus", checkBox->isChecked());
                   });

  // create a toolButton containing an icon for testing
  QToolButton toolButton;
  toolButton.setIcon(iconManager.getIcon("brush"));

  // build the layout
  layout->addWidget(checkBox);
  layout->addWidget(&toolButton);

  // build the window
  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
