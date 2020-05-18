#pragma once
#include "Bin.h"

class View : public QGLWidget {
public:
	Bin test;
	int layerNumber;
	GLuint VBOtexture;//����� �������� � ������ ����������
	QImage textureImage;//��� �������� �������� �����
	int mode = 0;//����� ��������� �����������
	int slider_mode = 0;//�� ����� ��� ������������ ����, 0 - Z, 1 - X, 2 - Y
	int min = 0;//����������� �������� ���������
	int max = 2000;//������������ �������� ���������
	string slaider_axis;//���������� ����� ��� ����� ������� �� �������� ��������
	int first, second;//�����-������
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
	