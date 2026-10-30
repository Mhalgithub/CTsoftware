//
// Created by Arman on 10/30/2026.
//

#ifndef SCOPE_MAINWINDOW_H
#define SCOPE_MAINWINDOW_H

#include <QMainWindow>

class Scope;
QT_BEGIN_NAMESPACE

namespace Ui {
    class mainwindow;
}

QT_END_NAMESPACE

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);

    ~mainwindow() override;
    Scope* scope;//=nullptr;
private:
    Ui::mainwindow *ui;


};



#endif //SCOPE_MAINWINDOW_H
