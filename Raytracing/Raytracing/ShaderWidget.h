#pragma once
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QKeyEvent>
#include <iostream>

class ShaderWidget : public QOpenGLWidget
{
private:
	QOpenGLShaderProgram m_program;
	GLfloat* vert_data; 
	int vert_data_location;
	int axis = 3;
	QVector3D position{ 0, 0, -7 };
protected:
	void initializeGL() override;
	void resizeGL(int nWidth, int nHeight) override;
	void paintGL() override;
public:
	ShaderWidget(QWidget* parent = 0);
	~ShaderWidget();
public slots:
	void keyPressEvent(QKeyEvent* event) override;
};