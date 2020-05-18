#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QInputDialog>
#include <QGLWidget>
#include <QLabel>
#include "Bin.h"
#include "View.h"


class Window: public QMainWindow{
	Q_OBJECT//макрос Q_OBJECT, т.к. тут реализуются свои собственные сигналы и слоты
public:
	explicit Window(QWidget *parent = 0);
	~Window();
	QPushButton *m_button;
	QSlider *sliderZ;
	QSlider *sliderX;
	QSlider *sliderY;
	View* example;
	QLabel *labelZ;
	QLabel *labelX;
	QLabel *labelY;
	int modenumber;
private slots:
	void handleButton();
	void sliderZ_function();
	void sliderX_function();
	void sliderY_function();
};

