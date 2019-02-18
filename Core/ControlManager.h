#pragma once

#include "stdafx.h"
#include "../Utilities/SimpleLock.h"
#include "IMemoryHandler.h"

class BaseControlDevice;
class IInputRecorder;
class IInputProvider;
class Console;
struct ControlDeviceState;
enum class ControllerType;
enum class ExpansionPortDevice;

class ControlManager
{
private:
	vector<IInputRecorder*> _inputRecorders;
	vector<IInputProvider*> _inputProviders;
	
	//Static so that power cycle does not reset its value
	uint32_t _pollCounter;

protected:
	shared_ptr<Console> _console;
	SimpleLock _deviceLock;
	vector<shared_ptr<BaseControlDevice>> _controlDevices;
	shared_ptr<BaseControlDevice> _systemActionManager;

	void RegisterControlDevice(shared_ptr<BaseControlDevice> controlDevice);

	ControllerType GetControllerType(uint8_t port);
	uint8_t GetOpenBusMask(uint8_t port);

public:
	ControlManager(shared_ptr<Console> console);
	~ControlManager();

	void UpdateControlDevices();
	void UpdateInputState();

	uint32_t GetPollCounter();
	void SetPollCounter(uint32_t value);

	void RegisterInputProvider(IInputProvider* provider);
	void UnregisterInputProvider(IInputProvider* provider);

	void RegisterInputRecorder(IInputRecorder* recorder);
	void UnregisterInputRecorder(IInputRecorder* recorder);

	vector<ControlDeviceState> GetPortStates();

	shared_ptr<BaseControlDevice> GetControlDevice(uint8_t port);
	vector<shared_ptr<BaseControlDevice>> GetControlDevices();
	
	static shared_ptr<BaseControlDevice> CreateControllerDevice(ControllerType type, uint8_t port, shared_ptr<Console> console);

	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t value);
};