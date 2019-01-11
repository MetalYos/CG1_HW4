#include "Animation.h"
#include <assert.h>
#include "Scene.h"

void Animation::AddKeyFrame(Frame* keyFrame)
{
	if (keyFrame == NULL)
		return;

	assert(keyFrame->FrameNumber > maxFrame);

	keyFrames.push_back(keyFrame);
	if (keyFrame->FrameNumber > 0)
	{
		Frame* prevKF = keyFrames[keyFrames.size() - 2];
		keyFrame->OriginalFrame = prevKF->OriginalFrame + (keyFrame->FrameNumber - prevKF->FrameNumber);
		keyFrame->FrameNumber = (int)(keyFrame->OriginalFrame * speedFactor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrame->FrameNumber;

	if (keyFrame->FrameNumber == 0)
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

void Animation::IncreasePlaybackSpeed(double percentage)
{
	double factor = 1.0 - percentage / 100.0;
	for (unsigned int i = 1; i < keyFrames.size(); i++)
	{
		keyFrames[i]->FrameNumber = (int)(keyFrames[i]->FrameNumber * factor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor *= factor;
}

void Animation::DecreasePlaybackSpeed(double percentage)
{
	double factor = 1.0 + percentage / 100.0;
	for (unsigned int i = 1; i < keyFrames.size(); i++)
	{
		keyFrames[i]->FrameNumber = (int)(keyFrames[i]->FrameNumber * factor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor *= factor;
}

void Animation::NormalPlaybackSpeed()
{
	for (Frame* keyFrame : keyFrames)
	{
		keyFrame->FrameNumber = keyFrame->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor = 1.0;
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
	maxFrame = -1;
	speedFactor = 1.0;
}

Frame* Animation::GetFrameLinearInterpolation2(Frame* before, Frame* after, int frame) const
{
	Frame* result = new Frame();
	double t = (double)(frame - before->FrameNumber) / (after->FrameNumber - before->FrameNumber);

	result->ModelTransform = before->ModelTransform * (1.0 - t) + after->ModelTransform * t;
	result->CamTransform = before->CamTransform * (1.0 - t) + after->CamTransform * t;
	result->FrameNumber = frame;

	return result;
}

Frame* Animation::GetFrameLinearInterpolation(Frame * before, Frame * after, int frame) const
{
	Frame* result = new Frame();
	double t = (double)(frame - before->FrameNumber) / (after->FrameNumber - before->FrameNumber);

	// Set initial matricies
	Mat4 modelTransform = before->ModelTransform;
	Mat4 camTransform = before->CamTransform;

	// Calculate delta interpolation
	Vec4 transform = Vec4(0.0, 0.0, 0.0, 1.0) * (1.0 - t) + after->Translation * t;
	Vec4 scale = Vec4(1.0, 1.0, 1.0, 1.0) * (1.0 - t) + after->Scale * t;
	Vec4 rotation = Vec4(0.0, 0.0, 0.0, 1.0) * (1.0 - t) + after->Rotation * t;

	// Multiply according to coordinates space and action
	if (after->ObjectSpace)
	{
		if (after->Action[0])
			modelTransform = Mat4::Translate(transform) * modelTransform;
		else if (after->Action[1])
			modelTransform = Mat4::Scale(scale) * modelTransform;
		else
			modelTransform = Mat4::RotateX(rotation[0]) * Mat4::RotateY(rotation[1])
			* Mat4::RotateZ(rotation[2]) * modelTransform;
	}
	else
	{
		if (after->Action[0])
			camTransform = camTransform * Mat4::Translate(transform);
		else if (after->Action[1])
		{
			Mat4 S = Mat4::Scale(scale);
			if (after->AroundEye)
			{
				CameraParameters camParams = Scene::GetInstance().GetCamera()->GetCameraParameters();
				camTransform = camTransform * Mat4::Translate(camParams.EyeCam) * S *
					Mat4::Translate(-camParams.EyeCam);
			}
			else
				camTransform = camTransform * S;
		}
		else
		{
			Mat4 R = Mat4::RotateX(rotation[0])
				* Mat4::RotateY(rotation[1]) * Mat4::RotateZ(rotation[2]);
			if (after->AroundEye)
			{
				CameraParameters camParams = Scene::GetInstance().GetCamera()->GetCameraParameters();
				camTransform = camTransform * Mat4::Translate(camParams.EyeCam) * R *
					Mat4::Translate(-camParams.EyeCam);
			}
			else
				camTransform = camTransform * R;
		}
	}

	// Create the frame to return
	result->ModelTransform = modelTransform;
	result->CamTransform = camTransform;
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
