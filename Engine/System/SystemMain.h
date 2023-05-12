#pragma once

#include <memory>

#include "Looper.h"
#include "System/Screen.h"

class SystemMain
{
public:
	bool StartUp();
	bool Finalize() const;
	void Run() const;

private:
	std::unique_ptr<Looper> looper;
};

