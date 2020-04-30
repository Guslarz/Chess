#include "Animation.h"

#include <glm/gtc/matrix_transform.hpp>


Animation::Animation(float start, float stop, Piece *target) :
	_start(start), _stop(stop), _target(target)
{}


void Animation::render(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M, float time) const
{
	glm::vec3 position;
	if (time <= _start)
		position = getPosition(0.0f);
	else if (time >= _stop)
		position = getPosition(1.0f);
	else position = getPosition((time - _start) / (_stop - _start));
	_target->renderForceM(shaderProgram, P,  V,  glm::translate(M, position));
}


StraightAnimation::StraightAnimation(float start, float stop, Piece *target, 
	float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo) :
	Animation(start, stop, target), _x0(xFrom), _dx(xTo - xFrom), _y0(yFrom),
	_dy(yTo - yFrom), _z0(zFrom), _dz(zTo - zFrom)
{}


glm::vec3 StraightAnimation::getPosition(float time) const
{
	return glm::vec3(_x0 + _dx * time, _y0 + _dy * time, _z0 + _dz * time);
}


CurveAnimation::CurveAnimation(float start, float stop, Piece *target,
	float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo) :
	Animation(start, stop, target), _x0(xFrom), _dx(xTo - xFrom), _y0(yFrom),
	_dy(yTo - yFrom), _z0(zFrom), _dz(zTo - zFrom)
{}


glm::vec3 CurveAnimation::getPosition(float time) const
{
	float tmp = std::abs(0.5f - time),
		coeff = 1.0f - (tmp < 0.25 ? tmp * 4.0f : 1.0f);
	return glm::vec3(_x0 + _dx * time, _y0 + _dy * coeff, _z0 + _dz * time);
}