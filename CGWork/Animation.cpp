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

	for (unsigned int i = 0; i < keyFrames.size(); i++)
	{
		if (keyFrames[i]->FrameNumber == frame)
		{
			frameToReturn = new Frame();
			frameToReturn->ModelTransform = keyFrames[i]->ModelTransform;
			frameToReturn->CamTransform = keyFrames[i]->CamTransform;
			frameToReturn->FrameNumber = frame;
			return frameToReturn;
		}
		else if ((keyFrames[i]->FrameNumber < frame) &&
			(keyFrames[i + 1]->FrameNumber > frame))
		{
			if (linearInterpolation)
				frameToReturn = GetFrameLinearInterpolation(keyFrames[i], keyFrames[i + 1], frame);
			else
			{
				// Bezier
			}

			return frameToReturn;
		}
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

bool Animation::StepToNextFrame()
{
	if ((keyFrames.size() == 0) || (currentFrame->FrameNumber == maxFrame))
		return false;

	int currentFrameNum = currentFrame->FrameNumber;
	delete currentFrame;
	currentFrame = GetFrame(++currentFrameNum);

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
