#pragma once

#include "ALMath.h"

struct Frame
{
	Mat4 ModelTransform;
	Mat4 CamTransform;
	int FrameNumber;

	Vec4 Translation;
	Vec4 Scale;
	Vec4 Rotation;

	bool Action[3];
	bool ObjectSpace;
	bool AroundEye;

	Frame()
		: FrameNumber(0), ObjectSpace(false), AroundEye(true)
	{
		Action[0] = Action[1] = Action[2] = true;
	}
};

class Animation
{
public:
	Animation()
		: maxFrame(-1), currentFrame(NULL) {}
	~Animation() { ClearAnimation(); }

	void AddKeyFrame(Frame* keyFrame);
	Frame* GetFrame(int frame, bool linearInterpolation = true);
	int GetLastFrameNumber() const;

	void ResetAnimation();
	bool StepToNextFrame(bool linearInterpolation = true);
	const Frame* GetCurrentFrame() const;

	void ClearAnimation();

private:
	Frame* GetFrameLinearInterpolation(Frame* before, Frame* after, int frame) const;
	Frame* GetFrameLinearInterpolation2(Frame* before, Frame* after, int frame) const;
	Frame* GetFrameBezierInterpolation(int frameNum) const;

private:
	std::vector<Frame*> keyFrames;
	int maxFrame;
	Frame* currentFrame;
};

