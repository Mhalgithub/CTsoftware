#include <QApplication>
#include <qrandom.h>

#include "mainwindow.h"
#include "Scope.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    mainwindow w;
    auto scope=w.scope;
         QVector<double> x(1000), y1(1000), y2(1000);
         for (int i = 0; i < 1000; ++i) {
             x[i] = (i-500) / 100.0;
             y1[i] = 5 * sin(2 * M_PI * 0.5 * x[i]); // 0.5 Hz sine wave
             y2[i] = 3 * cos(2 * M_PI * 1.0 * x[i]) + QRandomGenerator::global()->bounded(1.0); // 1.0 Hz cosine with noise
         }

        // Add signals to scope
        scope->addSignal(x, y1, "Sine Wave", Qt::blue);
        scope->addSignal(x, y2, "Noisy Cosine", Qt::red);
    //_______________________________________________________________________________________
        scope->enableCursors(true);

    w.show();
    return QApplication::exec();
}
