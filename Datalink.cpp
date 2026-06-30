#include <Datalink.h>
// #include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThreadPool>
#include <QtMath>
// #include <QMessageBox>
// #include <QTextStream>
Datalink::Datalink(QObject *parent)
    : QObject{parent} , km(0),qs(),mseries(new QList<QList<QPointF>>(10)),mem(new QByteArray()){
    (*mseries)[0]=QList<QPointF>(10);
    for (int i=0;i<10;i++){
        (*mseries)[i]=QList<QPointF>(100);
        // (*mseries)[0][i]=QPointF(i,2*i);
    }
};
int Datalink::k(){
    return km;
}
void Datalink::setK(int a) {
    km=a;
    emit kChanged();
    return;
}
QString Datalink::showserials(){
    QString out;  // Don't use 'new' on the heap - use stack allocation
    const auto ports = QSerialPortInfo::availablePorts();
    out.append("Number of ports: " + QString::number(ports.size())+"\n");
    for (const QSerialPortInfo &port : ports) {
        out.append(" Port: " + port.portName() +
                   " Description: " + port.description() +
                   " Manufacturer: " + port.manufacturer()+"\n");
    }
    return out;
}
Datalink::~Datalink(){
    qs->close();
    delete qs;
}
int Datalink::intializeserial(){
    qs = new QSerialPort();
    // qs->setPort(QSerialPortInfo("\\\\.\\CNCA0"));
    qs->setPortName("\\\\.\\CNCA0");
    // qDebug()<<QSerialPortInfo("\\\\.\\CNCA0").portName();
    qs->setBaudRate(9600);
    qs->setStopBits(QSerialPort::OneStop);
    qs->setDataBits(QSerialPort::Data8);
    qs->setParity(QSerialPort::NoParity);
    qs->setFlowControl(QSerialPort::NoFlowControl);
    qs->open(QIODeviceBase::ReadWrite);
    qs->write("hello");
    return qs->error();
}
void Datalink::write(QString text){
    qs->write(text.toUtf8());
}
QByteArray Datalink::read(int n){
    return qs->read(n);
}
void Datalink::pthread(){
    qDebug()<<this->thread();
    return;
}
QList<QList<QPointF>> *Datalink::series(){
    return mseries;
}
QList<QPointF> Datalink::getseries(int i){
    return (*mseries)[i];
}
void Datalink::getinput(){
    QByteArray a,temp;
    // qDebug()<<"temp: "<<temp;
    a.append(*mem);
    mem->clear();
    int len,first;
    a.append(qs->readAll());
    // qDebug()<<"a: "<<a;
    bool rdi=false;//dr  :  during data conversion,rdi  :  read data point for index
    char ind=0;
    for (int i=0;i<a.size();i++){
        if(rdi){ind=a.at(i);rdi=false;}
        if(a.at(i)==-1){
            // qDebug()<<"found -128";
            if(i&&temp.at(0)==-1){
            //todo later temp to list<point>
                int len=temp.size()/2;
            (*mseries)[ind].resizeForOverwrite(len-1);
                for(int j=1;j<len;j++){
                    // qDebug()<<"doing it -128:"<<j<<" indice: "<<int(ind);
                (*mseries)[ind][j-1]=QPointF(static_cast<unsigned char>(temp[2*j]),static_cast<unsigned char>(temp[2*j+1]));
                    // qDebug()<<(*mseries)[ind][j-1];
                }
                // (*mseries)[ind].resize(len/2);
                temp.clear();
            }
            rdi=true;
        }
        temp.append(a.at(i));
        // qDebug()<<"temp: " <<temp;
    }
    if(temp[0]!=-1){
        first=temp.indexOf(-1);
        if(first!=-1)
        mem->append(temp.mid(first));
    } else{
    mem->append(temp);
    }
// qDebug()<<(*mem);
}
