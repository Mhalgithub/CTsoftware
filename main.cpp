#include <QApplication>
#include <QQmlApplicationEngine>
#include "Datalink.h"
#include "simplehdlc.h"
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QQuickWindow>
struct RxData {
    uint8_t buffer[200];
    size_t len;
};
void rx_packet_callback(const uint8_t *payload, uint16_t len, void *user_ptr)
{
    RxData *rx = static_cast<RxData *>(user_ptr);

    rx->len = len;

    for (size_t i = 0; i < len; i++) {
        rx->buffer[i] = payload[i];
    }

    qDebug() << "Received packet, length =" << len;

    char text[1000] = {0};
    int pos = 0;

    for (size_t i = 0; i < len; i++) {
        pos += sprintf(text + pos, "%02X ", payload[i]);
    }

    qDebug() << text;
}

int main(int argc, char *argv[])

{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/CTsoftware.ico"));
    // qDebug()<<app.windowIcon();
    // Get executable directory
    QString exeDir = QCoreApplication::applicationDirPath();

    // Set custom paths relative to executable
    qputenv("QT_USER_DATA_PATH", (exeDir + "/appdata").toUtf8());
    qputenv("QT_USER_CACHE_PATH", (exeDir + "/cache").toUtf8());

    // Create directories
    QDir().mkpath(exeDir + "/appdata");
    QDir().mkpath(exeDir + "/cache");
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    Datalink datalink;
    qmlRegisterSingletonInstance("CTsoftware", 1, 0, "Datalink", &datalink);
    qDebug()<<Datalink::showserials();
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    engine.loadFromModule("CTsoftware", "Main");
    QVector<QPointF> *pl=new QVector<QPointF>();
    // QVector<int> *a=new QVector<int>;
    pl->append(QPointF(5,8));
    pl->append(QPointF(5,84));
    pl->append(QPointF(1,38));
    pl->append(QPointF(2,5));
    pl->append(QPointF(6,5));
    pl->append(QPointF(60,5));
    pl->append(QPointF(55,5));
    pl->append(QPointF(0,5));
    std::qsort(pl->data(),pl->count(),sizeof(QPointF),[](const void* a, const void* b){
        QPointF aa=*static_cast<const QPointF*> (a);
        QPointF bb=*static_cast<const QPointF*>(b);
        if(aa.x()>bb.x())return 1;
        if(aa.x()<bb.x())return -1;
        return 0;
    });
    QLineSeries *a=new QLineSeries();
    a->replace(*pl);
    uint8_t o[200] = {0};

    uint8_t x = 199;

    size_t encoded_size;

    simplehdlc_encode_to_buffer(
        o,
        sizeof(o),
        &encoded_size,
        &x,
        1
        );

    qDebug() << "Encoded size =" << encoded_size;

    char encoded_text[1000] = {0};
    int pos = 0;

    for (size_t i = 0; i < encoded_size; i++) {
        pos += sprintf(encoded_text + pos, "%02X ", o[i]);
    }

    qDebug() << "Encoded:" << encoded_text;


    // -------------------------------
    // Receiver
    // -------------------------------

    uint8_t parse_buffer[200];

    RxData received = {};

    simplehdlc_callbacks_t callbacks = {};
    callbacks.rx_packet_callback = rx_packet_callback;

    simplehdlc_context_t receiver;

    simplehdlc_init(
        &receiver,
        parse_buffer,
        sizeof(parse_buffer),
        &callbacks,
        &received
        );


    // Feed the encoded packet to the receiver
    simplehdlc_parse(
        &receiver,
        o,
        encoded_size
        );


    return app.exec();
}

