#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
const double PI = 3.141592653589793;

double  AlphaStart=90.0,
		AlphaStep=2.0,
		Rs=59.5,
		Ra=25.0,
		Zstart=0.25,
		Zstep=0.5,
		Zend=30.0,
		Rstart=4.0,Rend=8.0,
		Lstart=2.0,Lend=10.0,
		Px=0.0,Py=0.0,
		Beta=24.0,
		ThetaStep=2.0,
		Mu=0.19,
		Epsilon=0.000001;

double  Alpha, AlphaEnd,AlphaR, ThetaR,
		Sx, Sy, 
		Theta,
		z, Lz, Rz,
		nx,ny,
		dx,dy,
		OM,Mx,My,MB,
		Ax,Ay,Bx,By,
		C1x,C2x,
		C1T,C2T,
		Tx,Ty,
		Cx,Cy,
		Dx,Dy,
		L,
		Attenuation;


bool isInside(double Tx, double Ty) {
	double d;
	if (Tx < C1x) {
		d = sqrt((Tx - C1x)*(Tx - C1x) + (Ty - Py)*(Ty - Py));
		}
	else {
		if (Tx > C2x) {
			d = sqrt((Tx - C2x)*(Tx - C2x) + (Ty - Py)*(Ty - Py));
			}
		else {
			d = abs(Ty-Py);
			}
		}
	return (d <= Rz);
	}

int main(int argc, char* argv[]) {
	
	ofstream pFile("output.csv");
	
	if (pFile.is_open()){
		pFile << "z, Alpha ";
		Theta = -Beta;
		while (Theta <= Beta) {
			pFile << ", " << Theta;
			Theta += ThetaStep;
		}
		pFile << endl;
		
		z = Zstart;
		while (z <= Zend) {
			cout << "." ;

			Lz = Lstart + ((Lend - Lstart) / Zend)*z;
			Rz = Rstart + ((Rend - Rstart) / Zend)*z;

			C1x = Px - Lz;
			C2x = Px + Lz;

			Alpha = AlphaStart;
			AlphaEnd = AlphaStart + 360.0;
			while (Alpha < AlphaEnd) {
				pFile << z << ", " << Alpha;

				AlphaR = Alpha* PI / 180.0;
				Sx = Rs*cos(AlphaR);
				Sy = Rs*sin(AlphaR);
				
				Theta = -Beta;
				while (Theta<= Beta) {
					ThetaR = (Alpha+Theta)*PI/180.0;
					dx = -cos(ThetaR);
					dy = -sin(ThetaR);
					
					nx = -dy;
					ny = dx;

					Ax = Sx;
					Ay = Sy;

					Bx = Sx + 2.0*Rs*dx;
					By = Sy + 2.0*Rs*dy;
					
					C1T = (Sx - C1x)*nx + (Sy - Py)*ny;
					C2T = (Sx - C2x)*nx + (Sy - Py)*ny;

					if (0 <= C1T*C2T) {
						if (abs(C1T) <= abs(C2T)) {
							Tx = C1x + C1T*nx;
							Ty = Py + C1T*ny;
							}
						else {
							Tx = C2x + C2T*nx;
							Ty = Py + C2T*ny;
							}
						}
					else {
						Tx = C1x + (abs(C1T)/(abs(C1T)+abs(C2T)))*2.0*Lz;
						Ty = Py;
						}
					if (isInside(Tx,Ty)) {
						// Bisection T , A
						do {
							Cx = (Tx + Ax) / 2.0;
							Cy = (Ty + Ay) / 2.0;
							if (isInside(Cx, Cy)) {
								Tx = Cx;
								Ty = Cy;
								}
							else {
								Ax = Cx;
								Ay = Cy;
								}
							} while (sqrt((Tx - Ax)*(Tx - Ax) + (Ty - Ay)*(Ty - Ay)) > Epsilon);
						// Bisection T, B
						do {
							Dx = (Tx + Bx) / 2.0;
							Dy = (Ty + By) / 2.0;
							if (isInside(Dx, Dy)) {
								Tx = Dx;
								Ty = Dy;
								}
							else {
								Bx = Dx;
								By = Dy;
								}
							} while (sqrt((Tx - Bx)*(Tx - Bx) + (Ty - By)*(Ty - By)) > Epsilon);
						L = sqrt((Cx - Dx)*(Cx - Dx) + (Cy - Dy)*(Cy - Dy));
						Attenuation = exp(-Mu*L);
						}
					else {
						Attenuation = 0.0;
						}
					pFile << ", " << L;
					Theta += ThetaStep;
					}
				pFile << endl;
				Alpha += AlphaStep;
				}
			z += Zstep;
			}
		}
	else cout << "Unable to open file 'output.csv'."<<endl;
	pFile.close();
	return 0;
}