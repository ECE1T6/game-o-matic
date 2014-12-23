

#include <iostream>
#include <unistd.h>
#include "Map.h"
#include "Font.h"
#include "Drawing.h"
#include "Bmp.h"

using namespace std;

int main(int argc, char* argv[]){


	cout << "WELCOME!" << endl;
	Map* map = Map_Initialize(70, 20);
	Font* font = Font_Initialize("default.bcm");
	float x[] = {0.0f, 1.0f, -1.0f, 0.0f};
	float y[] = {1.0f, 0.0f, 0.0f, -1.0f};
	//Drawing* drawing = Drawing_Initialize(x, y, 4, D_TRIANGLE_STRIP, 1.0, 0.42, false);
	//Map_AddDrawing(map, drawing, 35, 10, 6.0, true);
	
	Font_Resize(font, 2);
	Map_WriteLn(map, font, "\"xyz\"", 1, 5, 1.0, 0.0, false, true);

	Map_Print(map);

	Font_Terminate(&font);
	Map_Terminate(&map);

	
	return 0;
}
//"

