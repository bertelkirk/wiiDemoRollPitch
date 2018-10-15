#include "WiiRemoteSensor.h"
#include "SensorThread.h"
#include <cstdio>
#include <QDebug>

WiiRemoteSensor::WiiRemoteSensor(QObject *parent) : QObject(parent)
{
    SensorThread *thread = new SensorThread();
    thread->mpParent= this;
    thread->start();
}

void  WiiRemoteSensor::runThread(){
    bool running=true;
    bool wiiConnected=false;
    bdaddr = *BDADDR_ANY;
    /* Connect to the wiimote */
    qDebug() << "Press button 1+2 to sync/connect wiimote";
    while(wiiConnected==false)
    {
        if (!(wiimoteC = cwiid_open(&bdaddr, 0))) {
            fprintf(stderr, "Unable to connect to wiimote\n");

        }else{ wiiConnected=true;
        }
        if (cwiid_set_mesg_callback(wiimoteC, this->cwiid_callback ) ) {
            fprintf(stderr, "Unable to set message callback\n");
            wiiConnected=false;
        }
    }

    qDebug() << "Wii-remote connected";

    cwiid_set_err(this->err);
    rpt_mode= CWIID_RPT_ACC | CWIID_RPT_BTN;
    set_rpt_mode(wiimoteC, rpt_mode);
    set_led_state(wiimoteC,4);

    while(running){
        QThread::msleep(60);
        if (cwiid_get_state(wiimoteC, &(state) ) ) {
            fprintf(stderr, "Error getting state\n");
        }else{
            buttons = state.buttons;
            int x= acceleration[0]=state.acc[0]-123;
            int y= acceleration[1]=state.acc[1]-123;
            int z= acceleration[2]=state.acc[2]-123;
            point3d_t acc, accF;
            acc.x = x;
            acc.y = y;
            acc.z = z;

            addDataToFilter(acc, accFiltered, 5);
            accF = getFilteredData(accFiltered);

            qDebug() << "input x:" << acc.x << "  y" << acc.y << "  z" << acc.z << "    filtered   :x"  << accF.x << "  y" << accF.y << "  z" << accF.z ;
            x=accF.x;
            y=accF.y;
            z=accF.z;

            roll  = (atan2(y, z)*180.0)/M_PI;
            pitch = (atan2(x, sqrt(y*y + z*z))*180.0)/M_PI;
            //qDebug() << "State: BTNs: " << buttons << "  x" << x << "  y" << y << "  z" << z << "   roll:"<<roll<<"    pitch:"<< pitch;
            signalWiiStatusUpdate(buttons, roll, pitch, x,y,z);

        }
    }
}

void WiiRemoteSensor::err(cwiid_wiimote_t *wiimote, const char *s, va_list ap)
{
    if (wiimote) printf("%d:", cwiid_get_id(wiimote));
    else printf("-1:");
    vprintf(s, ap);
    printf("\n");
}

void WiiRemoteSensor::set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state)
{
    if (cwiid_set_led(wiimote, led_state)) {
        fprintf(stderr, "Error setting LEDs \n");
    }
}

void WiiRemoteSensor::set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode)
{
    if (cwiid_set_rpt_mode(wiimote, rpt_mode)) {
        fprintf(stderr, "Error setting report mode\n");
    }
}


