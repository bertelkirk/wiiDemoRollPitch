#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H
#include "WiiRemoteSensor.h"

class SensorThread : public QThread
{
    Q_OBJECT
public:
    SensorThread();
    WiiRemoteSensor* mpParent;
signals:
    void testSignal(QString message);
protected:
    void run();
    int exec();
private:
};

#endif // SENSORTHREAD_H
