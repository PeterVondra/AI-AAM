#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

extern "C"
{

struct HEADER
{
	char idlength;
	char colormaptype;
	char datatypecode;
	short int colormaporigin;
	short int colormaplength;
	char colormapdepth;
	short int x_origin;
	short int y_origin;
	short width;
	short height;
	char bitsperpixel;
	char imagedescriptor;
};
struct PIXEL
{
	unsigned char R, G, B, A;
};

class TGA_Loader
{
	public:
		TGA_Loader();
		~TGA_Loader();

		static unsigned char* loadTGAfile(const char* path);

	private:
		static void mergeBytes(PIXEL* pixel, unsigned char* p, int bytes);
};


inline void TGA_Loader::mergeBytes(PIXEL* pixel, unsigned char* p, int bytes)
{
	if(bytes == 4)
	{
		pixel->R = p[2];
		pixel->G = p[1];
		pixel->B = p[0];
		pixel->A = p[3];
	}
	else if(bytes == 3)
	{
		pixel->R = p[2];
		pixel->G = p[1];
		pixel->B = p[0];
		pixel->A = 0;
	}
	else if(bytes = 2)
	{
		pixel->R = (p[1] & 0x7c) << 1;
		pixel->G = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
		pixel->B = (p[0] & 0x1f)  << 3;
		pixel->A = (p[1] & 0x80);
	}
}

inline unsigned char* TGA_Loader::loadTGAfile(const char* path)
{
	int n = 0, i, j;

	int bytes2read, skipover = 0;
	unsigned char p[5];
	FILE* fptr;

	//open file
	if((fptr = fopen(path, "r")) == NULL)
		std::cout << "failed to open" << path << std::endl;

	HEADER hedr;
	hedr.idlength = fgetc(fptr);
	hedr.colormaptype = fgetc(fptr);
	hedr.datatypecode = fgetc(fptr);
	fread(&hedr.colormaporigin, 2, 1, fptr);
	fread(&hedr.colormaplength, 2, 1, fptr);
	hedr.colormapdepth = fgetc(fptr);
	fread(&hedr.x_origin, 2, 1, fptr);
	fread(&hedr.y_origin, 2, 1, fptr);
	fread(&hedr.width, 2, 1, fptr);
	fread(&hedr.height, 2, 1, fptr);
	hedr.bitsperpixel = fgetc(fptr);
	hedr.imagedescriptor = fgetc(fptr);

	PIXEL* pixels;

	for(i = 0; i < hedr.width * hedr.height; i++)
	{
		pixels[i].R = 0;
		pixels[i].G = 0;
		pixels[i].B = 0;
		pixels[i].A = 0;	
	}

	if(hedr.datatypecode != 2 && hedr.datatypecode != 10)
		std::cerr << "can only read image type 2 or 10"  << std::endl;

	if(hedr.bitsperpixel != 16 && hedr.bitsperpixel != 24 && hedr.bitsperpixel != 32)
		std::cerr << "can only read pixel depth { 16, 24, 32 }" << std::endl;

	if(hedr.colormaptype != 0 && hedr.colormaptype != 1)
		std::cerr << "can only read color map types { 0, 10 }" << std::endl;

	skipover += hedr.idlength;
	skipover += hedr.colormaptype * hedr.colormaplength;
	fseek(fptr, skipover, SEEK_CUR);

	//read image
	bytes2read = hedr.bitsperpixel / 8;
	while(n < hedr.width * hedr.height)
	{
		if(hedr.datatypecode == 2)
		{
			if(fread(p, 1, bytes2read, fptr) != bytes2read)
				std::cerr << "unexpected end at pixel: " << n << "," << i << std::endl;
		}
		j = p[0] & 0x7f;
		mergeBytes(&(pixels[n]), &(p[1]), bytes2read);
		n++;
		if(p[0] & 0x80) //RLE chunk
		{
			for(i = 0; i < j; i++)
			{
				mergeBytes(&(pixels[n]), &(p[1]), bytes2read);
				n++;
			}
		}
		else
		{
			for(i = 0; i < j; i++)
			{
				if(fread(p, 1, bytes2read, fptr) != bytes2read)
					std::cerr << "unexpected end of file at pixel: " << n << ", " << i << std::endl;
				mergeBytes(&(pixels[n]), p, bytes2read);
				n++;
			}
		}
	}
	fclose(fptr);

	unsigned char* imageResult;
	for(int t = 0; t < hedr.width * hedr.height * sizeof(PIXEL); t += 4)
	{
		imageResult[t] = pixels[t].R;
		imageResult[t + 1] = pixels[t].G;
		imageResult[t + 2] = pixels[t].B;
		imageResult[t + 3] = pixels[t].A;
	}
	return imageResult;
}
}
