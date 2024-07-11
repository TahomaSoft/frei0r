#include<iostream>

#include<math.h>

using namespace std;

// x is reference image
// y is comparison image to remove reference from
// 
// 
float euclidDistance(int x_r, int x_g, int x_b,  int y_r, int y_g, int y_b)
{
  //calculating color channel differences for next steps
  float red_d = x_r - y_r;
  float green_d = x_g - y_g;
  float blue_d = x_b - y_b; 
  
  float sq_sum, dist;

  //calculating Euclidean distance
  sq_sum = pow(red_d, 2) + pow(green_d, 2) + pow (blue_d)
  dist = sqrt(sq_sum);                  
  
  return dist;
}

int main()
{
  int x_r, x_g, x_b, y_r, y_g, y_b;
  float dist;
  cout << "Enter x1:" << endl;    //user inputs the points
  cin >> x1;
  cout << "Enter y1:" << endl;
  cin >> y1;
  cout << "Enter x2:" << endl;
  cin >> x2;
  cout << "Enter y2:" << endl;
  cin >> y2;
  
  dist = distanceCalculate(x1, y1, x2, y2, x3,y3);    //initiate equation
  cout << "Distance Between (" << x1 << " , " << y1 << ") and (" << x2 << " , " << y2 << ") = " << dist;
}
