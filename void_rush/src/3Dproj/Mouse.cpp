#include "Mouse.h"
#include "settings.h"

Mouse::Mouse()
{
	mouseSense = settingsSingleTon::GetInst().getSettings().mouseSence * 0.5f;
	mouse_active = false;
	once = false;
	x = y = 0;
	leaftIsDown = rightIsDown = midIsDown = false;
}

Mouse::~Mouse()
{
}


bool Mouse::IsLeftDown()
{
	return this->leaftIsDown;
}

bool Mouse::isMiddleDown()
{
	return this->midIsDown;
}

bool Mouse::isRightDown()
{
	return this->rightIsDown;
}

bool Mouse::isLeftPressed()
{
	return leaftDown;
}

bool Mouse::isMiddlePressed()
{
	return midDown;
}

bool Mouse::isRightPressed()
{
	return rightDown;
}

MousePoint Mouse::getPos()
{
	return MousePoint({x,y});
}

int Mouse::getPosX()
{
	return x;
}

int Mouse::getPosY()
{
	return y;
}

bool Mouse::EventBufferEmpty()
{
	return mouseBuffer.empty();
}

mouseEvent Mouse::ReadEvent()
{
	if (EventBufferEmpty()) {
		return mouseEvent();
	}
	mouseEvent e = mouseBuffer.front();
	mouseBuffer.pop();
	return e;
}

void Mouse::changeSense(float newSence)
{
	this->mouseSense = newSence * 0.5f;
}


float Mouse::getSense()
{
	return mouseSense;
}

bool Mouse::getMouseActive()
{
	return mouse_active;
}

void Mouse::onLeftPressed(int x, int y)
{
	leaftDown = true;
	this->leaftIsDown = true;
	mouseEvent m(mouseEvent::EventType::LPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onLeftReleased(int x, int y)
{
	this->leaftIsDown = false;
	mouseEvent m(mouseEvent::EventType::LRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onRightPressed(int x, int y)
{
	rightDown = true;
	this->rightIsDown = true;
	mouseEvent m(mouseEvent::EventType::RPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onRightReleased(int x, int y)
{
	this->rightIsDown = false;
	mouseEvent m(mouseEvent::EventType::RRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMiddlePressed(int x, int y)
{
	midDown = true;
	this->midIsDown = true;
	mouseEvent m(mouseEvent::EventType::MPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMiddleReleased(int x, int y)
{
	this->midIsDown = false;
	mouseEvent m(mouseEvent::EventType::MRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onWheelUp(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::WheelUp, x, y);
	mouseBuffer.push(m);
}

void Mouse::onWheelDown(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::WheelDown, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMouseMove(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::Move, x, y);
	this->x = x;
	this->y = y;
	mouseBuffer.push(m);
}

void Mouse::activateMouse(bool activate)
{
	mouse_active = activate;
}

void Mouse::clear()
{
	leaftDown = rightDown = midDown = false;
}

void Mouse::clearEventBuffer()
{
	while (!this->mouseBuffer.empty()) {
		this->mouseBuffer.pop();
	}
}
void Mouse::set_captureEvent(bool status)
{
	this->captureEvent = status;
}


void Mouse::onMouseMoveRaw(int x, int y)
{
	if (this->captureEvent) {

		this->mouseBuffer.push(mouseEvent(mouseEvent::EventType::RAW_MOVE, x, y));
	}
}

/*Mouse event*/
mouseEvent::mouseEvent():
	type(EventType::INVALID),
	x(0),
	y(0)
{}

mouseEvent::mouseEvent(const EventType type, const int x, const int y):
	type(type),
	x(x),
	y(y)
{}

bool mouseEvent::IsValid()
{
	return !(type == EventType::INVALID);
}

mouseEvent::EventType mouseEvent::getType() const
{
	return type;
}

MousePoint mouseEvent::getPos() const
{
	return MousePoint({x,y});
}

int mouseEvent::getPosX() const
{
	return x;
}

int mouseEvent::getPosY() const
{
	return y;
}
