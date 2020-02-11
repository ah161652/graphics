#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <glm/glm.hpp>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;

#define WIDTH 480
#define HEIGHT 395

void draw();
void update();
void handleEvent(SDL_Event event);
void drawLine(CanvasPoint p1, CanvasPoint p2, uint32_t colour);
void drawStrokedTriangle(CanvasTriangle triangle);
void drawFilledTriangle(CanvasTriangle triangle);
void readPPM();

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

int main(int argc, char* argv[])
{
  SDL_Event event;
  readPPM();
  while(true)
  {
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);

    // update();
    // draw();
    //drawLine(p1, p2, colour);
    //drawTriangle(t1,colour);
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
  window.clearPixels();
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      float red = rand() % 255;
      float green = 0.0;
      float blue = 0.0;
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}

void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}

void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) cout << "LEFT" << endl;
    else if(event.key.keysym.sym == SDLK_RIGHT) cout << "RIGHT" << endl;
    else if(event.key.keysym.sym == SDLK_UP) cout << "UP" << endl;
    else if(event.key.keysym.sym == SDLK_DOWN) cout << "DOWN" << endl;
    else if(event.key.keysym.sym == SDLK_u){
      CanvasPoint p1(rand() % WIDTH,rand() % HEIGHT);
      CanvasPoint p2(rand() % WIDTH,rand() % HEIGHT);
      CanvasPoint p3(rand() % WIDTH,rand() % HEIGHT);

      float red = rand() % 255;
      float green = rand() % 255;
      float blue = rand() % 255;

      CanvasTriangle trandom(p1,p2,p3,Colour(red,green,blue));
      drawStrokedTriangle(trandom);
    }
    else if(event.key.keysym.sym == SDLK_f){
      CanvasPoint p1(rand() % WIDTH,rand() % HEIGHT);
      CanvasPoint p2(rand() % WIDTH,rand() % HEIGHT);
      CanvasPoint p3(rand() % WIDTH,rand() % HEIGHT);

      float red = rand() % 255;
      float green = rand() % 255;
      float blue = rand() % 255;

      CanvasTriangle trandom(p1,p2,p3,Colour(red,green,blue));
      drawFilledTriangle(trandom);
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}

void drawLine(CanvasPoint p1, CanvasPoint p2, uint32_t colour){
  float xDiff = p2.x -p1.x;
  float yDiff = p2.y - p1.y;

  float numberOfSteps = std::max(abs(xDiff),abs(yDiff));

  float xStepSize = xDiff/numberOfSteps;
  float yStepSize = yDiff/numberOfSteps;

  for (float i=0.0; i<numberOfSteps; i++){
    float x = p1.x + (xStepSize*i);
    float y = p1.y + (yStepSize*i);
    window.setPixelColour(round(x), round(y), colour);
  }

}

void drawStrokedTriangle(CanvasTriangle triangle){
  uint32_t colour = (255<<24) + (int(triangle.colour.red)<<16) + (int(triangle.colour.green)<<8) + int(triangle.colour.blue);
  drawLine(triangle.vertices[0], triangle.vertices[1],colour);
  drawLine(triangle.vertices[1], triangle.vertices[2],colour);
  drawLine(triangle.vertices[0], triangle.vertices[2],colour);
}

void drawFilledTriangle(CanvasTriangle triangle){
  uint32_t colour = (255<<24) + (int(triangle.colour.red)<<16) + (int(triangle.colour.green)<<8) + int(triangle.colour.blue);
  drawLine(triangle.vertices[0], triangle.vertices[1],colour);
  drawLine(triangle.vertices[1], triangle.vertices[2],colour);
  drawLine(triangle.vertices[0], triangle.vertices[2],colour);

  CanvasPoint ptop = triangle.vertices[0];
  CanvasPoint pmid = triangle.vertices[1];
  CanvasPoint pbottom = triangle.vertices[2];
  CanvasPoint pTemp;

  if (pbottom.y < pmid.y){
    swap(pbottom.y, pmid.y);
    swap(pbottom.x, pmid.x);

    }

  if (pmid.y < ptop.y){
    swap(pmid.y, ptop.y);
    swap(pmid.x, ptop.x);
  }

  if (pbottom.y < pmid.y){
    swap(pbottom.y, pmid.y);
    swap(pbottom.x, pmid.x);

    }
  CanvasPoint psplit(round(ptop.x + ((pmid.y - ptop.y)/(pbottom.y-ptop.y))*(pbottom.x - ptop.x)), pmid.y);

  float slope1 = (pmid.x - ptop.x)/(pmid.y - ptop.y);
  float slope2 = (psplit.x - ptop.x)/(psplit.y - ptop.y);

  float x1 = ptop.x;
  float x2 = ptop.x;
  for (int i = ptop.y; i <= pmid.y; i++) {
    drawLine(CanvasPoint(round(x1),i),CanvasPoint(round(x2),i), colour);
    x1 = x1 + slope1;
    x2 = x2 + slope2;
  }

  slope1 = (pbottom.x - pmid.x)/(pbottom.y - pmid.y);
  slope2 = (pbottom.x - psplit.x)/(pbottom.y - psplit.y);

  x1 = pbottom.x;
  x2 = pbottom.x;
  for (int i = pbottom.y; i > pmid.y; i--) {
    drawLine(CanvasPoint(round(x1),i),CanvasPoint(round(x2),i), colour);
    x1 = x1 - slope1;
    x2 = x2 - slope2;
  }
}

void readPPM(){
  char fileName[50] = "texture.ppm";
	string pSix;
	size_t width = 0;
	size_t height = 0;
	size_t maximum = 255;
   string data;

  ifstream infile;
  infile.open(fileName);

  getline(infile,pSix);
  getline(infile,data);
//  infile.ignore(2560, '\n');
  string widthAndHeight;
  getline(infile,widthAndHeight);
  width = stoi(widthAndHeight.substr(0,widthAndHeight.find(" ")));
  height = stoi(widthAndHeight.substr(widthAndHeight.find(" ")));
  getline(infile,data);
  std::cout << pSix  << ", "<< width  << ", "<< height  << ", "<< maximum << '\n';
  size_t size = width * height * 3;
  // char pixelsRGB[size];
  std::vector<char> pixelsRGB(size);
  char red, green, blue;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      red = infile.get();
      green = infile.get();
      blue = infile.get();
      uint32_t colour = (255<<24) + (red<<16) + (green<<8) + blue;
      window.setPixelColour( x, y, colour);
    }
  }


  // close file
  infile.close();

}
