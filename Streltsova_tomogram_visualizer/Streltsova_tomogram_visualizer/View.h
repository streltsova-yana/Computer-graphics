#pragma comment(lib, "opengl32.lib")
#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include "qimage.h"
#include <algorithm>
#include "Data.h"

class View : public QGLWidget
{
	Q_OBJECT
private:
	Data data;
	int layer;
	int axis = 3;
	int visualization_state = 1;
	short min, max;

	static const int MAX_WIN_SIZE = 1000;
	static const int MIN_WIN_SIZE = 250;

	static const int VISUALIZATION_QUADS = 0;
	static const int VISUALIZATION_QUADSTRIP = 1;
	static const int VISUALIZATION_TEXTURE = 2;

	void Up();
	void Down();

	void changeLayer();
	QColor TransferFunction(short value);

	void VisualizationQuads();
	void VisualizationQuadstrip();
	void VisualizationTexture();
	void genTextureImage();

	//для текстурирования
	QImage textureImage;
	GLuint VBOtexture;
	void Load2DTexture();

	QString DialogFile();
protected:
	void initializeGL() override;
	void resizeGL(int nWidth, int nHeight) override;
	void paintGL() override;

public:
	View(QWidget* parent = Q_NULLPTR);
	void LoadData(QString fileName);
	~View() {};

public slots:
	void keyPressEvent(QKeyEvent* event) override;
};
