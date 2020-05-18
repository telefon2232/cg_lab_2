#include <QtOpenGL/QGLWidget>
#include "View.h"

View::View() {
}

View::~View(){
}

template <class T>//����� �� ����� �� ������� 1
T clamp(T v, int max, int min) {
	if (v > max) {
		return max;
	}
	if (v < min) {
		return min;
	}
	return v;
}

void View::input_layer(int value) {//�������, ������ ��� ����� ����� � �������
	if (value >= 2) {
		layerNumber = value;
	}
	else {
		layerNumber = 1;
	}

	updateGL();//� ����� �-��, ��� ���-�� ����������������, ������� ��� �-��
	if (mode == 2) {
		glEnable(GL_TEXTURE_2D);//�������� �����-�� ����������� gl
		genTextureImage(layerNumber);
		Load2dTexture();
		updateGL();
	}
	else {
		glDisable(GL_TEXTURE_2D);//��������� �����-�� ����������� gl
	}
}

void View::setmode(int value) {
	mode = value;
	if (mode == 2) {
		glEnable(GL_TEXTURE_2D);//�������� �����-�� ����������� gl
		genTextureImage(layerNumber);
		Load2dTexture();
		updateGL();
	}
	else {
		glDisable(GL_TEXTURE_2D);//��������� �����-�� ����������� gl
	}
	paintGL();
}

int TransferFunction(short value, int imin, int imax) {//��������� �������� ��������� ���������� � �����-����� ����
	int min = imin;
	int max = imax;
	int newVal = (value - min) * 255 / (max - min);
	return newVal;
}

void View::initializeGL() {//�������������
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//������� ������ �����������
	glGenTextures(1, &VBOtexture);//���������� ���������� ����� ��������, ������� ������������, ��� ����� ��� ������, ��������� � ���������
}

void View::resizeGL(int width, int height) {//���� ������
	glShadeModel(GL_SMOOTH);//� ���������� ������ ��� ���
	glMatrixMode(GL_PROJECTION);//����� ������� ��������
	glLoadIdentity();//���������������� ������� � �������� �� ��������� �� ���������(���������???)
	if (slaider_axis == "Z") {
		first = test.X;
		second = test.Y;
	}
	else if (slaider_axis == "X") {
		first = test.Y;
		second = test.Z;
	}
	else {
		first = test.X;
		second = test.Z;
	}
	cout << first << " " << second << endl;
	glOrtho(0, first, 0, second, -1, 1);//��������� ��������� ��������������� ������� ���������(���� �������� ��������)
	glViewport(0, 0, width, height);//������ �������������, ����� ������� �� ����� ��� ��������
}

void View::paintGL() {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	switch (mode) {
	case 0://��������� ������������������
		DrawQuads(layerNumber);
		break;
	case 1://����� 4 ����� ��������������, � ����� ��������� �� 2
		DrawQuadStrip();
		break;
	case 2:
		VizualizationTexture();//� ����������
		break;
	}
}

void View::DrawQuads(int layerNumber) {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	glBegin(GL_QUADS);//���������, ��� ����� �������� �������������

	if (slaider_axis == "Z") {
		for (int x_coord = 0; x_coord < test.X - 1; x_coord++) {
			for (int y_coord = 0; y_coord < test.Y - 1; y_coord++) {
				short value;
				int transfer;
				//1 �������
				value = test.array[x_coord + y_coord * test.X + layerNumber * test.X * test.Y];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(x_coord, y_coord);//��������� �������
				//2 �������
				value = test.array[x_coord + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(x_coord, (y_coord + 1));
				//3 �������
				value = test.array[(x_coord + 1) + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), (y_coord + 1));
				//4 �������
				value = test.array[(x_coord + 1) + y_coord * test.X + layerNumber * test.X * test.Y];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), y_coord);
			}
		}
	}
	else if (slaider_axis == "X") {
		for (int z_coord = 0; z_coord < test.Z - 1; z_coord++) {
			for (int y_coord = 0; y_coord < test.Y - 1; y_coord++) {
				short value;
				int transfer;
				//1 �������
				value = test.array[z_coord * test.X * test.Y + y_coord * test.X + layerNumber];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(y_coord, z_coord);//��������� �������
				//2 �������
				value = test.array[(z_coord + 1) * test.X * test.Y + y_coord * test.X + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(y_coord, (z_coord + 1));
				//3 �������
				value = test.array[(z_coord + 1) * test.X * test.Y + (y_coord + 1) * test.X + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((y_coord + 1), (z_coord + 1));
				//4 �������
				value = test.array[z_coord * test.X * test.Y + (y_coord + 1) * test.X + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((y_coord + 1), z_coord);
			}
		}
	}
	else {
		for (int z_coord = 0; z_coord < test.Z - 1; z_coord++) {
			for (int x_coord = 0; x_coord < test.X - 1; x_coord++) {
				short value;
				int transfer;
				//1 �������
				value = test.array[z_coord * test.X * test.Y + layerNumber * test.X + x_coord];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(x_coord, z_coord);//��������� �������
				//2 �������
				value = test.array[(z_coord + 1) * test.X * test.Y + layerNumber * test.X + x_coord];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(x_coord, (z_coord + 1));
				//3 �������
				value = test.array[(z_coord + 1) * test.X * test.Y + layerNumber * test.X + x_coord + 1];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), (z_coord + 1));
				//4 �������
				value = test.array[z_coord * test.X * test.Y + layerNumber * test.X + x_coord + 1];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), z_coord);
			}
		}
	}

	
	glEnd();//��������� ��������
}



