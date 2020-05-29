#include "ShaderWidget.h"

ShaderWidget::ShaderWidget(QWidget* parent) : QOpenGLWidget(parent), vert_data_location(0)
{
	vert_data = new GLfloat[12]{ -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
};
ShaderWidget::~ShaderWidget()
{
	delete[] vert_data;
};
void ShaderWidget::initializeGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	QOpenGLShader vShader(QOpenGLShader::Vertex);
	vShader.compileSourceFile("raytracing.vert");

	QOpenGLShader fShader(QOpenGLShader::Fragment);
	fShader.compileSourceFile("raytracing.frag");

	m_program.addShader(&vShader);
	m_program.addShader(&fShader);
	if (!m_program.link())
	{
		qWarning("Error link programm shader");
		return;
	}

	vert_data_location = m_program.attributeLocation("vertex");

	qDebug() << QString("Log programm");
	qDebug() << m_program.log();

	if (!m_program.bind())
		qWarning("Error bind programm shader");
	m_program.setUniformValue("camera.position", position);
	m_program.setUniformValue("camera.view", QVector3D(0.0, 0.0, 1.0));
	m_program.setUniformValue("camera.up", QVector3D(0.0, 1.0, 0.0));
	m_program.setUniformValue("camera.side", QVector3D(1.0, 0.0, 0.0));

	m_program.setUniformValue("spherein[0].center", QVector3D(-1.0, 1.0, 0.0));
	m_program.setUniformValue("spherein[0].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("spherein[0].radius", 1.0f);
	m_program.setUniformValue("spherein[1].center", QVector3D(0.0, -1.0, 0.0));
	m_program.setUniformValue("spherein[1].color", QVector3D(0.8, 0.64, 0.87));
	m_program.setUniformValue("spherein[1].radius", 2.0f);
	m_program.setUniformValue("spherein[2].center", QVector3D(2.0, 2.0, 3.0));
	m_program.setUniformValue("spherein[2].color", QVector3D(0.0, 1.0, 0.0));
	m_program.setUniformValue("spherein[2].radius", 1.0f);

	m_program.setUniformValue("scale", QVector2D(width(), height()));
	m_program.release();
};
void ShaderWidget::resizeGL(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
	if (!m_program.bind())
		qWarning("Error bind program shader");
	m_program.setUniformValue("scale", QVector2D(width(), height()));
	m_program.release();
};
void ShaderWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (!m_program.bind())
		return;

	m_program.enableAttributeArray(vert_data_location);
	m_program.setAttributeArray(vert_data_location, vert_data, 3);
	glDrawArrays(GL_QUADS, 0, 4);
	m_program.disableAttributeArray(vert_data_location);
	m_program.release();
};
void ShaderWidget::keyPressEvent(QKeyEvent* event)
{
	if (!m_program.bind())
		qWarning("Error bind programm shader");
	float x = position.x();
	float y = position.y();
	float z = position.z();
	if (event->nativeVirtualKey() == Qt::Key_X)
	{
		position = { -7, 0, 0 };
		m_program.setUniformValue("camera.view", QVector3D(1.0, 0.0, 0.0));
		m_program.setUniformValue("camera.up", QVector3D(0.0, 1.0, 0.0));
		m_program.setUniformValue("camera.side", QVector3D(0.0, 0.0, -1.0));
		axis = 1;
	}
	else if (event->nativeVirtualKey() == Qt::Key_Y)
	{
		position = { 1, 8, 1 };
		m_program.setUniformValue("camera.view", QVector3D(0.0, -1.0, 0.0));
		m_program.setUniformValue("camera.up", QVector3D(0.0, 0.0, 1.0));
		m_program.setUniformValue("camera.side", QVector3D(1.0, 0.0, 0.0));
		axis = 2;
	}
	else if (event->nativeVirtualKey() == Qt::Key_Z)
	{
		position = { 0, 0, -7 };
		m_program.setUniformValue("camera.view", QVector3D(0.0, 0.0, 1.0));
		m_program.setUniformValue("camera.up", QVector3D(0.0, 1.0, 0.0));
		m_program.setUniformValue("camera.side", QVector3D(1.0, 0.0, 0.0));
		axis = 3;
	}
	else if (event->nativeVirtualKey() == Qt::Key_W)
	{
		if (axis == 2) 
			position.setZ(++z);
		else
			position.setY(++y);
	}
	else if (event->nativeVirtualKey() == Qt::Key_S)
	{
		if (axis == 2)
			position.setZ(--z);
		else
			position.setY(--y);
	}
	else if (event->nativeVirtualKey() == Qt::Key_Q)
	{
		if (axis == 1)
			position.setX(--x);
		else if (axis == 2)
			position.setY(++y);
		else
			position.setZ(--z);
	}
else if (event->nativeVirtualKey() == Qt::Key_E)
	{
		if (axis == 1)
			position.setX(++x);
		else if (axis == 2)
			position.setY(--y);
		else
			position.setZ(++z);
	}
else if (event->nativeVirtualKey() == Qt::Key_D)
	{
		if (axis == 1)
			position.setZ(--z);
		else
			position.setX(++x);
	}
else if (event->nativeVirtualKey() == Qt::Key_A)
	{
		if (axis == 1)
			position.setZ(++z);
		else
			position.setX(--x);
	}
	qDebug() << "Position: " << x  << " " << y << " " << z;
	m_program.setUniformValue("camera.position", position);
	m_program.setUniformValue("scale", QVector2D(width(), height()));
	m_program.release();
	update();
}