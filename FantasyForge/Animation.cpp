#include "Animation.h"

Animation::Animation(Image sprite_sheet, int2 sprite_size, int2 sheet_dim, float fps)
	:
	currentFrame(0),
	frameWidth(sprite_size.x),
	frameHeight(sprite_size.y),
	nFrames(sheet_dim.x * sheet_dim.y),
	currentFrameTime(0.0f),
	secsPerFrame(1.0f / fps)
{
	assert(sprite_size.x * sheet_dim.x <= sprite_sheet.GetWidth());
	assert(sprite_size.y * sheet_dim.y <= sprite_sheet.GetHeight());
	frames.resize(nFrames);
	for (int y = 0; y < sheet_dim.y; ++y)
	{
		for (int x = 0; x < sheet_dim.x; ++x)
		{
			frames[y * sheet_dim.x + x] = sprite_sheet.Cropped(sprite_size.x, sprite_size.y, x * sprite_size.x, y * sprite_size.y);
		}
	}
}

const int& Animation::GetFrameWidth() const
{
	return frameWidth;
}

const int& Animation::GetFrameHeight() const
{
	return frameHeight;
}

vec2i Animation::GetFrameSize() const
{
	return { frameWidth,frameHeight };
}

const int& Animation::GetCurrentFrameIndex() const
{
	return currentFrame;
}

void Animation::SetCurrentFrameIndex(int frame)
{
	currentFrame = frame;
}

const float& Animation::GetCurrentFrameTime() const
{
	return currentFrameTime;
}

void Animation::SetCurrentFrameTime(float time)
{
	currentFrameTime = time;
}

float Animation::GetFPS() const
{
	return 1.0f / secsPerFrame;
}

void Animation::SetFPS(float fps)
{
	secsPerFrame = 1.0f / fps;
}

const std::vector<Image>& Animation::GetFrames() const
{
	return frames;
}

std::vector<Image>& Animation::GetFrames()
{
	return frames;
}

const Image& Animation::GetCurrentFrame() const
{
	return frames[currentFrame];
}

const Image& Animation::Play(float time_ellapsed)
{
	currentFrameTime += time_ellapsed;
	while (currentFrameTime >= secsPerFrame)
	{
		currentFrameTime -= secsPerFrame;
		++currentFrame;
		if (currentFrame >= nFrames)
		{
			currentFrame = 0;
		}
	}
	return frames[currentFrame];
}

bool Animation::PlayAndCheck(float time_ellapsed)
{
	currentFrameTime += time_ellapsed;
	bool frameChange = false;
	while (currentFrameTime >= secsPerFrame)
	{
		currentFrameTime -= secsPerFrame;
		++currentFrame;
		if (currentFrame >= nFrames)
		{
			currentFrame = 0;
		}
		frameChange = true;
	}
	return frameChange;
}

void Animation::Draw(Graphics& gfx, int x, int y, int layer) const
{
	frames[currentFrame].Draw(gfx, x, y, layer);
}

void Animation::DrawWithTransparency(Graphics& gfx, int x, int y, int layer) const
{
	frames[currentFrame].DrawWithTransparency(gfx, x, y, layer);
}
