#define OLC_PGE_APPLICATION
//#define OLC_IGNORE_VEC2D
#include "olcPixelGameEngine.h"

#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include "functions.h"
#include "particles.h"



class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}


private:
	float fDim;
	float fScale = 1.0f;
	float fRotation = 0.0f;

	vec2d vX = {1.0f, 0.0f};
	vec2d vY = {0.0f, -1.0f};

	vec2d vCamera = {0.0f, 0.0f};
	object square;

	mat3x3 matScaleToScreen, matTranslateCamera, matTranslateCameraBack, matZoomScreen, matRotateScreen;
	mat3x3 matScreenTransform, matFlipY;


	Ball b;
	
public:
	bool OnUserCreate() override
	{
		square.loadFromFile("file.txt");
		b.mesh = &square;

		

		fDim = (float)std::min(ScreenWidth(), ScreenHeight());
		matScaleToScreen = Mat_MakeScale(fDim / 2);
		matScreenTransform = Mat_MakeIdentity(); 
		

		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);
		
		vX = {1.0f, 0.0f};
		vY = {0.0f, -1.0f};

		
		
		//if(GetKey(olc::E).bHeld) fRotation += 1.0f * fElapsedTime;
		//if(GetKey(olc::Q).bHeld) fRotation -= 1.0f * fElapsedTime;

		
		//vX = Vec_MultiplyMatrix(vX, matRotateScreen);
		//vY = Vec_MultiplyMatrix(vY, matRotateScreen);

		vec2d vdX, vdY;
		vdX = Vec_Mul(vX, 1.0f * fElapsedTime / fScale);
		vdY = Vec_Mul(vY, 1.0f * fElapsedTime / fScale);


		if(GetKey(olc::A).bHeld) vCamera = Vec_Sub(vCamera, vdX);
		if(GetKey(olc::D).bHeld) vCamera = Vec_Add(vCamera, vdX);
		if(GetKey(olc::W).bHeld) vCamera = Vec_Add(vCamera, vdY);
		if(GetKey(olc::S).bHeld) vCamera = Vec_Sub(vCamera, vdY);
		if(GetKey(olc::UP).bHeld) fScale *= expf(fElapsedTime);
		if(GetKey(olc::DOWN).bHeld) fScale /= expf(fElapsedTime);

		b.dv = Vec_Mul(b.vel, 1.0f * fElapsedTime);

		b.pos = Vec_Add(b.pos, b.dv);



		// OUTPUT
		matTranslateCamera = Mat_MakeTranslation(-vCamera.x, -vCamera.y);
		matZoomScreen = Mat_MakeScale(fScale);
		matRotateScreen = Mat_MakeRotation(0);
		
		matTranslateCameraBack = Mat_MakeTranslation(1.0f, 1.0f);

		
		for(auto& l: b.mesh->lines)
		{
			line lineRealPosition, lineTransformed;

			lineRealPosition.p[0] = Vec_Add(l.p[0], b.pos);
			lineRealPosition.p[1] = Vec_Add(l.p[1], b.pos);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineRealPosition.p[0], matTranslateCamera);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineRealPosition.p[1], matTranslateCamera);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineTransformed.p[0], matZoomScreen);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineTransformed.p[1], matZoomScreen);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineTransformed.p[0], matRotateScreen);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineTransformed.p[1], matRotateScreen);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineTransformed.p[0], matTranslateCameraBack);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineTransformed.p[1], matTranslateCameraBack);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineTransformed.p[0], matScaleToScreen);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineTransformed.p[1], matScaleToScreen);
			
			DrawLine(lineTransformed.p[0].x, lineTransformed.p[0].y, lineTransformed.p[1].x, lineTransformed.p[1].y);
		}
		return true;
	}
};


int main(int argc, char* argv[])
{
	Example demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();

	return 0;
}
