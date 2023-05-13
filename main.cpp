#include <QApplication>
#include <QLabel>
#include "iconManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IconManager iconManager;
    iconManager.loadIconsFromResource(":/icons");

    QLabel label;
    label.setPixmap(iconManager.getIcon("eraser").pixmap(64, 64));
    label.show();

    return app.exec();
}
