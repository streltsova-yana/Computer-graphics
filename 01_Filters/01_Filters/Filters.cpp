#include "Filters.h"
template <class T>
T clamp(T v, int max, int min)
{
	if (v > max)
		return max;
	else if (v < min)
		return min;
	return v;
}

//Point filters
QImage Invert_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);
			photo_color.setRgb(255 - photo_color.red(), 255 - photo_color.green(), 255 - photo_color.blue());
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}

QImage Gray_scale_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);
			int intensity = 0.36 * photo_color.red() + 0.53 * photo_color.green() + 0.11 * photo_color.blue();
			photo_color.setRgb(intensity, intensity, intensity);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Sepia_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);
			int intensity = 0.36 * photo_color.red() + 0.53 * photo_color.green() + 0.11 * photo_color.blue();
			int k = 20;
			int red = clamp<int>(intensity + 2 * k, 255, 0);
			int green = clamp<int>(intensity + 0.5 * k, 255, 0);
			int blue = clamp<int>(intensity - 1 * k, 255, 0);
			photo_color.setRgb(red, green, blue);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Bright_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);
			int c = 20;
			int red = clamp<int>(photo_color.red() + c, 255, 0);
			int green = clamp<int>(photo_color.green() + c, 255, 0);
			int blue = clamp<int>(photo_color.blue() + c, 255, 0);
			photo_color.setRgb(red, green, blue);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Shift_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			int c = 50;
			QColor photo_color;
			if ((x + c > photo.width() - 1) || (x + c < 0))
				photo_color.setRgb(0, 0, 0);
			else
				photo_color = photo.pixelColor(x + c, y);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Rotate_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			float u = 1.57;
			int x0 = photo.width() / 2;
			int y0 = photo.height() / 2;
			int x_new = (x - x0) * cos(u) - (y - y0) * sin(u) + x0;
			int y_new = (x - x0) * sin(u) + (y - y0) * cos(u) + y0;
			QColor photo_color;
			if ((x_new > photo.width() - 1) || (x_new < 0) || (y_new > photo.height() - 1) || (y_new < 0))
				photo_color.setRgb(0, 0, 0);
			else
				photo_color = photo.pixelColor(x_new, y_new);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Horizontal_waves_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			int x_new = x + 20 * sin(2 * 3.14 * y / 60);
			QColor photo_color = photo.pixelColor(clamp<int>(x_new, photo.width() - 1, 0), y);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

QImage Vertical_waves_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			int x_new = x + 20 * sin(2 * 3.14 * x / 30);
			QColor photo_color = photo.pixelColor(clamp<int>(x_new, photo.width() - 1, 0), y);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

//Matrix Filters
QColor Matrix_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	int size = 2 * mRadius + 1;
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++)
		{
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), 
				                            clamp<int>(y + i, photo.height() - 1, 0));
			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}
	return QColor(clamp(returnR, 255, 0),
		          clamp(returnG, 255, 0),
		          clamp(returnB, 255, 0));
};

QImage Matrix_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}

Blur_filter::Blur_filter(int radius)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			vector[i * size + j] = 1.0f / (size * size);
}

void Gaussian_blur_filter::createGaussianVector(int radius, int sigma)
{
	mRadius = radius;
	const unsigned int size = 2 * mRadius + 1;
	float norm = 0;
	vector = new float[size * size];
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++)
		{
			int idx = (i + mRadius) * size + j + mRadius;
			vector[idx] = exp(-(i * i + j * j) / (sigma * sigma));
			norm += vector[idx];
		}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			vector[i * size + j] /= norm;
};

void Sobel_filter::createSobelVector()
{
	mRadius = 1;
	vector = new float[18];
	float vec[] = { -1, -2,  -1, 0, 0, 0, 1, 2, 1, -1, 0,  1, -2, 0, 2, -1, 0, 1 };
	for (int i = 0; i < 18; i++) 
		vector[i] = vec[i];
};