void WiiRemoteSensor::cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                                     union cwiid_mesg mesg[], struct timespec *timestamp)
{
    int i, j;
    int valid_source;
    (void) timestamp;
    for (i=0; i < mesg_count; i++)
    {
        switch (mesg[i].type) {
        case CWIID_MESG_STATUS:
            printf("Status Report: battery=%d extension=",
                   mesg[i].status_mesg.battery);
            switch (mesg[i].status_mesg.ext_type) {
            case CWIID_EXT_NONE:
                printf("none");
                break;
            case CWIID_EXT_NUNCHUK:
                printf("Nunchuk");
                break;
            case CWIID_EXT_CLASSIC:
                printf("Classic Controller");
                break;
            case CWIID_EXT_BALANCE:
                printf("Balance Board");
                break;
            case CWIID_EXT_MOTIONPLUS:
                printf("MotionPlus");
                break;
            default:
                printf("Unknown Extension");
                break;
            }
            printf("\n");
            break;
        case CWIID_MESG_BTN:
            printf("Button Report: %.4X\n", mesg[i].btn_mesg.buttons);
            break;
        case CWIID_MESG_ACC:
            printf("Acc Report: x=%d, y=%d, z=%d\n",
                   mesg[i].acc_mesg.acc[CWIID_X],
                   mesg[i].acc_mesg.acc[CWIID_Y],
                   mesg[i].acc_mesg.acc[CWIID_Z]);
            break;
        case CWIID_MESG_IR:
            printf("IR Report: ");
            valid_source = 0;
            for (j = 0; j < CWIID_IR_SRC_COUNT; j++) {
                if (mesg[i].ir_mesg.src[j].valid) {
                    valid_source = 1;
                    printf("(%d,%d) ", mesg[i].ir_mesg.src[j].pos[CWIID_X],
                           mesg[i].ir_mesg.src[j].pos[CWIID_Y]);
                }
            }
            if (!valid_source) {
                printf("no sources detected");
            }
            printf("\n");
            break;
        case CWIID_MESG_NUNCHUK:
            printf("Nunchuk Report: btns=%.2X stick=(%d,%d) acc.x=%d acc.y=%d "
                   "acc.z=%d\n", mesg[i].nunchuk_mesg.buttons,
                   mesg[i].nunchuk_mesg.stick[CWIID_X],
                   mesg[i].nunchuk_mesg.stick[CWIID_Y],
                   mesg[i].nunchuk_mesg.acc[CWIID_X],
                   mesg[i].nunchuk_mesg.acc[CWIID_Y],
                   mesg[i].nunchuk_mesg.acc[CWIID_Z]);
            break;
        case CWIID_MESG_CLASSIC:
            printf("Classic Report: btns=%.4X l_stick=(%d,%d) r_stick=(%d,%d) "
                   "l=%d r=%d\n", mesg[i].classic_mesg.buttons,
                   mesg[i].classic_mesg.l_stick[CWIID_X],
                   mesg[i].classic_mesg.l_stick[CWIID_Y],
                   mesg[i].classic_mesg.r_stick[CWIID_X],
                   mesg[i].classic_mesg.r_stick[CWIID_Y],
                   mesg[i].classic_mesg.l, mesg[i].classic_mesg.r);
            break;
        case CWIID_MESG_BALANCE:
            printf("Balance Report: right_top=%d right_bottom=%d "
                   "left_top=%d left_bottom=%d\n",
                   mesg[i].balance_mesg.right_top,
                   mesg[i].balance_mesg.right_bottom,
                   mesg[i].balance_mesg.left_top,
                   mesg[i].balance_mesg.left_bottom);
            break;
        case CWIID_MESG_MOTIONPLUS:
            printf("MotionPlus Report: angle_rate=(%d,%d,%d) low_speed=(%d,%d,%d)\n",
                   mesg[i].motionplus_mesg.angle_rate[0],
                    mesg[i].motionplus_mesg.angle_rate[1],
                    mesg[i].motionplus_mesg.angle_rate[2],
                    mesg[i].motionplus_mesg.low_speed[0],
                    mesg[i].motionplus_mesg.low_speed[1],
                    mesg[i].motionplus_mesg.low_speed[2]);
            break;
        case CWIID_MESG_ERROR:
            if (cwiid_close(wiimote)) {
                fprintf(stderr, "Error on wiimote disconnect\n");
                return; //exit(-1);
            }
            return; // exit(0);
            break;
        default:
            printf("Unknown Report");
            break;
        }
    }
}
