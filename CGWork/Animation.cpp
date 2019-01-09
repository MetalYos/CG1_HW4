#include "Animation.h"
#include <assert.h>

void Animation::AddKeyFrame(const Mat4& modelTransform, const Mat4& camTransform, int frameNumber)
{
	assert(frameNumber > maxFrame);
	
	Frame* frame = new Frame();
	frame->ModelTransform = modelTransform;
	frame->CamTransform = camTransform;
	frame->FrameNumber = frameNumber;

	keyFrames.push_back(frame);
	maxFrame = frameNumber;

	if (frameNumber == 0)
		currentFrame = GetFrame(0);
}

Frame* Animation::GetFrame(int frame, bool linearInterpolation)
{
	if ((frame < 0) || (frame > maxFrame) || (keyFrames.size() == 0))
		return NULL;

	Frame* frameToReturn = NULL;
	if (linearInterpolation)
	{
		for (unsigned int i = 0; i < keyFrames.size(); i++)
		{
			if (keyFrames[i]->FrameNumber == frame)
			{
				frameToReturn = new Frame();
				frameToReturn->ModelTransform = keyFrames[i]->ModelTransform;
				frameToReturn->CamTransform = keyFrames[i]->CamTransform;
				frameToReturn->FrameNumber = frame;
				break;
			}
			else if ((keyFrames[i]->FrameNumber < frame) &&
				(keyFrames[i + 1]->FrameNumber > frame))
			{
				frameToReturn = GetFrameLinearInterpolation(keyFrames[i], keyFrames[i + 1], frame);
				break;
			}
		}
	}
	else
	{
		frameToReturn = GetFrameBezierInterpolation(frame);
	}

	return frameToReturn;
}

int Animation::GetLastFrameNumber() const
{
	return maxFrame;
}

void Animation::ResetAnimation()
{
	if (keyFrames.size() == 0)
		return;

	currentFrame = GetFrame(0);
}

bool Animation::StepToNextFrame(bool linearInterpolation)
{
	if ((keyFrames.size() == 0) || (currentFrame->FrameNumber == maxFrame))
		return false;

	int currentFrameNum = currentFrame->FrameNumber;
	delete currentFrame;
	currentFrame = GetFrame(++currentFrameNum, linearInterpolation);

	return true;
}

const Frame* Animation::GetCurrentFrame() const
{
	return currentFrame;
}

void Animation::ClearAnimation()
{
	while (keyFrames.size() > 0)
	{
		Frame* frame = keyFrames.back();
		keyFrames.pop_back();
		delete frame;
	}
	currentFrame = NULL;
}

Frame* Animation::GetFrameLinearInterpolation(Frame* before, Frame* after, int frame) const
{
	Frame* result = new Frame();
	double t = (double)(frame - before->FrameNumber) / (after->FrameNumber - before->FrameNumber);

	result->ModelTransform = before->ModelTransform * (1.0 - t) + after->ModelTransform * t;
	result->CamTransform = before->CamTransform * (1.0 - t) + after->CamTransform * t;
	result->FrameNumber = frame;

	return result;
}

Frame* Animation::GetFrameBezierInterpolation(int frameNum) const
{
	double t = frameNum / (double)maxFrame;
	Mat4 sumModel(0.0);
	Mat4 sumCam(0.0);
	int n = keyFrames.size() - 1;

	for (unsigned int i = 0; i < keyFrames.size(); i++)
	{
		double temp1 = Factorial(n);
		double temp2 = pow(1.0 - t, n - i);
		double temp3 = pow(t, i);
		double temp4 = Factorial(i) * Factorial(n - i);

		double fact = (temp1 * temp2 * temp3) / temp4;

		sumModel = sumModel + keyFrames[i]->ModelTransform * fact;
		sumCam = sumCam + keyFrames[i]->CamTransform * fact;
	}
	sumModel[3][3] = 1.0;
	sumCam[3][3] = 1.0;

	Frame* frame = new Frame();
	frame->ModelTransform = sumModel;
	frame->CamTransform = sumCam;
	frame->FrameNumber = frameNum;

	return frame;
}
