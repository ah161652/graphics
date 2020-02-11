#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

using namespace std;
using namespace glm;

#define WIDTH 320
#define HEIGHT 240

void draw(vector<vec3> pixel_vector);
void update();
void handleEvent(SDL_Event event);
vector<float> interpolate(float from, float to, int numOfValues);
vector<vec3> interpolate3d(vec3 from, vec3 to, int numOfValues);

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

int main(int argc, char* argv[])
{
  SDL_Event event;

  //Getting pixel values fvrom interpolation
  //vector<float> pixel_vector = interpolate(255,0,window.width);
  vec3 from(255,0,0);
  vec3 to(255,255,0);
  vector<vec3> leftcolumn = interpolate3d(from,to,window.height);
  vec3 from2(0,0,255);
  vec3 to2(0,255,0);
  vector<vec3> rightcolumn = interpolate3d(from2,to2,window.height);
  vector<vec3> pixel_vector;
  for(int i=0; i<window.height ;i++) {
      vector<vec3> current_row = interpolate3d(leftcolumn[i],rightcolumn[i],window.width);
      for(int j=0; j<window.width ;j++) {
        pixel_vector.push_back(current_row[j]);
      }

  }

  while(true)
  {
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);
    update();
    draw(pixel_vector);
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw(vector<vec3> pixel_vector)
{
  window.clearPixels();

  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {

      float red = pixel_vector[x+y*window.width].x;
      float green = pixel_vector[x+y*window.width].y;
      float blue = pixel_vector[x+y*window.width].z;
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
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}

vector<float> interpolate(float from, float to, int numOfValues){
  float difference = to - from;
  float stepSize = difference/(numOfValues-1);

  vector<float> outVector;
  for (size_t i = 0; i < numOfValues; i++) {
    outVector.push_back(from + i*stepSize);

  }
  return outVector;
}

vector<vec3> interpolate3d(vec3 from, vec3 to, int numOfValues){
  float difference1 = to.x - from.x;
  float difference2 = to.y - from.y;
  float difference3 = to.z - from.z;

  vec3 stepSize(difference1/(numOfValues-1),difference2/(numOfValues-1),difference3/(numOfValues-1));

  vector<vec3> outVector;
  for (size_t i = 0; i < numOfValues; i++) {
    vec3 current(from.x + i*stepSize.x,from.y + i*stepSize.y,from.z + i*stepSize.z);
    outVector.push_back(current);
  }
  return outVector;
}