QColor Sobel_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	int returnRx = 0;
	int returnGx = 0;
	int returnBx = 0;
	int returnRy = 0;
	int returnGy = 0;
	int returnBy = 0;
	int size = 2 * mRadius + 1;
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++)
		{
			int idx_x = (i + mRadius) * size + j + mRadius;
			int idx_y = (i + mRadius) * size + j + mRadius + 9;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
				clamp<int>(y + i, photo.height() - 1, 0));
			returnRx += color.red() * vector[idx_x];
			returnGx += color.green() * vector[idx_x];
			returnBx += color.blue() * vector[idx_x];
			returnRy += color.red() * vector[idx_y];
			returnGy += color.green() * vector[idx_y];
			returnBy += color.blue() * vector[idx_y];
		}
	return QColor(clamp(sqrt(returnRx * returnRx + returnRy * returnRy), 255, 0),
		          clamp(sqrt(returnGx * returnGx + returnGy * returnGy), 255, 0),
		          clamp(sqrt(returnBx * returnBx + returnBy * returnBy), 255, 0));
};

QImage Sobel_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

void Sharpness_filter::createSharpnessVector()
{
	mRadius = 1;
	vector = new float[9];
	float vec[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
	for (int i = 0; i < 9; i++)
		vector[i] = vec[i];
};

void Sharpen_filter::createSharpenVector()
{
	mRadius = 1;
	vector = new float[9];
	float vec[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
	for (int i = 0; i < 9; i++)
		vector[i] = vec[i];
};

QColor Median_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	int size = 2 * mRadius + 1;
	QVector<int> red(size * size);
	QVector<int>green(size * size);
	QVector<int> blue(size * size);
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++)
		{
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
				clamp<int>(y + i, photo.height() - 1, 0));
			red[idx] = color.red();
			green[idx] = color.green();
			blue[idx] = color.blue();
		}
	qSort(red.begin(), red.end());
	qSort(green.begin(), green.end());
	qSort(blue.begin(), blue.end());
	return QColor(red[4], green[4], blue[4]);
};

QImage Median_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}

QColor Maximum_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	int size = 2 * mRadius + 1;
	QVector<int> red(size * size);
	QVector<int>green(size * size);
	QVector<int> blue(size * size);
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++)
		{
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
				clamp<int>(y + i, photo.height() - 1, 0));
			red[idx] = color.red();
			green[idx] = color.green();
			blue[idx] = color.blue();
		}
	qSort(red.begin(), red.end());
	qSort(green.begin(), green.end());
	qSort(blue.begin(), blue.end());
	return QColor(red[8], green[8], blue[8]);
};

QImage Maximum_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}

QImage Glowing_edges_filter::calculateNewImagePixMap(const QImage& photo)
{
	QImage image(photo);
	Median_filter* median = new Median_filter(mRadius);
	image = median->calculateNewImagePixMap(image);
	Sobel_filter* sobel = new Sobel_filter();
	image = sobel->calculateNewImagePixMap(image);
	Maximum_filter* maximum = new Maximum_filter(mRadius);
	image = maximum->calculateNewImagePixMap(image);
	return image;
}

void Dilation::createDilationVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QColor Dilation::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	QColor result(0, 0, 0);
	int size = 2 * radius + 1;
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++) {
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), 
				                            clamp<int>(y + i, photo.height() - 1, 0));
			int intens_color = 0.36 * color.red() + 0.53 * color.green() + 0.11 * color.blue();
			int intens_result = 0.36 * result.red() + 0.53 * result.green() + 0.11 * result.blue();
			if ((vector[idx] != 0) && (intens_result < intens_color))
				result = color;
		}
	return result;
};

QImage Dilation::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

void Erosion::createErosionVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QColor Erosion::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	QColor result(255, 255, 255);
	int size = 2 * radius + 1;
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++) {
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
				clamp<int>(y + i, photo.height() - 1, 0));
			int intens_color = 0.36 * color.red() + 0.53 * color.green() + 0.11 * color.blue();
			int intens_result = 0.36 * result.red() + 0.53 * result.green() + 0.11 * result.blue();
			if ((vector[idx] != 0) && (intens_result > intens_color))
				result = color;
		}
	return result;
};

QImage Erosion::calculateNewImagePixMap(const QImage& photo)
{
	QImage result_Image(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = calculateNewPixelColor(photo, x, y, mRadius);
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
};

void Opening::createOpeningVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QImage Opening::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	Erosion* erosion = new Erosion(mRadius, vector);
	result = erosion->calculateNewImagePixMap(result);
	Dilation* dilation = new Dilation(mRadius, vector);
	result  = dilation->calculateNewImagePixMap(result);
	return result;
};

