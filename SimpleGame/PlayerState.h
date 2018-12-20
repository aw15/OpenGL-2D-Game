#pragma once


class ObjectState
{
public:
	virtual ~ObjectState() {};
	virtual string handleInput(Object& object,char key, KEY_STATUS status) = 0;
};


class MoveState : public ObjectState
{
public:
	virtual ~MoveState() {};
	virtual string handleInput(Object& object,char key, KEY_STATUS status);

private:

};

class IdleState : public ObjectState
{
public:
	virtual ~IdleState() {};
	virtual string handleInput(Object& object,char key, KEY_STATUS status);
public:

};

