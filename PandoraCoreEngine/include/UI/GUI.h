#pragma once
#include "Prerequisites.h"

#include "imgui.h"

class Window;
class Device;
class DeviceContext;

class
GUI {
 public:
  GUI() = default; 
  ~GUI() = default;

  void
  awake();

  void
  init(Window& window, Device *device, DeviceContext& deviceContext);

  void
  update(float deltaTime);

  void
  render();

  void
  destroy();



};