void View::Load2dTexture() {//�������� �������� � ������ ����������
	glBindTexture(GL_TEXTURE_2D, VBOtexture);//����������� �������� � ������������� ����������� ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());//��������� �������� � ������ ����������(����� ���������, ������� ������� �����������?, ������(��������) ��������, ������, ������, �������, ������(��������) ��������, ��� ��������, ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//������������� ��������� ��� ������� ��������, ����������� � ����������� �����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//����� ��������, ��������������� ��� ��������� ������� ����(�����), �������� �����������(��� ������ �� ��������)
}

void View::genTextureImage(int layerNumber) {//��������� ����������������� ��������������
	int w = test.X;
	int h = test.Y;
	int d = test.Z;


	if (slaider_axis == "Z") {
		textureImage = QImage(w, h, QImage::Format_RGB32);
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int intensity = TransferFunction(test.array[layerNumber * w * h + w * y + x], min, max);
				QColor c = QColor::fromRgbF(intensity, intensity, intensity);
				textureImage.setPixelColor(x, y, c);
			}
		}
	}
	else if (slaider_axis == "X") {
		textureImage = QImage(h, d, QImage::Format_RGB32);
		for (int z = 0; z < d; z++) {
			for (int y = 0; y < h; y++) {
				int intensity = TransferFunction(test.array[z * w * h + w * y + layerNumber], min, max);
				QColor c = QColor::fromRgbF(intensity, intensity, intensity);
				textureImage.setPixelColor(y, z, c);
			}
		}
	}
	else {
		textureImage = QImage(w, d, QImage::Format_RGB32);
		for (int z = 0; z < d; z++) {
			for (int x = 0; x < w; x++) {
				int intensity = TransferFunction(test.array[z * w * h + w * layerNumber + x], min, max);
				QColor c = QColor::fromRgbF(intensity, intensity, intensity);
				textureImage.setPixelColor(x, z, c);
			}
		}
	}

	
}

void View::VizualizationTexture() {//������ ���� ������������� � ���������� ���������
	glBegin(GL_QUADS);
	qglColor(QColor(255, 255, 255));

	glTexCoord2f(0, 0);
	glVertex2i(0, 0);

	glTexCoord2f(0, 1);
	glVertex2i(0, second);

	glTexCoord2f(1, 1);
	glVertex2i(first, second);

	glTexCoord2f(1, 0);
	glVertex2i(first, 0);

	glEnd();
}


void View::DrawQuadStrip() {
	int w = test.X;
	int h = test.Y;
	int d = test.Z;
	if (slaider_axis == "Z") {
		for (int x = 0; x < w - 1; x++) {
			for (int y = 0; y < h - 1; y++) {
				short value;
				int transfer;
				glBegin(GL_QUAD_STRIP);
				//1 �������
				value = test.array[x + y * w + layerNumber * w * h];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(x, y);//��������� �������
				//2 �������
				value = test.array[x + (y + 1) * w + layerNumber * w * h];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(x, (y + 1));
				//3 �������
				value = test.array[(x + 1) + (y + 1) * w + layerNumber * w * h];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x + 1), (y + 1));
				//4 �������
				value = test.array[(x + 1) + y * w + layerNumber * w * h];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x + 1), y);
				glEnd();
			}
		}
	}
	else if (slaider_axis == "X") {
		for (int z = 0; z < d - 1; z++) {
			for (int y = 0; y < h - 1; y++) {
				short value;
				int transfer;
				glBegin(GL_QUAD_STRIP);
				//1 �������
				value = test.array[z * w * h + y * w + layerNumber];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(y, z);//��������� �������
				//2 �������
				value = test.array[(z + 1) * w * h + y * w + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(y, (z + 1));
				//3 �������
				value = test.array[(z + 1) * w * h + (y + 1) * w + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((y + 1), (z + 1));
				//4 �������
				value = test.array[z * w * h + (y + 1) * w + layerNumber];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((y + 1), z);
				glEnd();
			}
		}
	}
	else {
		for (int z = 0; z < d - 1; z++) {
			for (int x = 0; x < w - 1; x++) {
				short value;
				int transfer;
				glBegin(GL_QUAD_STRIP);
				//1 �������
				value = test.array[z * w * h + layerNumber * w + x];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(x, z);//��������� �������
				//2 �������
				value = test.array[(z + 1) * w * h + layerNumber * w + x];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(x, (z + 1));
				//3 �������
				value = test.array[(z + 1) * w * h + layerNumber * w + x + 1];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x + 1), (z + 1));
				//4 �������
				value = test.array[z * w * h + layerNumber * w + x + 1];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x + 1), z);
				glEnd();
			}
		}
	}
}

