#include "writer.h"

Writer::Writer(QString output, QObject *parent) : QObject(parent)
{
    this->file = new QFile(output);
}

void Writer::write(QByteArray array)
{
    qDebug() << "Writer starts to write!";

    QTime time;
    time.start();

    if (!file->isOpen())
    {
        file->open(QFile::WriteOnly);
    }

    file->write(array.data(), array.size());

    file->close();

    qDebug() << "Writer time elapsed:" << time.elapsed() << "ms";

    emit dataHasBeenWritten();
}

Writer::~Writer()
{
    file->deleteLater();
}
