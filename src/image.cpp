#include "image.h"
#include <iostream>

Image::Image(int width, int height){
	this->width		= width;
	this->height	= height;
	this->_max		= 1;
	pixels = (Colour*)malloc(sizeof(Colour) * height * width);

}

Image::~Image(){
	free(pixels);
}

void Image::setColour(int x, int y, Colour colour){

	if (x < 0 || x > width - 1 ||
		y < 0 || y > height - 1){
		printf("ERROR: Image::setColour(%d,%d) out of range!", x, y);
		exit(EXIT_FAILURE);
	}

	pixels[x * height + y] = colour;


}

int Image::getHeight(){
	return this->height;
}

int Image::getWidth(){
	return this->width;
}

void Image::writeTGA(const char *outfile, bool scale_color)
{
	FILE *fp = fopen(outfile, "w");
	if (fp == NULL)
	{
		perror("ERROR: Image::WriteTga() failed to open file for writing!\n");
		exit(EXIT_FAILURE);
	}

	// write 24-bit uncompressed targa header
	putc(0, fp);
	putc(0, fp);

	putc(2, fp); // type is uncompressed RGB

	putc(0, fp);
	putc(0, fp);
	putc(0, fp);
	putc(0, fp);
	putc(0, fp);

	putc(0, fp); // x origin, low byte
	putc(0, fp); // x origin, high byte

	putc(0, fp); // y origin, low byte
	putc(0, fp); // y origin, high byte

	putc(width & 0xff, fp); // width, low byte
	putc((width & 0xff00) >> 8, fp); // width, high byte

	putc(height & 0xff, fp); // height, low byte
	putc((height & 0xff00) >> 8, fp); // height, high byte

	putc(24, fp); // 24-bit color depth

	putc(0, fp);

	// write the raw pixel data in groups of 3 bytes (BGR order)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// if color scaling is on, scale 0.0 -> _max as a 0 -> 255 unsigned byte
			unsigned char rbyte, gbyte, bbyte;
			Colour *color = pixels + (x * height + y);
			if (scale_color)
			{
				rbyte = (unsigned char)((color->r / _max) * 255);
				gbyte = (unsigned char)((color->g / _max) * 255);
				bbyte = (unsigned char)((color->b / _max) * 255);
			}
			else
			{
				if(color->r > 0){
					std::cout << "";
				}
				double r = (color->r > 1.0) ? 1.0 : color->r;
				double g = (color->g > 1.0) ? 1.0 : color->g;
				double b = (color->b > 1.0) ? 1.0 : color->b;
				rbyte = (unsigned char)(r * 255);
				gbyte = (unsigned char)(g * 255);
				bbyte = (unsigned char)(b * 255);
			}
			putc(bbyte, fp);
			putc(gbyte, fp);
			putc(rbyte, fp);
		}
	}

	fclose(fp);
}