/**
 * NES_DEV is a cross-platform, portable, and hand-held NES emulator.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "config.h"
#include "tft-ili9325.h"

#define flash const

#define	SCREEN_WIDTH		240
#define	SCREEN_HEIGHT		320

#define	BLACK	0x0000
#define	BLUE	0x001F
#define	GREEN	0x07E0
#define	CYAN	0x07FF
#define	RED	0xF800
#define	MAGENTA	0xF81F
#define	PURPLE	0xF81F
#define	YELLOW	0xFFE0
#define	WHITE	0xFFFF

#define RGB(r, g, b)	(((r << 10) & RED)	\
						| ((g << 5) & GREEN)\
						| (b & BLUE))

#define SWAP(a, b, tmp) do {	\
	tmp = a;										\
	a = b;											\
	b = tmp;										\
} while(0)

typedef unsigned int Color;

typedef struct Point
{
	int x;
	int y;
} Point, * pPoint;

typedef struct Line
{
	Point start;
	Point end;
} Line, *pLine;

typedef struct Rectangle
{
	Point p;
	int width;
	int height;
} Rectangle, * pRectangle;

#if defined TFT_ILI9325_AVR_LIB
#pragma used+
#endif

/**
 * Add two point.
 *
 * @param a The left point to add.
 * @param b The right point to add.
 * @return The result of addition of a and b.
 */
Point add_point(pPoint a, pPoint b);

/**
 * Set the pixel by the given point.
 *
 * @param p The point to set color.
 * @param c The color of pixel to set.
 */
void set_pixel(Point p, Color c);

/**
 * Drow a vertical line.
 *
 * @param p1 Starting point of the line.
 * @param p2 Ending point ot the line.
 * @param c Color to draw.
 */
void draw_vertical_line(Point p1, Point p2, Color c);

/**
 * Draw a horizontal line.
 *
 * @param p1 Starting point of the line.
 * @param p2 End point of the line.
 * @parma c Color to draw.
 */
void draw_horizontal_line(Point p1, Point p2, Color c);

/**
 * Draw a line.
 *
 * @param p1 Starting point of the line.
 * @param p2 Ending point of the line.
 */
void draw_line(Point p1, Point p2, Color c);

/**
 * Draw a rectangle.
 *
 * @param rect The rectangle you want to draw.
 * @param c Color to draw.
 */
void draw_rectangle(Rectangle rect, Color c);

void fill_rectangle(Rectangle rect, Color c);

void plot_8_circle_point(pPoint center, Point p, Color c);

/**
 * Draw a circle.
 *
 * @param center The center of circle.
 * @param radius The radius of circle.
 * @param c Color to draw.
 */
void draw_circle(Point center, unsigned int radius, Color c);

void fill_circle(Point center, unsigned int radius, Color c);

/**
 * Fill screen by given color.
 *
 * @param c The color of screen.
 */
void clear_screen(Color c);

void draw_char(char ch, const pPoint p, const pPoint s, Color c);

void draw_string(char* str, Point p, Point scale, Color c);

void draw_stringf(flash char* str, Point p, Point scale, Color c);

#if defined TFT_ILI9325_AVR_LIB
#pragma used-
#endif

#endif

