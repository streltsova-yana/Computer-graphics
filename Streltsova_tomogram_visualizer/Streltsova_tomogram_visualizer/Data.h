#pragma once
#include <iostream>
#include <locale>
#include <fstream>
#include <stdio.h>
#include "qdebug.h"

class Data
{
private:
	int width, height, depth;
	float scaleX, scaleY, scaleZ;
	short* arr;
	short min, max;
public:
	Data() : width(0), height(0), depth(0), scaleX(0), scaleY(0), scaleZ(0), arr(nullptr), min(0), max(0) {};
	~Data() { delete[] arr; };
	short getMin() const { return min; };
	short getMax() const { return max; };
	int getWidth() const { return width; };
	int getHeight() const { return height; };
	int getDepth() const { return depth; };

	int readFile(QString fileName)
	{
		setlocale(0, "RUS");
		std::fstream file;
		file.open(fileName.toStdString(), std::ios::binary | std::ios::in);

		if (!file.is_open())
			qDebug() << "File can't be open\n";
		qDebug() << QString("Open");

		int x, y, z;
		file.read((char*)&x, sizeof(int));
		file.read((char*)&y, sizeof(int));
		file.read((char*)&z, sizeof(int));

		if ((x == 0) || (y == 0) || (z == 0))
		{
			qDebug() << QString("Wrong data size");
			file.close();
			return 2;
		}
		qDebug() << "width = " << x << ", height = " << y << ", depth = " << z;

		file.read((char*)&scaleX, sizeof(float));
		file.read((char*)&scaleY, sizeof(float));
		file.read((char*)&scaleZ, sizeof(float));
		qDebug() << "scaleX = " << scaleX << ", scaleY = " << scaleY << ", scaleZ = " << scaleZ;

		if (arr != nullptr)
			delete[] arr;

		width = x;
		height = y;
		depth = z;
		long sizeArr = width * height * depth;

		arr = new short[sizeArr];
		file.read((char*)arr, sizeArr * sizeof(short));
		file.close();
		qDebug() << QString("close");

		MinMax();

		return 0;
	};
	void Data::MinMax()
	{
		min = max = arr[0];
		for (int i = 0; i < width * height * depth; i++)
		{
			if (min > arr[i])
				min = arr[i];
			if (max < arr[i])
				max = arr[i];
		}
		qDebug() << "Min Max" << min << max;
	};
	short& Data::operator[](const int index)
	{
		assert((index >= 0) && (index < width * height * depth));
		return arr[index];
	};
};