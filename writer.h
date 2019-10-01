#ifndef WRITER_H
#define WRITER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QTime>

class Writer : public QObject
{
    Q_OBJECT
private:
    QFile* file;
    QByteArray array;
public:
    Writer(QByteArray array, QString output, QObject* parent = nullptr);
    void write();
    ~Writer();
signals:
    void dataHasBeenWritten();
};
#endif // WRITER_H
