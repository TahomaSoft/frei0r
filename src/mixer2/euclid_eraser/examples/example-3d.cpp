#include<iostream>

#include<math.h>

using namespace std;

// x is reference image
// y is comparison image to remove reference from
// 
// 
float euclidDistance(int x_r, int y_r, int x_g,  int y_g, int x_b, int y_b)
{
  //calculating color channel differences for next steps
  float red_d = x_r - y_r;
  float green_d = x_g - y_g;
  float blue_d = x_b - y_b; 
  
  float sq_sum, dist;

  //calculating Euclidean distance
  sq_sum = pow(red_d, 2) + pow(green_d, 2) + pow (blue_d, 2);
  dist = sqrt(sq_sum);                  
  
  return dist;
}

int main()
{
  int x_r, y_r, x_g, y_g, x_b, y_b;
  float dist;
  cout << "Enter x_r:" << endl;    //user inputs the points
  cin >> x_r;
  cout << "Enter y_r:" << endl;
  cin >> y_r;
  cout << "Enter x_g:" << endl;
  cin >> x_g;
  cout << "Enter y_g:" << endl;
  cin >> y_g;
  cout << "Enter x_b:" << endl;
  cin >> x_b;
  cout << "Enter y_b:" << endl;
  cin >> y_b;


  
  dist = euclidDistance(x_r, y_r, x_g, y_g, x_b, y_b);    //initiate equation
  cout << "Euclidan Distance Between X and Y = " << dist;
}
