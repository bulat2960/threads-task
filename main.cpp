#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QPushButton>

#include "reader.h"
#include "writer.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // https://www.youtube.com/watch?v=3JGaNOpSbmA
    QString input("/home/bulat2960/dev/my-qt-projects/ThreadsTask/input.mp4");

    Controller* controller = new Controller(3);

    for (int i = 0; i < 100; i++)
    {
        Reader* reader = new Reader(input, i + 1);
        controller->createReaderThread(reader);
    }

    QPushButton* button = new QPushButton("Press me");
    button->setGeometry(500, 500, 500, 200);
    QObject::connect(button, &QPushButton::clicked, controller, &Controller::createWriterThread);

    button->show();

    return a.exec();
}
