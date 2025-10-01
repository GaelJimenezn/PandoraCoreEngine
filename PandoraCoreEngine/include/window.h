#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Manages the application's native window and message loop.
 * @details Encapsulates the creation, destruction, and message handling
 * for a standard Win32 window (HWND), which serves as the primary
 * rendering surface for the application.
 */
class
Window {
public:
  Window() = default;
  ~Window() = default;

  /**
   * @brief Creates and displays the application window.
   * @param hInstance The handle to the application instance.
   * @param nCmdShow A flag that specifies how the window is to be shown.
   * @param wndproc A pointer to the application-defined window procedure.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief Placeholder for window-related update logic.
   */
  void
  update();

  /**
   * @brief Placeholder for window-related rendering logic.
   */
  void
  render();

  /**
   * @brief Releases the window handle and unregisters the window class.
   */
  void
  destroy();

public:
  /**
   * @brief The handle to the native Win32 window.
   */
  HWND m_hWnd = nullptr;

  /**
   * @brief The current width of the window's client area in pixels.
   */
  unsigned int m_width;

  /**
   * @brief The current height of the window's client area in pixels.
   */
  unsigned int m_height;
private:
  /**
   * @brief The handle to the application instance.
   */
  HINSTANCE m_hInst = nullptr;

  /**
   * @brief Rectangle struct defining the window dimensions.
   */
  RECT m_rect;

  /**
   * @brief The name displayed in the window's title bar.
   */
  std::string m_windowName = "PandoraCoreEngine";
};