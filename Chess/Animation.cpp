#include "Animation.h"

#include <glm/gtc/matrix_transform.hpp>


Animation::Animation(float start, float stop, Piece *target, std::function<void()> callback) :
	_start(start), _stop(stop), _target(target), _callback(callback)
{}


PositionAnimation::PositionAnimation(float start, float stop, Piece *target, const glm::vec3 &from, const glm::vec3 &to, std::function<void()> callback) :
	Animation(start, stop, target, callback), _from(from), _delta(to - from)
{}


void PositionAnimation::apply(float time, bool force)
{
	if (time <= _start) return;
	else if (time >= _stop) {
		if (force)
			_target->setPosition(getPosition(1.0f));
	} else _target->setPosition(getPosition((time - _start) / (_stop - _start)));
}


glm::vec3 StraightAnimation::getPosition(float time) const
{
	return _delta * time + _from;
}


CurveAnimation::CurveAnimation(float start, float stop, Piece *target, const glm::vec3 &from, const glm::vec3 &to, float maxDeltaY, std::function<void()> callback) :
	PositionAnimation(start, stop, target, from, to, callback), _maxDeltaY(maxDeltaY)
{}


glm::vec3 CurveAnimation::getPosition(float time) const
{
	float tmp = std::abs(0.5f - time),
		coeff = 1.0f - (tmp < 0.25 ? tmp * 4.0f : 1.0f);
	glm::vec3 position = _delta * time + _from;
	position.y += _maxDeltaY * coeff;
	return position;
}