void Closing::createClosingVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QImage Closing::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	Dilation* dilation = new Dilation(mRadius, vector);
	result = dilation->calculateNewImagePixMap(result);
	Erosion* erosion = new Erosion(mRadius, vector);
	result = erosion->calculateNewImagePixMap(result);
	return result;
};

void Grad::createGradVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QImage Grad::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	QImage dilationImage(photo);
	QImage erosionImage(photo);
	Dilation* dilation = new Dilation(mRadius, vector);
	dilationImage = dilation->calculateNewImagePixMap(photo);
	Erosion* erosion = new Erosion(mRadius, vector);
	erosionImage = erosion->calculateNewImagePixMap(photo);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor erosion_color = erosionImage.pixelColor(x, y);
			QColor dilation_color = dilationImage.pixelColor(x, y);
			QColor color(clamp<int>(dilation_color.red() - erosion_color.red(), 255, 0),
				         clamp<int>(dilation_color.green() - erosion_color.green(), 255, 0),
				         clamp<int>(dilation_color.blue() - erosion_color.blue(), 255, 0));
			result.setPixelColor(x, y, color);
		}
	return result;
};

void Black_hat::createBlack_hatVector(int radius, float* _vector)
{
	mRadius = radius;
	int size = 2 * mRadius + 1;
	vector = new float[size * size];
	for (int i = 0; i < size * size; i++)
		vector[i] = _vector[i];
};

QImage Black_hat::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	Closing* closing = new Closing(mRadius, vector);
	result = closing->calculateNewImagePixMap(result);
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);
			QColor result_color = result.pixelColor(x, y);
			QColor color(clamp<int>(result_color.red() - photo_color.red(), 255, 0),
				         clamp<int>(result_color.green() - photo_color.green(), 255, 0), 
				         clamp<int>(result_color.blue() - photo_color.blue(), 255, 0));
			result.setPixelColor(x, y, color);
		}
	return result;
};

QImage Autolevels::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	int minR = 255;
	int minG = 255;
	int minB = 255;
	int maxR = 0;
	int maxG = 0;
	int maxB = 0;
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			if (minR > color.red())
				minR = color.red();
			if (minG > color.green())
				minG = color.green();
			if (minB > color.blue())
				minB = color.blue();
			if (maxR < color.red())
				maxR = color.red();
			if (maxG < color.green())
				maxG = color.green();
			if (maxB < color.blue())
				maxB = color.blue();
		}
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			int red = (color.red() - minR) * (255 / (maxR - minR));
			int green = (color.green() - minG) * (255 / (maxG - minG));
			int blue = (color.blue() - minB) * (255 / (maxB - minB));
			QColor result_color(clamp<int>(red, 255, 0), clamp<int>(green, 255, 0), clamp<int>(blue, 255, 0));
			result.setPixelColor(x, y, result_color);
		}
	return result;
};

QImage Perfect_reflector::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	int maxR = 0;
	int maxG = 0;
	int maxB = 0;
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			if (maxR < color.red())
				maxR = color.red();
			if (maxG < color.green())
				maxG = color.green();
			if (maxB < color.blue())
				maxB = color.blue();
		}
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			int red = color.red() * (255 / maxR);
			int green = color.green() * (255 / maxG);
			int blue = color.blue() * (255 / maxB);
			QColor result_color(clamp<int>(red, 255, 0), clamp<int>(green, 255, 0), clamp<int>(blue, 255, 0));
			result.setPixelColor(x, y, result_color);
		}
	return result;
};

QImage Gray_world::calculateNewImagePixMap(const QImage& photo)
{
	QImage result(photo);
	long int R = 0;
	long int G = 0;
	long int B = 0;
	long int n = 0;
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			R += color.red();
			G += color.green();
			B += color.blue();
			n++;
		}
	R = R / n;
	G = G / n;
	B = B / n;
	double avg = (R + G + B) / 3;
	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor color = photo.pixelColor(x, y);
			int red = (int)(color.red() * (avg / R));
			int green = (int)(color.green() * (avg / G));
			int blue = (int)(color.blue() * (avg / B));
			QColor result_color(clamp<int>(red, 255, 0), clamp<int>(green, 255, 0), clamp<int>(blue, 255, 0));
			result.setPixelColor(x, y, result_color);
		}
	return result;
};