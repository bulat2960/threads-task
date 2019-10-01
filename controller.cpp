#include "controller.h"

Controller::Controller(int threadsNumber, QObject *parent) : QObject(parent)
{
    QString input("/home/bulat2960/dev/my-qt-projects/ThreadsTask/input.mp4");
    Reader* reader = new Reader(input, 0);
    reader->read();
    data = reader->getData();
    reader->deleteLater();

    for (int i = 0; i < threadsNumber; i++)
    {
        readerThreads.append(new QThread(this));
    }
    writerThread = new QThread(this);

    hasPermissionForRead = true;
}

void Controller::createReaderThread(Reader *r)
{
    QThread* readerThread = findFreeReaderThread();

    if (readerThread != nullptr && hasPermissionForRead)
    {
        r->moveToThread(readerThread);

        connect(readerThread, &QThread::started, r, &Reader::read);
        connect(r, &Reader::resultReady, this, &Controller::processNewReader);
        connect(r, &Reader::resultReady, readerThread, &QThread::quit);

        readerThread->start();
    }
    else
    {
        waitReaders.enqueue(r);
    }
}

void Controller::waitForReadersFinished()
{
    for (QThread* readerThread : readerThreads)
    {
        readerThread->disconnect();
    }

    while (true)
    {
        for (QThread* readerThread : readerThreads)
        {
            if (readerThread->isRunning())
            {
                continue;
            }
        }

        break;
    }
}

void Controller::createWriterThread()
{
    if (writerThread->isRunning())
    {
        return;
    }

    hasPermissionForRead = false;

    waitForReadersFinished();

    QString output("/home/bulat2960/dev/my-qt-projects/ThreadsTask/output.mp4");

    Writer* w = new Writer(data, output);

    writerThread->disconnect();
    connect(writerThread, &QThread::started, w, &Writer::write);
    connect(w, &Writer::dataHasBeenWritten, writerThread, &QThread::quit);
    connect(w, &Writer::dataHasBeenWritten, this, &Controller::processWaitReaders);
    connect(w, &Writer::dataHasBeenWritten, w, &Writer::deleteLater);

    w->moveToThread(writerThread);
    writerThread->start();
}

QThread* Controller::findFreeReaderThread()
{
    for (QThread* readerThread : readerThreads)
    {
        if (!readerThread->isRunning())
        {
            return readerThread;
        }
    }
    return nullptr;
}

void Controller::processWaitReaders()
{
    hasPermissionForRead = true;

    if (waitReaders.empty())
    {
        return;
    }

    while (true)
    {
        QThread* readerThread = findFreeReaderThread();

        if (readerThread == nullptr)
        {
            break;
        }

        readerThread->disconnect();

        Reader* r = waitReaders.dequeue();

        qDebug() << "Читатель" << r->getReaderNumber() << "начинает чтение после завершения операции записи";

        r->moveToThread(readerThread);

        connect(readerThread, &QThread::started, r, &Reader::read);
        connect(r, &Reader::resultReady, this, &Controller::processNewReader);
        connect(r, &Reader::resultReady, readerThread, &QThread::quit);

        readerThread->start();
    }
}

void Controller::processNewReader()
{
    QObject* sender = QObject::sender();
    Reader* readerObject = qobject_cast<Reader*>(sender);

    readerObject->thread()->quit();
    readerObject->thread()->wait(1000);

    if (waitReaders.empty())
    {
        return;
    }

    QThread* readerThread = findFreeReaderThread();

    if (readerThread == nullptr)
    {
        return;
    }

    if (!hasPermissionForRead)
    {
        return;
    }

    readerThread->disconnect();

    Reader* r = waitReaders.dequeue();

    qDebug() << "Поток читателя" << readerObject->getReaderNumber()
             << "передает управление читателю" << r->getReaderNumber();

    readerObject->deleteLater();

    r->moveToThread(readerThread);

    connect(readerThread, &QThread::started, r, &Reader::read);
    connect(r, &Reader::resultReady, this, &Controller::processNewReader);
    connect(r, &Reader::resultReady, readerThread, &QThread::quit);

    readerThread->start();
}
