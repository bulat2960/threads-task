#include "reader.h"

Reader::Reader(QString input, int readerNumber, QObject *parent) : QObject(parent)
{
    this->file = new QFile(input);

    this->readerNumber = readerNumber;
}

void Reader::read()
{
    qDebug() << "Reader" << readerNumber << "starts to read!";

    QTime time;
    time.start();

    array.clear();

    if (!file->isOpen())
    {
        file->open(QFile::ReadOnly);
    }

    while (file->bytesAvailable() > 0)
    {
        array += file->readAll();
    }

    qDebug() << "Reader" << readerNumber << "time elapsed:" << time.elapsed() << "ms";

    emit resultReady();
}

QByteArray Reader::getData() const
{
    return array;
}

int Reader::getReaderNumber() const
{
    return readerNumber;
}

Reader::~Reader()
{
    file->close();
    file->deleteLater();
}
