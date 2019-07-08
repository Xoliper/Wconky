/*
 * Font.hpp
 *
 *  Created on: 21 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef ENGINE_COMMON_FONT_HPP_
#define ENGINE_COMMON_FONT_HPP_


#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

class Glyph {

	friend class Font;
	friend class Text;

public:
	Glyph(){
		adv_x = adv_y = bitmap_w = bitmap_h = bitmap_l = bitmap_t = texture_offset = 0.0f;
	};
	~Glyph(){};

private:
	  float adv_x; // advance.x
	  float adv_y; // advance.y
	  float bitmap_w; // bitmap.width;
	  float bitmap_h; // bitmap.rows;
	  float bitmap_l; // bitmap_left;
	  float bitmap_t; // bitmap_top;
	  float texture_offset; // x offset of glyph in texture coordinates

};



class Font {

	friend class Text;

public:
	Font(std::string path, int size);
	virtual ~Font();
	GLuint tex;
	unsigned int atlas_width = 0;
	unsigned int atlas_height = 0;

	//Font data
	std::string path;
	int size;

private:
	//Freetype data
	static FT_Library ft;
	FT_Face face;
	FT_GlyphSlot g;



	//Glyph data
	std::vector<Glyph> glyphs_info;

	//OpenGL data

};

#endif /* ENGINE_COMMON_FONT_HPP_ */
