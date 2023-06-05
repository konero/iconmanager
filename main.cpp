#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QEvent>
#include "iconManager.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
    fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtWarningMsg:
    fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtCriticalMsg:
    fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtFatalMsg:
    fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    abort();
  }
}

int main(int argc, char *argv[]) {
  qInstallMessageHandler(customMessageHandler);
  QApplication a(argc, argv);
  QMainWindow w;

  // Load icons into icon map
  IconMap::getInstance().loadIconsFromResource(":/icons/");
  //IconMap::getInstance().printIconMap(); // debug

  // central widget layout
  QWidget *centralWidget = new QWidget;
  QVBoxLayout *layout    = new QVBoxLayout(centralWidget);

  // Apply button
  QPushButton *applyButton = new QPushButton("Apply");
  applyButton->setIcon(QIcon(IconMap::getInstance().getIcon("brush")));
  applyButton->setIconSize(QSize(64, 64));

  // Color replacement for SVG list
  QList<QPair<QColor, QColor>> colorReplacements{{QColor(0, 0, 255), QColor()},
                                                 {QColor(1, 1, 1), QColor()}};

  // Layout
  layout->addWidget(applyButton);

  w.setCentralWidget(centralWidget);
  w.show();
  return a.exec();
}
