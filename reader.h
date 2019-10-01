#ifndef READER_H
#define READER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QTime>

class Reader : public QObject
{
    Q_OBJECT
private:
    QFile* file;
    QByteArray array;

    int readerNumber;
public:
    explicit Reader(QString input, int readerNumber, QObject* parent = nullptr);
    void read();
    QByteArray getData() const;
    int getReaderNumber() const;
    ~Reader();
signals:
    void resultReady();
};

#endif // READER_H
