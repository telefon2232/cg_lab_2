#include "Window.h"


Window::Window(QWidget *parent): QMainWindow(parent){

	bool ok;
	QString iname = QInputDialog::getText(this, tr("input file name"), tr("Your file name:"), QLineEdit::Normal, "", &ok);//спрашиваем бинарный файл
	string iname_string = iname.toStdString();//преобразовали QString к string
	cout << iname_string << endl;


	example = new View(iname_string, this);
	example->setGeometry(200, 350, 200, 200);

	int imin = QInputDialog::getInt(this, tr("input min transferfunction"), tr("Your min value:"), 0, 0, 5000, 1, &ok);//спрашиваем минимум плотности
	if (ok)//если человек нажал окей
		example->min = imin;

	int imax = QInputDialog::getInt(this, tr("input max transferfunction"), tr("Your max value:"), 2000, 0, 5000, 1, &ok);//спрашиваем максимум плотности
	if (ok)//если человек нажал окей
		example->max = imax;

	int imode = QInputDialog::getInt(this, tr("input mode number(0-DrawQuads, 1-DrawQuadStrip, 2-Textures)"), tr("Your mode number:"), 0, 0, 2, 1, &ok);//спрашиваем режим
	if (ok)//если человек нажал окей
		modenumber = imode;

	QString iaxis = QInputDialog::getText(this, tr("input axis(X, Y or Z)"), tr("Your slider axis:"), QLineEdit::Normal, "", &ok);//спрашиваем какую ось человек хочет перематывать
	string iaxis_string = iaxis.toStdString();//преобразовали QString к string

	//кнопка открыть файл
	setFixedSize(1200, 1200);
	m_button = new QPushButton("Open File", this);
	m_button->setGeometry(QRect(QPoint(300, 270), QSize(200, 50)));
	connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));

	//определяем, какую ось(слои) будем менять
	if (iaxis_string == "Z") {
		//ползунок для слоев по Z
		QLabel *labelZ = new QLabel(this);
		labelZ->setText("Zslider");
		labelZ->setGeometry(QRect(QPoint(100, 20), QSize(40, 10)));

		sliderZ = new QSlider(Qt::Horizontal, this);
		int max_count_of_layers = example->test.Z;
		sliderZ->setRange(0, max_count_of_layers);
		sliderZ->setGeometry(QRect(QPoint(100, 40), QSize(500, 50)));
		connect(sliderZ, &QSlider::valueChanged, this, &Window::sliderZ_function);
		example->slaider_axis = "Z";
		
	}
	else if (iaxis_string == "X") {
		//ползунок для слоев по X
		QLabel *labelX = new QLabel(this);
		labelX->setText("Xslider");
		labelX->setGeometry(QRect(QPoint(100, 100), QSize(40, 10)));

		sliderX = new QSlider(Qt::Horizontal, this);
		int max_count_of_layers = example->test.X;
		sliderX->setRange(0, max_count_of_layers);
		sliderX->setGeometry(QRect(QPoint(100, 120), QSize(1000, 50)));
		connect(sliderX, &QSlider::valueChanged, this, &Window::sliderX_function);
		example->slaider_axis = "X";
	}
	else if (iaxis_string == "Y") {
		//ползунок для слоев по Y
		QLabel *labelY = new QLabel(this);
		labelY->setText("Yslider");
		labelY->setGeometry(QRect(QPoint(100, 190), QSize(40, 10)));

		sliderY = new QSlider(Qt::Horizontal, this);
		int max_count_of_layers = example->test.Y;
		sliderY->setRange(0, max_count_of_layers);
		sliderY->setGeometry(QRect(QPoint(100, 210), QSize(1000, 50)));
		connect(sliderY, &QSlider::valueChanged, this, &Window::sliderY_function);
		example->slaider_axis = "Y";
	}
	
	
}

Window::~Window(){

}

void Window::handleButton(){
	example->setmode(modenumber);//указываю режим
	example->resize(example->first, example->second);
	example->show();
}

//смена значений слоя при сдвиге ползунка
void Window::sliderZ_function() {
	int value = sliderZ->value();
	example->input_layer(value - 1);
}

void Window::sliderX_function() {
	int value = sliderX->value();
	example->input_layer(value - 1);
}

void Window::sliderY_function() {
	int value = sliderY->value();
	example->input_layer(value - 1);
}