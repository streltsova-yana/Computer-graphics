#pragma once
#include <QImage>
class Filter
{
public:
	Filter() {};
	~Filter() {};
	virtual QImage calculateNewImagePixMap(const QImage& photo) = 0;
};

//Point filters
class Invert_filter : public Filter
{
public:
	Invert_filter() {};
	~Invert_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Gray_scale_filter : public Filter
{
public:
	Gray_scale_filter() {};
	~Gray_scale_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Sepia_filter : public Filter
{
public:
	Sepia_filter() {};
	~Sepia_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Bright_filter : public Filter
{
public:
	Bright_filter() {};
	~Bright_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Shift_filter : public Filter
{
public:
	Shift_filter() {};
	~Shift_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Rotate_filter : public Filter
{
public:
	Rotate_filter() {};
	~Rotate_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Horizontal_waves_filter : public Filter
{
public:
	Horizontal_waves_filter() {};
	~Horizontal_waves_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Vertical_waves_filter : public Filter
{
public:
	Vertical_waves_filter() {};
	~Vertical_waves_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

//Matrix Filters
class Matrix_filter : public Filter
{
public: 
	float* vector;
	int mRadius;

	Matrix_filter(int radius = 1) : mRadius(radius) {};
	~Matrix_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class Blur_filter : public Matrix_filter
{
public:
	Blur_filter(int radius);
	~Blur_filter() {};
};

class Gaussian_blur_filter : public Matrix_filter
{
public:
	Gaussian_blur_filter(int radius = 3, int sigma = 2)
	{
		createGaussianVector(radius, sigma);
	};
	~Gaussian_blur_filter() {};
	void createGaussianVector(int radius, int sigma);
};

class Sobel_filter : public Matrix_filter
{
public:
	Sobel_filter()
	{
		createSobelVector();
	};
	~Sobel_filter() {};
	void createSobelVector();
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Sharpness_filter : public Matrix_filter
{
public:
	Sharpness_filter()
	{
		createSharpnessVector();
	};
	~Sharpness_filter() {};
	void createSharpnessVector();
};

class Sharpen_filter : public Matrix_filter
{
public:
	Sharpen_filter()
	{
		createSharpenVector();
	};
	~Sharpen_filter() {};
	void createSharpenVector();
};

class Median_filter : public Matrix_filter
{
public:
	Median_filter(int radius = 1)
	{
		mRadius = radius;
	};
	~Median_filter() {};
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Maximum_filter : public Matrix_filter
{
public:
	Maximum_filter(int radius = 1)
	{
		mRadius = radius;
	};
	~Maximum_filter() {};
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Glowing_edges_filter : public Matrix_filter
{
public:
	Glowing_edges_filter(int radius = 1)
	{
		mRadius = radius;
	};
	~Glowing_edges_filter() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Dilation : public Matrix_filter
{
public:
	Dilation(int radius, float* _vector)
	{
		createDilationVector(radius, _vector);
	};
	~Dilation() {};
	void createDilationVector(int radius, float* _vector);
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Erosion : public Matrix_filter
{
public:
	Erosion(int radius, float* _vector)
	{
		createErosionVector(radius, _vector);
	};
	~Erosion() {};
	void createErosionVector(int radius, float* _vector);
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Opening : public Matrix_filter
{
public:
	Opening(int radius, float* _vector)
	{
		createOpeningVector(radius, _vector);
	};
	~Opening() {};
	void createOpeningVector(int radius, float* _vector);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Closing : public Matrix_filter
{
public:
	Closing(int radius, float* _vector)
	{
		createClosingVector(radius, _vector);
	};
	~Closing() {};
	void createClosingVector(int radius, float* _vector);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Grad : public Matrix_filter
{
public:
	Grad(int radius, float* _vector)
	{
		createGradVector(radius, _vector);
	};
	~Grad() {};
	void createGradVector(int radius, float* _vector);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Black_hat : public Matrix_filter
{
public:
	Black_hat(int radius, float* _vector)
	{
		createBlack_hatVector(radius, _vector);
	};
	~Black_hat() {};
	void createBlack_hatVector(int radius, float* _vector);
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Autolevels : public Matrix_filter
{
public:
	Autolevels() {};
	~Autolevels() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Perfect_reflector : public Matrix_filter
{
public:
	Perfect_reflector() {};
	~Perfect_reflector() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};

class Gray_world : public Matrix_filter
{
public:
	Gray_world() {};
	~Gray_world() {};
	QImage calculateNewImagePixMap(const QImage& photo);
};
