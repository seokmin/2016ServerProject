#pragma once
class IOCPManager
{
public:
	IOCPManager*		GetInstance();
	void				Initialize();
private:
	IOCPManager() {};
public:
private:
	static IOCPManager*	_instance ;
	bool				_initialized = false;
};