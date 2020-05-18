#include <QApplication>
#include <QGLWidget>
#include <iostream>
#include <string>
#include <stdio.h>
#include <qimage.h>
#include <QtOpenGL/QGLWidget>
#include "Bin.h"
#include "View.h"
#include "Window.h"
using namespace std;

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	
	Window mainWindow;
	mainWindow.showMaximized();


	return a.exec();
}
