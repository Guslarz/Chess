#pragma once

#include <functional>

#include "Piece.h"


class Animation
{
protected:
	float _start, _stop;
	Piece *_target;
	std::function<void()> _callback;

public:
	Animation(float, float, Piece*, std::function<void()> = nullptr);
	virtual ~Animation() { if (_callback) _callback(); }

	virtual void apply(float) = 0;
	virtual void finish() = 0;
	virtual bool finished(float time) const { return time >= _stop; }
	virtual Piece* target() const { return _target; }
};


class PositionAnimation :
	public Animation
{
protected:
	glm::vec3 _from, _delta;

public:
	PositionAnimation(float, float, Piece*, const glm::vec3&, const glm::vec3&, std::function<void()> = nullptr);

	virtual void apply(float) override;
	virtual void finish() override;

protected:
	virtual glm::vec3 getPosition(float) const = 0;
};


class StraightAnimation :
	public PositionAnimation
{
public:
	using PositionAnimation::PositionAnimation;

private:
	virtual glm::vec3 getPosition(float) const override;
};


class CurveAnimation :
	public PositionAnimation
{
	float _maxDeltaY;

public:
	CurveAnimation(float, float, Piece*, const glm::vec3&, const glm::vec3&, float, std::function<void()> = nullptr);

private:
	virtual glm::vec3 getPosition(float) const override;
};


class OpacityAnimation :
	public Animation
{
public:
	OpacityAnimation(float, float, Piece*, std::function<void()> = nullptr);

	virtual void apply(float) override;
	virtual void finish() override;

protected:
	virtual float getOpacity(float) const = 0;
};


class FadeAnimation :
	public OpacityAnimation
{
	using OpacityAnimation::OpacityAnimation;

private:
	float getOpacity(float) const;
};


class ShowAnimation :
	public OpacityAnimation
{
	using OpacityAnimation::OpacityAnimation;

private:
	float getOpacity(float) const;
};