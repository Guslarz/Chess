#pragma once

#include "Piece.h"


class Animation
{
protected:
	float _start, _stop;
	Piece *_target;

public:
	Animation(float, float, Piece*);
	virtual ~Animation() {}

	virtual void render(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&, float) const;
	virtual bool finished(float time) const { return _stop >= time; }
	virtual Piece* target() const { return _target; }

private:
	virtual glm::vec3 getPosition(float) const = 0;
};


class StraightAnimation :
	public Animation
{
	float _x0, _dx,
		_y0, _dy,
		_z0, _dz;

public:
	StraightAnimation(float, float, Piece*, float, float, float, float, float, float);

private:
	virtual glm::vec3 getPosition(float) const override;
};


class CurveAnimation :
	public Animation
{
	float _x0, _dx,
		_y0, _dy,
		_z0, _dz;

public:
	CurveAnimation(float, float, Piece*, float, float, float, float, float, float);

private:
	virtual glm::vec3 getPosition(float) const override;
};


//class FadeAnimation :
//	public Animation
//{
//
//};

