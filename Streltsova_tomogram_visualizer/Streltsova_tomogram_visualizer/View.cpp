#include "View.h"
#include "Dialog.h"
#include <qfiledialog>

int clamp(int value, int min, int max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	return value;
};

View::View(QWidget* parent) : QGLWidget(parent)
{
	layer = 0;
	//задаем размеры окна
	resizeGL(MIN_WIN_SIZE, MIN_WIN_SIZE);
	setFocus();
};
void View::LoadData(QString fileName)
{
	data.readFile(fileName);
	min = data.getMin();
	max = data.getMax();
	//установка размера окна
	resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
	update();
};

void View::initializeGL()
{
	//устанавливаем заполняющий цвет
	qglClearColor(Qt::white);
	//устанавливаем режим сглаживания
	glShadeModel(GL_SMOOTH);
	//задаем модельно-видовую матрицу
	glMatrixMode(GL_MODELVIEW);
	//загрузка единичной матрицы
	glLoadIdentity();
	glGenTextures(1, &VBOtexture);
};
void View::resizeGL(int nWidth, int nHeight)
{
	//установка режима камеры
	glMatrixMode(GL_PROJECTION);
	//загрузка единичной матрицы
	glLoadIdentity();
	//установка ортогонального преобразования
	switch (axis)
	{
	case 3:
		glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);
		break;
	case 2:
		glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getDepth() - 1, -1.0f, 1.0f);
		break;
	case 1:
		glOrtho(0.0f, data.getHeight() - 1, 0.0f, data.getDepth() - 1, -1.0f, 1.0f);
		break;
	}
	//установка окна обзора
	glViewport(0, 0, nWidth, nHeight);
	update();
};
void View::paintGL()
{
	qDebug() << "repaint" << visualization_state;
	//очистка экрана
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (visualization_state)
	{
	case VISUALIZATION_QUADS:
		this->setWindowTitle("Visualization Quads");
		VisualizationQuads();
		break;
	case VISUALIZATION_QUADSTRIP:
		this->setWindowTitle("Visualization QuadStrip");
		VisualizationQuadstrip();
		break;
	case VISUALIZATION_TEXTURE:
		this->setWindowTitle("Visualization Texture");
		VisualizationTexture();
		break;
	}
};

void View::Up()
{
	switch (axis)
	{
	case 3:
		if ((layer + 1) < data.getDepth())
			layer++;
		break;
	case 2:
		if ((layer + 1) < data.getHeight())
			layer++;
		break;
	case 1:
		if ((layer + 1) < data.getWidth())
			layer++;
		break;
	}
	qDebug() << layer;
	updateGL();
};
void View::Down()
{
	if ((layer - 1) >= 0)
		layer --;
	qDebug() << layer;
	updateGL();
};

void View::changeLayer()
{
	if (visualization_state == VISUALIZATION_TEXTURE)
	{
		genTextureImage();
		Load2DTexture();
	}
};
QColor View::TransferFunction(short value)
{
	int c = (value - min) * 255 / (max - min);
	return QColor(c, c, c);
};

