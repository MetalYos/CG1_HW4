#pragma once

#include "ALMath.h"

struct Frame
{
	Mat4 ModelTransform;
	Mat4 CamTransform;
	int FrameNumber;
};

class Animation
{
public:
	Animation()
		: maxFrame(-1), currentFrame(NULL) {}
	~Animation() { ClearAnimation(); }

	void AddKeyFrame(const Mat4& modelTransform, const Mat4& camTransform, int frameNumber);
	Frame* GetFrame(int frame, bool linearInterpolation = true);
	int GetLastFrameNumber() const;

	void ResetAnimation();
	bool StepToNextFrame();
	const Frame* GetCurrentFrame() const;

	void ClearAnimation();

private:
	Frame* GetFrameLinearInterpolation(Frame* before, Frame* after, int frame) const;

private:
	std::vector<Frame*> keyFrames;
	int maxFrame;
	Frame* currentFrame;
};

