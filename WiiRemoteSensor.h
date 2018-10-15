#ifndef WIIREMOTESENSOR_H
#define WIIREMOTESENSOR_H

#include <QThread>
#include <QObject>
#include <QVector>
#include <QDebug>
#include <bluetooth/bluetooth.h>
#include <cstdio>
extern "C"{
#include <cwiid.h>

}

class WiiRemoteSensor: public QObject
{
    Q_OBJECT
    typedef struct point3d {
        int x;
        int y;
        int z;
    } point3d_t;

public:
    explicit WiiRemoteSensor(QObject *parent = 0);
    void runThread();
    int test;

    void set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state);
    void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode);
    static void err(cwiid_wiimote_t *wiimote, const char *s, va_list ap);
    static void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], struct timespec *timestamp);
signals:
    void signalWiiStatusUpdate(quint16 buttons, double roll, double pitch, qint16 x,qint16 y,qint16 z);
private:

    QThread *wiiStatusThread;
    cwiid_wiimote_t *wiimoteC;	/* wiimote handle */
    struct cwiid_state state;	/* wiimote state */
    bdaddr_t bdaddr;	/* bluetooth device address */
    unsigned char mesg = 0;
    unsigned char led_state = 0;
    unsigned char rpt_mode = 0;
    unsigned char rumble = 0;
    int exit = 0;
    quint16 buttons;
    QVector<int> acceleration = {127,127,127};
    QVector<point3d> accFiltered;
    double pitch=0.0;
    double roll=0.0;
    double yaw=0.0;

    void addDataToFilter(point3d_t &acc, QVector<point3d_t> &filterBuffer,int filterLen=5){
         filterBuffer.push_back( acc );
         while( filterBuffer.size() > filterLen ){
             filterBuffer.remove(0);
         }
    }

    point3d_t getFilteredData( QVector<point3d_t> &filterBuffer ){
       point3d_t res;
       res.x = 0;
       res.y = 0;
       res.z = 0;
       int filterLen=filterBuffer.size();
       for(uint i=0; i< filterLen; i++ ){
           res.x += filterBuffer.at(i).x;
           res.y += filterBuffer.at(i).y;
           res.z += filterBuffer.at(i).z;
       }
       res.x = res.x/filterLen;
       res.y = res.y/filterLen;
       res.z = res.z/filterLen;
       return res;
    }

};

#endif // WIIREMOTESENSOR_H
