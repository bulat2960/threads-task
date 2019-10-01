#include "controller.h"

Controller::Controller(int threadsNumber, QObject *parent) : QObject(parent)
{
    for (int i = 0; i < threadsNumber; i++)
    {
        readerThreads.append(new QThread(this));
    }
    writerThread = new QThread(this);
}

void Controller::createReaderThread(Reader *r)
{
    QThread* readerThread = findFreeReaderThread();

    if (readerThread != nullptr)
    {
        qDebug() << "Для ридера" << r->getReaderNumber() << "найден поток!";

        r->moveToThread(readerThread);

        connect(readerThread, &QThread::started, r, &Reader::read);
        connect(r, &Reader::resultReady, this, &Controller::processNewReader);
        connect(r, &Reader::resultReady, readerThread, &QThread::quit);

        readerThreads.push_back(readerThread);

        readerThread->start();
    }
    else
    {
        qDebug() << "Ридер" << r->getReaderNumber() << "помещен в очередь ожидания";

        waitReaders.append(r);
    }
}

void Controller::createWriterThread(Writer *w)
{
    /*QThread* newWriter = new QThread(this);
    w->moveToThread(newWriter);
    connect(this, &Controller::begin, w, &Writer::write);
    newWriter->start();
    writer = newWriter;*/
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

void Controller::processNewReader()
{
    QObject* sender = QObject::sender();
    Reader* readerObject = qobject_cast<Reader*>(sender);

    qDebug() << "Ридер" << readerObject->getReaderNumber() << "завершил свою работу";


    if (waitReaders.empty())
    {
        return;
    }

    QThread* readerThread = findFreeReaderThread();

    if (readerThread == nullptr)
    {
        return;
    }

    readerThread->disconnect();

    Reader* r = waitReaders.dequeue();

    qDebug() << "Начинается работа ридера" << r->getReaderNumber();

    r->moveToThread(readerThread);

    connect(readerThread, &QThread::started, r, &Reader::read);
    connect(r, &Reader::resultReady, this, &Controller::processNewReader);
    connect(r, &Reader::resultReady, readerThread, &QThread::quit);

    readerThread->start();
}
