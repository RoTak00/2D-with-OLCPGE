#define OLC_PGE_APPLICATION
//#define OLC_IGNORE_VEC2D
#include "olcPixelGameEngine.h"

#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include "functions.h"



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
	vec2d vY = {0.0f, 1.0f};

	vec2d vCamera = {0.0f, 0.0f};
	mat3x3 matScaleToScreen, matTranslateCamera, matTranslate, matTranslateBack, matZoom, matTransform;
	mat3x3 matRotateToCoordinates;
	object square;
	
public:
	bool OnUserCreate() override
	{
		square.loadFromFile("file.txt");
		

		fDim = (float)std::min(ScreenWidth(), ScreenHeight());
		matScaleToScreen = Mat_MakeScale(fDim / 2);
		
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);

		if(GetKey(olc::A).bHeld) vCamera.x -= 1.0f * fElapsedTime / fScale;
		if(GetKey(olc::D).bHeld) vCamera.x += 1.0f * fElapsedTime / fScale;
		if(GetKey(olc::W).bHeld) vCamera.y += 1.0f * fElapsedTime / fScale;
		if(GetKey(olc::S).bHeld) vCamera.y -= 1.0f * fElapsedTime / fScale;

		if(GetKey(olc::UP).bHeld) fScale *= expf(fElapsedTime);
		if(GetKey(olc::DOWN).bHeld) fScale /= expf(fElapsedTime);

		//if(GetKey(olc::E).bHeld) fRotation += 1.0f * fElapsedTime;
		//if(GetKey(olc::Q).bHeld) fRotation -= 1.0f * fElapsedTime;

		matRotateToCoordinates = Mat_MakeRotation(fRotation + 3.1415 / 2);
		vX = Vec_MultiplyMatrix(vX, matRotateToCoordinates);
		vY = Vec_MultiplyMatrix(vY, matRotateToCoordinates);
		
		matTranslateCamera = Mat_MakeTranslation(-vCamera.x, vCamera.y);
		matZoom = Mat_MakeScale(fScale);
		matTranslateBack= Mat_MakeTranslation(1.0f, 1.0f);
		matTransform = Mat_MultiplyMatrix(matZoom, matTranslateCamera);
		matTransform = Mat_MultiplyMatrix(matTranslateBack, matTransform);

		for(auto& l: square.lines)
		{
			line lineTransformed, lineScaled, lineRotated;
			
			lineRotated.p[0] = Vec_MultiplyMatrix(l.p[0], matRotateToCoordinates);
			lineRotated.p[1] = Vec_MultiplyMatrix(l.p[1], matRotateToCoordinates);

			lineTransformed.p[0] = Vec_MultiplyMatrix(lineRotated.p[0], matTransform);
			lineTransformed.p[1] = Vec_MultiplyMatrix(lineRotated.p[1], matTransform);
			
			lineScaled.p[0] = Vec_MultiplyMatrix(lineTransformed.p[0], matScaleToScreen);
			lineScaled.p[1] = Vec_MultiplyMatrix(lineTransformed.p[1], matScaleToScreen);

			DrawLine(lineScaled.p[0].x, lineScaled.p[0].y, lineScaled.p[1].x, lineScaled.p[1].y);
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
