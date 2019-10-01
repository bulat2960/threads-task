#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QQueue>

#include "reader.h"
#include "writer.h"

class Controller : public QObject
{
    Q_OBJECT
private:
    QList<QThread*> readerThreads;
    QQueue<Reader*> waitReaders;

    QThread* writerThread;
public:
    explicit Controller(int threadsNumber, QObject *parent = nullptr);

    void createReaderThread(Reader* r);
    void createWriterThread(Writer* w);

    QThread* findFreeReaderThread();

    void start();
public slots:
    void processNewReader();
};

#endif // CONTROLLER_H
