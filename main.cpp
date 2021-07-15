#include "mainwindow.h"
#include<QPushButton>
#include <QApplication>
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<iomanip>
#include<fstream>
#include <stdio.h>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
