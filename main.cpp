#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include "iconManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.resize(400, 300);

    IconManager iconManager;
    iconManager.loadIconsFromResource(":/icons");

    QWidget* centralWidget = new QWidget(&mainWindow);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // A label for showing the icon
    QLabel label(&mainWindow);
    label.setPixmap(iconManager.getIcon("eraser").pixmap(64, 64));

    // Checkbox
    QCheckBox checkbox;
    checkbox.setText("Enable Eraser");
    checkbox.setChecked(true);

    // Add widgets to layout
    mainLayout->addWidget(&label);
    mainLayout->addWidget(&checkbox);

    // Signal for changing state of label via checkbox
    QObject::connect(&checkbox, &QCheckBox::stateChanged, [&label](int state) {
        if (state == Qt::Checked) {
            // Checkbox is checked, enable eraser function
            label.setEnabled(true);
        } else {
            // Checkbox is unchecked, disable eraser function
            label.setEnabled(false);
        }
    });

    mainWindow.setCentralWidget(centralWidget);
    mainWindow.show();

    return app.exec();
}
