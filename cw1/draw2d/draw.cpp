#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

#include <iostream>

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	
	// Get width and height of window
	int mWidth = aSurface.get_width();
	int mHeight = aSurface.get_height();
	float startX = 0;
	float endX = 0;
	float startY = 0;
	float endY = 0;

	// Check if all values are positive
	if(aBegin.x <= 0.0 || aBegin.y <= 0.0 || aEnd.x <= 0.0 || aEnd.y <= 0.0)
		return;

	// Check all points fall into the grid
	if((aBegin.x >= mWidth || aBegin.y >= mHeight) && (aEnd.x >= mWidth || aEnd.y >= mHeight))
		return;
	
	// Resize the points that fall outside the range
	if(aBegin.x >= mWidth){
		aBegin.x = mWidth -1;
	}
	if(aBegin.y >= mHeight){
		aBegin.y = mHeight -1;
	}
	if(aEnd.x >= mWidth){
		aEnd.x = mWidth -1;
	}
	if(aEnd.y >= mHeight){
		aEnd.y = mHeight -1;
	}

	// Find min and max of x coordinates
	if(aBegin.x < aEnd.x){
		startX = aBegin.x;
		endX = aEnd.x;
	}
	else if (aBegin.x > aEnd.x){
		startX = aEnd.x;
		endX = aBegin.x;
	}

	// Find min and max of y coordinates
	if(aBegin.y < aEnd.y){
		startY = aBegin.y;
		endY = aEnd.y;
	}
	else if (aBegin.y > aEnd.y){
		startY = aEnd.y;
		endY = aBegin.y;
	}

	// Ref: https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm

	if(endY == startY){
		for (int x = startX; x <= endX; ++x){
			aSurface.set_pixel_srgb(x,aEnd.y, aColor);
		}
		return;
	}
	// if x coordinates are the same? draw vertical line |
	if(startX == endX){
		for (int y = startY; y <= endY; ++y){
			aSurface.set_pixel_srgb(aEnd.x,y, aColor);
		}
		return;
	}

	float dx = (float) (endX - startX);
	float dy = (float) (endY - startY);
	float step = 1;	
	if(dx > dy)
		step = dx;
	else
		step = dy;
	
	float Xincrement = dx / (float) step;
	float Yincrement = dy / (float) step;

	for(int v=0; v < step; v++)
	{
		int flagx = 0, flagy = 0 ;
		if(aBegin.x - aEnd.x < 0){
			startX = startX + Xincrement;
			flagx = 0;
		}
		else{
			endX = endX - Xincrement;
			flagx = 1;
		}

		if(aBegin.y - aEnd.y < 0){
			flagy = 0;
			startY = startY + Yincrement;
		}
		else{
			flagy = 1;
			endY = endY - Yincrement;
		}
		aSurface.set_pixel_srgb(round(flagx == 0? startX : endX),round(flagy == 0? startY : endY), aColor);
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	int minX = std::min(aP0.x, std::min(aP1.x, aP2.x));
    int maxX = std::max(aP0.x, std::max(aP1.x, aP2.x));
    int minY = std::min(aP0.y, std::min(aP1.y, aP2.y));
    int maxY = std::max(aP0.y, std::max(aP1.y, aP2.y));

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vec2f P = {x, y};
			double detT = det(aP0, aP1, aP2);
			double detPBC = det(P, aP1, aP2);
			double detAPC = det(aP0, P, aP2);
			double detABP = det(aP0, aP1, P);

			if (detT == 0) {
				return;
			}

			double alpha = detPBC / detT;
			double beta = detAPC / detT;
			double gamma = detABP / detT;

    		if (alpha >= 0 && beta >= 0 && gamma >= 0){
				aSurface.set_pixel_srgb(x, y , aColor);
			}
        }
    }

	draw_line_solid(aSurface , aP0 , aP1, aColor);
	draw_line_solid(aSurface , aP1 , aP2, aColor);
	draw_line_solid(aSurface , aP2 , aP0, aColor);

	/* 	
	to find all integer coordinates within a triangle in C++ with a time complexity of O(n),
	you can use a simple grid traversal approach. The number of integer coordinates within a triangle 
	is typically proportional to the area of the triangle.So, you can iterate through the grid and 
	check if each point is inside the triangle or not. Here's a C++ code to find all integer coordinates within
	a triangle with a time complexity of O(n), where n is the number of integer coordinates within the triangle:
	n this code, we first calculate the bounding box of the triangle (minX, maxX, minY, maxY), and then iterate through all the integer coordinates within that bounding box. For each point, we check whether it's inside the triangle using the barycentric coordinates method. Coordinates that are inside the triangle are added to the coordinates vector.
	The time complexity of this algorithm is O(n), where n is the number of integer coordinates within the triangle.
	*/
	
}
double det(Vec2f A, Vec2f B, Vec2f C) {
    return A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: Remove the following when you start your implementation!
	//TODO: This draws a solid triangle until you implement the function
	//TODO: properly. (This is such that you can see the asteroids with solid
	//TODO: shading until that point.)
	draw_triangle_solid( aSurface, aP0, aP1, aP2, linear_to_srgb( aC0 ) );
	(void)aC1;
	(void)aC2;
}


void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
