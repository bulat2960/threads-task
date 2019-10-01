#include "writer.h"

Writer::Writer(QByteArray array, QString output, QObject *parent) : QObject(parent)
{
    this->file = new QFile(output);
    this->array = array;
}

void Writer::write()
{
    QTime time;
    time.start();

    if (!file->isOpen())
    {
        file->open(QFile::WriteOnly);
    }

    file->write(array.data(), array.size());

    file->close();

    qDebug() << "Писатель завершил свою работу. Время записи:" << time.elapsed() << "мс";

    emit dataHasBeenWritten();
}

Writer::~Writer()
{
    file->deleteLater();
}
