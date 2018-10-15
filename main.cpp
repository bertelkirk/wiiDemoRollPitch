#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "WiiRemoteSensor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    /*WiiRemoteSensor wiiRemote;
    wiiRemote.test=1;
    */
    qmlRegisterType<WiiRemoteSensor>("BKWiiLibrary", 1, 0, "WiiRemoteSensor");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}



//int mainC(int argc, char *argv[])
//{
//    cwiid_wiimote_t *wiimote;	/* wiimote handle */
//    struct cwiid_state state;	/* wiimote state */
//    bdaddr_t bdaddr;	/* bluetooth device address */
//    unsigned char mesg = 0;
//    unsigned char led_state = 0;
//    unsigned char rpt_mode = 0;
//    unsigned char rumble = 0;
//    int exit = 0;

//    cwiid_set_err(err);

//    /* Connect to address given on command-line, if present */
//    if (argc > 1) {
//        //str2ba(argv[1], &bdaddr);
//    }
//    else {
//        bdaddr = *BDADDR_ANY;
//    }

//    /* Connect to the wiimote */
//    printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
//    if (!(wiimote = cwiid_open(&bdaddr, 0))) {
//        fprintf(stderr, "Unable to connect to wiimote\n");
//        return -1;
//    }
//    if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
//        fprintf(stderr, "Unable to set message callback\n");
//    }


//    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return app.exec();


//    printf("Note: To demonstrate the new API interfaces, wmdemo no longer "
//           "enables messages by default.\n"
//           "Output can be gathered through the new state-based interface (s), "
//           "or by enabling the messages interface (m).\n");

//    /* Menu */
//    printf("%s", MENU);

//    while (!exit) {
//        switch (getchar()) {
//        case '1':
//            toggle_bit(led_state, CWIID_LED1_ON);
//            set_led_state(wiimote, led_state);
//            break;
//        case '2':
//            toggle_bit(led_state, CWIID_LED2_ON);
//            set_led_state(wiimote, led_state);
//            break;
//        case '3':
//            toggle_bit(led_state, CWIID_LED3_ON);
//            set_led_state(wiimote, led_state);
//            break;
//        case '4':
//            toggle_bit(led_state, CWIID_LED4_ON);
//            set_led_state(wiimote, led_state);
//            break;
//        case '5':
//            toggle_bit(rumble, 1);
//            if (cwiid_set_rumble(wiimote, rumble)) {
//                fprintf(stderr, "Error setting rumble\n");
//            }
//            break;
//        case 'a':
//            toggle_bit(rpt_mode, CWIID_RPT_ACC);
//            set_rpt_mode(wiimote, rpt_mode);
//            break;
//        case 'b':
//            toggle_bit(rpt_mode, CWIID_RPT_BTN);
//            set_rpt_mode(wiimote, rpt_mode);
//            break;
//        case 'c':
//            cwiid_enable(wiimote, CWIID_FLAG_MOTIONPLUS);
//            break;
//        case 'e':
//            /* CWIID_RPT_EXT is actually
//             * CWIID_RPT_NUNCHUK | CWIID_RPT_CLASSIC | CWIID_RPT_BALANCE */
//            toggle_bit(rpt_mode, CWIID_RPT_EXT);
//            set_rpt_mode(wiimote, rpt_mode);
//            break;
//        case 'i':
//            /* libwiimote picks the highest quality IR mode available with the
//             * other options selected (not including as-yet-undeciphered
//             * interleaved mode */
//            toggle_bit(rpt_mode, CWIID_RPT_IR);
//            set_rpt_mode(wiimote, rpt_mode);
//            break;
//        case 'm':
//            if (!mesg) {
//                if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) {
//                    fprintf(stderr, "Error enabling messages\n");
//                }
//                else {
//                    mesg = 1;
//                }
//            }
//            else {
//                if (cwiid_disable(wiimote, CWIID_FLAG_MESG_IFC)) {
//                    fprintf(stderr, "Error disabling message\n");
//                }
//                else {
//                    mesg = 0;
//                }
//            }
//            break;
//        case 'p':
//            printf("%s", MENU);
//            break;
//        case 'r':
//            if (cwiid_request_status(wiimote)) {
//                fprintf(stderr, "Error requesting status message\n");
//            }
//            break;
//        case 's':
//            if (cwiid_get_state(wiimote, &state)) {
//                fprintf(stderr, "Error getting state\n");
//            }
//            print_state(&state);
//            break;
//        case 't':
//            toggle_bit(rpt_mode, CWIID_RPT_STATUS);
//            set_rpt_mode(wiimote, rpt_mode);
//            break;
//        case 'x':
//            exit = -1;
//            break;
//        case '\n':
//            break;
//        default:
//            fprintf(stderr, "invalid option\n");
//        }
//    }

//    if (cwiid_close(wiimote)) {
//        fprintf(stderr, "Error on wiimote disconnect\n");
//        return -1;
//    }

//    return 0;
//}
