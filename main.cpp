#include <QApplication>
#include <QQmlApplicationEngine>
#include "Datalink.h"
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QQuickWindow>

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
    return app.exec();
}
