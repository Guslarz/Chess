#include "Animation.h"

#include <glm/gtc/matrix_transform.hpp>

constexpr float
LIFT_DURATION = 0.25f;


Animation::Animation(float start, float stop, Piece *target, std::function<void()> callback) :
	_start(start), _stop(stop), _target(target), _callback(callback)
{}


PositionAnimation::PositionAnimation(float start, float stop, Piece *target, const glm::vec3 &from, const glm::vec3 &to, std::function<void()> callback) :
	Animation(start, stop, target, callback), _from(from), _delta(to - from)
{}


void PositionAnimation::apply(float time)
{
	if (time <= _start) return;
	else if (time <= _stop)
		_target->setPosition(getPosition((time - _start) / (_stop - _start)));
}


void PositionAnimation::finish()
{
	_target->setPosition(getPosition(1.0f));
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
	glm::vec3 position(_from);
	if (time <= LIFT_DURATION) {
		position.y += _maxDeltaY * (time / LIFT_DURATION);
	}
	else if (time >= 1.0f - LIFT_DURATION) {
		position += _delta;
		position.y += _maxDeltaY * (1.0f - (time + LIFT_DURATION - 1.0f) / LIFT_DURATION);
	}
	else {
		position += _delta * (time - LIFT_DURATION) / (1.0f - 2.0f * LIFT_DURATION);
		position.y += _maxDeltaY;
	}
	return position;
}