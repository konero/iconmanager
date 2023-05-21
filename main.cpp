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

void printIconMap(const IconManager& iconManager) {
    qDebug() << "Printing iconMap: ";
    for (auto it = iconManager.getIconMap().constBegin();
        it != iconManager.getIconMap().constEnd(); ++it) {
        const QString& iconName = it.key();
        const QString& iconPath = iconManager.getIconPaths().value(iconName);
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
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // checkbox for toggling the theme
    QCheckBox *checkBox = new QCheckBox("Toggle", centralWidget);

    // load settings
    checkBox->setChecked(settings.value("checkBoxStatus", false).toBool());

    // connect stateChanged signal of checkBox to a lambda function that writes
    // the status into settings
    QObject::connect(checkBox, &QCheckBox::stateChanged,
                    [checkBox, &settings](int state) {
                    settings.setValue("checkBoxStatus",
                    checkBox->isChecked());
    });

    // create a toolButton containing an icon for testing
    QToolButton toolButton;
    QIcon icon(":/icons/brush.svg");
    toolButton.setIcon(icon);
    toolButton.setIconSize(icon.actualSize(QSize(32, 32)));

    // build the layout
    layout->addWidget(checkBox);
    layout->addWidget(&toolButton);

    // build the window
    w.setCentralWidget(centralWidget);
    w.show();
    return a.exec();
}

































/*
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // create settings file
    QSettings settings("settings.ini", QSettings::IniFormat);

    QComboBox comboBox;
    comboBox.addItem("Light");
    comboBox.addItem("Dark");

    // load the saved option from the settings file
    QString savedOption = settings.value("ComboBoxOption").toString();
    int savedIndex = comboBox.findText(savedOption);
    if (savedIndex != -1) {
        comboBox.setCurrentIndex(savedIndex);
    }

    layout.addWidget(&comboBox);

    window.setLayout(&layout);
    window.setWindowTitle("Tool Buttons and ComboBox Example");
    window.show();

    return app.exec();
}
*/
/*
    // connect a slot to handle the combo box selection change
    QObject::connect(&comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index){
        QString selectedOption = comboBox.itemText(index);
        settings.setValue("ComboBoxOption", selectedOption);
    });

    QMainWindow mainWindow;

    IconManager iconManager;
    iconManager.loadIconsFromResource(":/icons/");

    const QMap<QString, QIcon>& iconMap = iconManager.getIconMap();

    QWidget* centralWidget = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    for (auto it = iconMap.constBegin(); it != iconMap.constEnd(); ++it)
    {
        const QString& iconName = it.key();
        const QIcon& icon = it.value();

        QLabel* label = new QLabel();
        label->setText(iconName);
        label->setPixmap(icon.pixmap(QSize(32, 32)));

        layout->addWidget(label);
    }

    layout->addWidget(&comboBox);
    mainWindow.show();

    return app.exec();
}



*/






/* working function that updates colors of icons
#include <QApplication>
#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSettings>
#include <QImage>
#include <QRgb>

QPixmap recolorIcon(const QString& iconPath, const QColor& color) {
    QPixmap originalIcon(iconPath);
    QImage img = originalIcon.toImage().convertToFormat(QImage::Format_ARGB32);
    QRgb targetColor = color.rgb();
    int height = img.height();
    int width = img.width();

    for (int y = 0; y < height; ++y) {
        QRgb* pixel = reinterpret_cast<QRgb*>(img.scanLine(y));
        QRgb* end = pixel + width;
        for (; pixel != end; ++pixel) {
            if (qGray(*pixel) == 0) {
                *pixel = (targetColor & 0x00FFFFFF) | (qAlpha(*pixel) << 24);
            }
        }
    }

    return QPixmap::fromImage(img);
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QSettings settings("settings.ini", QSettings::IniFormat);

    QWidget window;
    QVBoxLayout layout(&window);

    QToolButton button1;
    button1.setText("Button 1");
    button1.setIcon(recolorIcon(":/icons/animate.svg", QColor(Qt::black)));
    button1.setIconSize(QSize(32, 32));

    QToolButton button2;
    button2.setText("Button 2");
    button2.setIcon(recolorIcon(":/icons/brush.svg", QColor(Qt::black)));
    button2.setIconSize(QSize(32, 32));

    QToolButton button3;
    button3.setText("Button 3");
    button3.setIcon(recolorIcon(":/icons/eraser.svg", QColor(Qt::black)));
    button3.setIconSize(QSize(32, 32));

    layout.addWidget(&button1);
    layout.addWidget(&button2);
    layout.addWidget(&button3);

    QComboBox comboBox;
    comboBox.addItem("Light");
    comboBox.addItem("Dark");

    // Load the saved option from the settings file
    QString savedOption = settings.value("ComboBoxOption").toString();
    int savedIndex = comboBox.findText(savedOption);
    if (savedIndex != -1) {
        comboBox.setCurrentIndex(savedIndex);
        // Apply recoloring based on the selected option
        if (savedOption == "Dark") {
            button1.setIcon(recolorIcon(":/icons/animate.svg", QColor(Qt::white)));
            button2.setIcon(recolorIcon(":/icons/brush.svg", QColor(Qt::white)));
            button3.setIcon(recolorIcon(":/icons/eraser.svg", QColor(Qt::white)));
        }
    }

    // Connect a slot to handle the combo box selection change
    QObject::connect(&comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        QString selectedOption = comboBox.itemText(index);
        settings.setValue("ComboBoxOption", selectedOption);

        // Apply recoloring based on the selected option
        if (selectedOption == "Light") {
            button1.setIcon(recolorIcon(":/icons/animate.svg", QColor(Qt::black)));
            button2.setIcon(recolorIcon(":/icons/brush.svg", QColor(Qt::black)));
            button3.setIcon(recolorIcon(":/icons/eraser.svg", QColor(Qt::black)));
        }
        else if (selectedOption == "Dark") {
            button1.setIcon(recolorIcon(":/icons/animate.svg", QColor(Qt::white)));
            button2.setIcon(recolorIcon(":/icons/brush.svg", QColor(Qt::white)));
            button3.setIcon(recolorIcon(":/icons/eraser.svg", QColor(Qt::white)));
        }
    });

    layout.addWidget(&comboBox);

    window.setLayout(&layout);
    window.setWindowTitle("Tool Buttons and ComboBox Example");
    window.show();

    return app.exec();
}
*/




/*
#include <QApplication>
#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSettings settings("settings.ini", QSettings::IniFormat);

    QWidget window;
    QVBoxLayout layout(&window);

    QToolButton button1;
    button1.setText("Button 1");
    button1.setIcon(QIcon(":/icons/animate.svg"));
    button1.setIconSize(QSize(32, 32));

    QToolButton button2;
    button2.setText("Button 2");
    button2.setIcon(QIcon(":/icons/brush.svg"));
    button2.setIconSize(QSize(32, 32));

    QToolButton button3;
    button3.setText("Button 3");
    button3.setIcon(QIcon(":/icons/eraser.svg"));
    button3.setIconSize(QSize(32, 32));

    layout.addWidget(&button1);
    layout.addWidget(&button2);
    layout.addWidget(&button3);

    QComboBox comboBox;
    comboBox.addItem("Light");
    comboBox.addItem("Dark");

    // Load the saved option from the settings file
    QString savedOption = settings.value("ComboBoxOption").toString();
    int savedIndex = comboBox.findText(savedOption);
    if (savedIndex != -1) {
        comboBox.setCurrentIndex(savedIndex);
    }

    // Connect a slot to handle the combo box selection change
    QObject::connect(&comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index){
        QString selectedOption = comboBox.itemText(index);
        settings.setValue("ComboBoxOption", selectedOption);
    });

    layout.addWidget(&comboBox);

    window.setLayout(&layout);
    window.setWindowTitle("Tool Buttons and ComboBox Example");
    window.show();

    return app.exec();
}
*/