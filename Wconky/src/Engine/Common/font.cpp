/*
 * Font.cpp
 *
 *  Created on: 21 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#include "font.hpp"


bool SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile )
{
	BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER info;
		memset ( &bmfh, 0, sizeof (BITMAPFILEHEADER ) );
		memset ( &info, 0, sizeof (BITMAPINFOHEADER ) );
		bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
			bmfh.bfReserved1 = 0;
			bmfh.bfReserved2 = 0;
			bmfh.bfSize = sizeof(BITMAPFILEHEADER) +
				sizeof(BITMAPINFOHEADER) + paddedsize;
			bmfh.bfOffBits = 0x36;
			info.biSize = sizeof(BITMAPINFOHEADER);
				info.biWidth = width;
				info.biHeight = height;
				info.biPlanes = 1;
				info.biBitCount = 8;
				info.biCompression = BI_RGB;
				info.biSizeImage = 0;
				info.biXPelsPerMeter = 0x0ec4;
				info.biYPelsPerMeter = 0x0ec4;
				info.biClrUsed = 0;
				info.biClrImportant = 0;
				HANDLE file = CreateFile ( bmpfile , GENERIC_WRITE, FILE_SHARE_READ,
						 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
					if ( NULL == file )
					{
						CloseHandle ( file );
						return false;
					}
					unsigned long bwritten;
						if ( WriteFile ( file, &bmfh, sizeof ( BITMAPFILEHEADER ),
							&bwritten, NULL ) == false )
						{
							CloseHandle ( file );
							return false;
						}

						if ( WriteFile ( file, &info, sizeof ( BITMAPINFOHEADER ),
							&bwritten, NULL ) == false )
						{
							CloseHandle ( file );
							return false;
						}
						if ( WriteFile ( file, Buffer, paddedsize, &bwritten, NULL ) == false )
							{
								CloseHandle ( file );
								return false;
							}
						CloseHandle ( file );
							return true;
						}

FT_Library Font::ft;



Font::Font(std::string path, int size) {
	setlocale( LC_ALL, "" );

	this->path = path;
	this->size = size;

	if(FT_Init_FreeType(&ft)) {
		std::cout<<"Could not init freetype library\n"<<std::endl;
	}

	if(FT_New_Face(ft, path.c_str(), 0, &face)) {
		//Try to change path
		if(FT_New_Face(ft, ("C:\\Windows\\Fonts\\"+path).c_str(), 0, &face)){
			std::cout<<"Could not open font\n"<<std::endl;
		}
	}

	FT_Select_Charmap(face , ft_encoding_unicode);
	FT_Set_Pixel_Sizes(face, 0, size);

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
		    continue;
		 }
		 g = face->glyph;
		 atlas_width += g->bitmap.width+2;
		 atlas_height = std::max(atlas_height, g->bitmap.rows);
	}


	//Create atlas texture
	//glActiveTexture(GL_TEXTURE0);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);





	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


	//"Load texture"
	int x = 0;
	glyphs_info.reserve(128);
	for(int i = 32; i < 128; i++) {
		//if(FT_Load_Char(face, i, FT_LOAD_RENDER))
		//	continue;

		unsigned long c = FT_Get_Char_Index(face, i);
		FT_Load_Glyph(face,c,FT_LOAD_RENDER);
		FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
		g = face->glyph;

		Glyph g_temp;
		g_temp.adv_x = g->advance.x >> 6;
		g_temp.adv_y = g->advance.y >> 6;
		g_temp.bitmap_w = g->bitmap.width;
		g_temp.bitmap_h = g->bitmap.rows;
		g_temp.bitmap_l = g->bitmap_left;
		g_temp.bitmap_t = g->bitmap_top;
		g_temp.texture_offset = (float)x / atlas_width;
		glyphs_info[i] = g_temp;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		x += g->bitmap.width+2;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex = textureID;

}


/*
Font::Font(std::string path, int size) {
	setlocale( LC_ALL, "" );

	if(FT_Init_FreeType(&ft)) {
		std::cout<<"Could not init freetype library\n"<<std::endl;
	}

	if(FT_New_Face(ft, path.c_str(), 0, &face)) {
		std::cout<<"Could not open font\n"<<std::endl;
	}

	FT_Select_Charmap(face , ft_encoding_unicode);
	FT_Set_Pixel_Sizes(face, 0, size);

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
		    continue;
		 }
		 g = face->glyph;
		 atlas_width += g->bitmap.width;
		 atlas_height = std::max(atlas_height, g->bitmap.rows);
	}


	//Create atlas texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


	unsigned long c = FT_Get_Char_Index(face, 'M');
	FT_Load_Glyph(face,c,FT_LOAD_RENDER);
	FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	g = face->glyph;
	int xc = g->bitmap.width;
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);


	c = FT_Get_Char_Index(face, 'T');
	FT_Load_Glyph(face,c,FT_LOAD_RENDER);
	FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	g = face->glyph;
	glTexSubImage2D(GL_TEXTURE_2D, 0, xc, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //nearest
	atlas_width = g->bitmap.width+xc;
	atlas_height = g->bitmap.rows;
	tex = textureID;

}*/



Font::~Font() {
	glDeleteTextures(1, &tex);
	std::cout<<"Font removed"<<std::endl;
}

