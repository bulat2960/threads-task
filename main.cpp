#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QThread>

#include "reader.h"
#include "writer.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // https://www.youtube.com/watch?v=3JGaNOpSbmA
    QString input("/home/bulat2960/dev/my-qt-projects/ThreadsTask/origin.mp4");
    QString output("/home/bulat2960/dev/my-qt-projects/ThreadsTask/output.mp4");

    Controller* controller = new Controller(4);

    for (int i = 0; i < 10; i++)
    {
        Reader* reader = new Reader(input, i + 1);
        controller->createReaderThread(reader);
    }

    Writer* writer = new Writer(output);
    controller->createWriterThread(writer);

    return a.exec();
}
