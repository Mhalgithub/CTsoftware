#ifndef DATALINK_H
#define DATALINK_H
#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLineSeries>
class Datalink : public QObject
{
    Q_OBJECT;
    QML_ELEMENT;
    QML_SINGLETON;
    Q_PROPERTY(int k READ k WRITE setK NOTIFY kChanged FINAL);
    Q_PROPERTY(QList<QList<QPointF>>* series READ series NOTIFY seriesChanged FINAL);
private :
    int km;
    QList<QList<QPointF>> *mseries;
    QSerialPort *qs;
    QByteArray *mem;
public:
    QList<QList<QPointF>>* series();
    explicit Datalink (QObject *parent=nullptr);
    ~Datalink();
    void setK(int a);
    Q_INVOKABLE QList<QPointF> getseries(int i);
    // void setSeries(const QList<QLineSeries>* a);
    int k();
signals:
    void kChanged();
    void seriesChanged();
public slots :
    static QString showserials();
    int intializeserial();
    void write(QString text);
    QByteArray read(int n);
    void pthread();
    void getinput();

};
#endif
