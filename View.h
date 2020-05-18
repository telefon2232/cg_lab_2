#pragma once
#include "Bin.h"

class View : public QGLWidget {
public:
	Bin test;
	int layerNumber;
	GLuint VBOtexture;//номер текстуры в памяти видеокарты
	QImage textureImage;//для создания текстуры нужно
	int mode = 0;//режим отрисовки изображения
	int slider_mode = 0;//по какой оси перематываем слои, 0 - Z, 1 - X, 2 - Y
	int min = 0;//минимальное значение плотности
	int max = 2000;//максимальное значение плотности
	string slaider_axis;//показываем какую ось будем крутить на ползунке слайдера
	int first, second;//длина-ширина
	View();
	View(string path, QWidget* parent): QGLWidget(parent){
		test.readBIN(path);
		layerNumber = 1;
	}
	~View();
	void DrawQuads(int);
	virtual void initializeGL();
	virtual void resizeGL(int, int);
	virtual void paintGL();
	void input_layer(int);
	void Load2dTexture();
	void genTextureImage(int);
	void VizualizationTexture();
	void setmode(int);
	void DrawQuadStrip();
;};
	