//
// Created by Arman on 10/30/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"

#include "Scope.h"
#include "ui_mainwindow.h"


mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    scope=new Scope(this,ui->customePlot);

}

mainwindow::~mainwindow() {
    delete ui;
}
