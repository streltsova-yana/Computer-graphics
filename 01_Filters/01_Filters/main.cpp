#include <QtCore/QCoreApplication>
#include <iostream>
#include <string>
#include <QImage>
#include "Filters.h"

using namespace std;

int main(int argc, char *argv[])
{
	string s;
	QImage photo;
	for (int i = 0; i < argc; i++)
		if (!strcmp(argv[i], "-p") && (i + 1 < argc))
			s = argv[i + 1];
	photo.load(QString(s.c_str() ) );
	photo.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Starting.png");

	//Point filters
	Invert_filter* invert = new Invert_filter();
	QImage invertImage = invert->calculateNewImagePixMap(photo);
	invertImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Invert.png");

	Gray_scale_filter* gray = new Gray_scale_filter();
	QImage grayImage = gray->calculateNewImagePixMap(photo);
	grayImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Gray_scale.png");

	Sepia_filter* sepia = new Sepia_filter();
	QImage sepiaImage = sepia->calculateNewImagePixMap(photo);
	sepiaImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Sepia.png");

	Bright_filter* bright = new Bright_filter();
	QImage brightImage = bright->calculateNewImagePixMap(photo);
	brightImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Bright.png");

	Shift_filter* shift = new Shift_filter();
	QImage shiftImage = shift->calculateNewImagePixMap(photo);
	shiftImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Shift.png");

	Rotate_filter* rotate = new Rotate_filter();
	QImage rotateImage = rotate->calculateNewImagePixMap(photo);
	rotateImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Rotate.png");

	Horizontal_waves_filter* horizon_waves = new Horizontal_waves_filter();
	QImage horizon_waves_image = horizon_waves->calculateNewImagePixMap(photo);
	horizon_waves_image.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Horizontal_waves.png");

	Vertical_waves_filter* vertical_waves = new Vertical_waves_filter();
	QImage vertical_waves_image = vertical_waves->calculateNewImagePixMap(photo);
	vertical_waves_image.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Vertical_waves.png");

	//Matrix Filters
	Blur_filter* blur = new Blur_filter(2);
	QImage blurImage = blur->calculateNewImagePixMap(photo);
	blurImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Blur.png");

	Gaussian_blur_filter* gauss_blur = new Gaussian_blur_filter();
	QImage gaussBlurImage = gauss_blur->calculateNewImagePixMap(photo);
	gaussBlurImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Gaussian_blur.png");

	Sobel_filter* sobel = new Sobel_filter();
	QImage sobelImage = sobel->calculateNewImagePixMap(photo);
	sobelImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Sobel.png");

	Sharpness_filter* sharp = new Sharpness_filter();
	QImage sharpImage = sharp->calculateNewImagePixMap(photo);
	sharpImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Sharpness.png");

	Sharpen_filter* sharpen = new Sharpen_filter();
	QImage sharpenImage = sharpen->calculateNewImagePixMap(photo);
	sharpenImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Sharpen.png");

	Median_filter* median = new Median_filter();
	QImage medianImage = median->calculateNewImagePixMap(photo);
	medianImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Median.png");

	Maximum_filter* maximum = new Maximum_filter();
	QImage maximumImage = maximum->calculateNewImagePixMap(photo);
	maximumImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Maximum.png");

	Glowing_edges_filter* glowing = new Glowing_edges_filter();
	QImage glowingImage = glowing->calculateNewImagePixMap(photo);
	glowingImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Glowing_edges.png");

	float* vec = new float[9];
	for (int i = 0; i < 9; i++)
		vec[i] = 1;
	Dilation* dilation = new Dilation(1, vec);
	QImage dilationImage = dilation->calculateNewImagePixMap(photo);
	dilationImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Dilation.png");

	Erosion* erosion = new Erosion(1, vec);
	QImage erosionImage = erosion->calculateNewImagePixMap(photo);
	erosionImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Erosion.png");

	Opening* opening = new Opening(1, vec);
	QImage openingImage = opening->calculateNewImagePixMap(photo);
	openingImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Opening.png");

	Closing* closing = new Closing(1, vec);
	QImage closingImage = closing->calculateNewImagePixMap(photo);
	closingImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Closing.png");

	Grad* grad = new Grad(1, vec);
	QImage gradImage = grad->calculateNewImagePixMap(photo);
	gradImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Grad.png");

	Black_hat* black_hat = new Black_hat(1, vec);
	QImage black_hatImage = black_hat->calculateNewImagePixMap(photo);
	black_hatImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Black_hat.png");

	Autolevels* autolevels = new Autolevels();
	QImage autolevelsImage = autolevels->calculateNewImagePixMap(photo);
	autolevelsImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Autolevels.png");

	Perfect_reflector* reflector = new Perfect_reflector();
	QImage reflectorImage = reflector->calculateNewImagePixMap(photo);
	reflectorImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Perfect_reflector.png");

	Gray_world* gray_world = new Gray_world();
	QImage gray_worldImage = gray_world->calculateNewImagePixMap(photo);
	gray_worldImage.save("C:/Users/yanas/Desktop/My_project/CG/01_Filters/01_Filters/Resources/Gray_world.png");
	return 0;
}
