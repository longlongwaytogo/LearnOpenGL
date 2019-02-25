// Chapter 1: Output an image

#include <iostream>
using namespace std;

int main()
{
	int nx = 200;
	int ny = 200;
	std::cout << "P3\n" << nx << " " << ny << "\n255" << std::endl;

	for(int j = ny -1; j >= 0; j--)
	{
		for(int i = 0; i < nx; i++)
		{
			float r = float(i)/nx;
			float g = float(j)/ny;
			float b = 0.2;

			int ir = int(255.9 * r);
			int ig = int(255.9 * g);
			int ib = int(255.9 * b);

			std::cout << ir <<" " << ig << " " << ib << " " << std::endl;

		}
	}
	return 0;
}