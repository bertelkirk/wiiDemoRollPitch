#include "SensorThread.h"
#include <QDebug>

SensorThread::SensorThread()
{
}

void SensorThread::run()
{
    exec();
}

int SensorThread::exec()
{

    while(1)
    {
        qDebug() << "SensorThread started";
        mpParent->runThread();
    }
}