void View::VisualizationQuads()
{
	QColor c;
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();
	switch (axis)
	{
	case 3:
		for (int y = 0; y < (h - 1); y++)
			for (int x = 0; x < (w - 1); x++)
			{
				glBegin(GL_QUADS);

				c = TransferFunction(data[layer * w * h + y * w + x]);
				qglColor(c);
				glVertex2i(x, y);

				c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
				qglColor(c);
				glVertex2i(x, (y + 1));

				c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
				qglColor(c);
				glVertex2i((x + 1), (y + 1));

				c = TransferFunction(data[layer * w * h + y * w + x + 1]);
				qglColor(c);
				glVertex2i((x + 1), y);
				glEnd();
			}
		break;
	case 2:
		for (int z = 0; z < (d - 1); z++)
			for (int x = 0; x < (w - 1); x++)
			{
				glBegin(GL_QUADS);

				c = TransferFunction(data[z * w * h + layer * w + x]);
				qglColor(c);
				glVertex2i(x, z);

				c = TransferFunction(data[(z + 1) * w * h + layer * w + x]);
				qglColor(c);
				glVertex2i(x, (z + 1));

				c = TransferFunction(data[(z + 1) * w * h + layer * w + x + 1]);
				qglColor(c);
				glVertex2i((x + 1), (z + 1));

				c = TransferFunction(data[z * w * h + layer * w + x + 1]);
				qglColor(c);
				glVertex2i((x + 1), z);
				glEnd();
			}
		break;
	case 1:
		for (int z = 0; z < (d - 1); z++)
			for (int y = 0; y < (h - 1); y++)
			{
				glBegin(GL_QUADS);

				c = TransferFunction(data[z * w * h + y * w + layer]);
				qglColor(c);
				glVertex2i(y, z);

				c = TransferFunction(data[(z + 1) * w * h + y * w + layer]);
				qglColor(c);
				glVertex2i(y, (z + 1));

				c = TransferFunction(data[(z + 1) * w * h + (y + 1) * w + layer]);
				qglColor(c);
				glVertex2i((y + 1), (z + 1));

				c = TransferFunction(data[z * w * h + (y + 1) * w + layer]);
				qglColor(c);
				glVertex2i((y + 1), z);
				glEnd();
			}
		break;
	}
};
void View::VisualizationQuadstrip()
{
	QColor c;
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();

	switch (axis)
	{
	case 3:
		for (int y = 0; y < h - 1; y++)
		{ 
			glBegin(GL_QUAD_STRIP);
			for (int x = 0; x < w; x++)
			{
				glBegin(GL_QUAD_STRIP);

				c = TransferFunction(data[layer * w * h + y * w + x]);
				qglColor(c);
				glVertex2i(x, y);

				c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
				qglColor(c);
				glVertex2i(x, (y + 1));
			}
			glEnd();
		}
		break;
	case 2:
		for (int z = 0; z < (d - 1); z++)
		{ 
			glBegin(GL_QUAD_STRIP);
			for (int x = 0; x < w; x++)
			{
				glBegin(GL_QUAD_STRIP);

				c = TransferFunction(data[z * w * h + layer * w + x]);
				qglColor(c);
				glVertex2i(x, z);

				c = TransferFunction(data[(z + 1) * w * h + layer * w + x]);
				qglColor(c);
				glVertex2i(x, (z + 1));
			}
			glEnd();
		}
		break;
	case 1:
		for (int z = 0; z < d - 1; z++)
		{ 
			glBegin(GL_QUAD_STRIP);
			for (int y = 0; y < h; y++)
			{
				glBegin(GL_QUAD_STRIP);

				c = TransferFunction(data[z * w * h + y * w + layer]);
				qglColor(c);
				glVertex2i(y, z);

				c = TransferFunction(data[(z + 1) * w * h + y * w + layer]);
				qglColor(c);
				glVertex2i(y, (z + 1));
			}
			glEnd();
		}
		break;
	}
};
void View::VisualizationTexture()
{
	glBegin(GL_QUADS);
	qglColor(QColor(255, 255, 255));

	glTexCoord2f(0, 0);
	glVertex2i(0, 0);

	switch (axis)
	{
	case 3:
		glTexCoord2f(0, 1);
		glVertex2i(0, data.getHeight());

		glTexCoord2f(1, 1);
		glVertex2i(data.getWidth(), data.getHeight());

		glTexCoord2f(1, 0);
		glVertex2i(data.getWidth(), 0);
		break;
	case 2:
		glTexCoord2f(0, 1);
		glVertex2i(0, data.getDepth());

		glTexCoord2f(1, 1);
		glVertex2i(data.getWidth(), data.getDepth());

		glTexCoord2f(1, 0);
		glVertex2i(data.getWidth(), 0);
		break;
	case 1:
		glTexCoord2f(0, 1);
		glVertex2i(0, data.getDepth());

		glTexCoord2f(1, 1);
		glVertex2i(data.getHeight(), data.getDepth());

		glTexCoord2f(1, 0);
		glVertex2i(data.getHeight(), 0);
		break;
	}
	glEnd();
};

void View::genTextureImage()
{
	int w = data.getWidth();
	int h = data.getDepth();
	int d = data.getDepth();

	switch (axis)
	{
	case 3:
		textureImage = QImage(w, h, QImage::Format_RGB32);
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
			{
				QColor c = TransferFunction(data[layer * w * h + y * w + x]);
				textureImage.setPixelColor(x, y, c);
			}
		break;
	case 2:
		textureImage = QImage(w, d, QImage::Format_RGB32);
		for (int z = 0; z < d; z++)
			for (int x = 0; x < w; x++)
			{
				QColor c = TransferFunction(data[z * w * h + layer * w + x]);
				textureImage.setPixelColor(x, z, c);
			}
		break;
	case 1:
		textureImage = QImage(h, d, QImage::Format_RGB32);
		for (int z = 0; z < d; z++)
			for (int y = 0; y < h; y++)
			{
				QColor c = TransferFunction(data[z * w * h + y * w + layer]);
				textureImage.setPixelColor(y, z, c);
			}
		break;
	}
};
void View::Load2DTexture()
{
	glBindTexture(GL_TEXTURE_2D, VBOtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
};
void View::keyPressEvent(QKeyEvent* event)
{
	if (event->nativeVirtualKey() == Qt::Key_W)
	{
		Up();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_S)
	{
		Down();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_N)
	{
		visualization_state = (visualization_state + 1) % 3;
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
			genTextureImage();
			Load2DTexture();
		}
		else
			glDisable(GL_TEXTURE_2D);
	}
	else if (event->nativeVirtualKey() == Qt::Key_Z)
	{
		axis = 3;
		layer = 0;

		resizeGL(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE),
			     clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			genTextureImage();
			Load2DTexture();
		}
	}
	else if (event->nativeVirtualKey() == Qt::Key_Y)
	{
		axis = 2;
		layer = 0;

		resizeGL(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE),
			     clamp(data.getDepth(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			genTextureImage();
			Load2DTexture();
		}
	}
	else if (event->nativeVirtualKey() == Qt::Key_X)
	{
		axis = 1;
		layer = 0;

		resizeGL(clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE),
			     clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			genTextureImage();
			Load2DTexture();
		}
	}
	else if (event->nativeVirtualKey() == Qt::Key_O)
	{
		layer = 0;
		changeLayer();
		QString path = DialogFile();
		LoadData(path);
	}
	else if (event->nativeVirtualKey() == Qt::Key_M)
	{
		Dialog* dialog = new Dialog;
		dialog->exec();

		if (dialog->getMax() > 0)
			max = dialog->getMax();
		if (dialog->getMin() > 0)
			min = dialog->getMin();

		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			genTextureImage();
			Load2DTexture();
		}

		delete dialog;
	}
	update();
};

QString View::DialogFile()
{
	QString path = QFileDialog::getOpenFileName(this, "Choose binary file", "", "*.bin");
	return path;
};