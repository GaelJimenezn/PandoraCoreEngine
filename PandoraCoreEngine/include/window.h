#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Manages a Win32 application window.
 *
 * Handles window creation, registration, and basic properties like dimensions
 * and window handle (HWND).
 */
class
Window {
public:
  Window()  = default;
  ~Window() = default;

  /**
   * @brief Creates and initializes the window.
   *
   * Registers the window class and creates the window instance.
   *
   * @param hInstance App instance handle.
   * @param nCmdShow  Show command flag.
   * @param wndproc   Pointer to the message processing function.
   * @return S_OK if successful.
   */
  HRESULT
  init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief Placeholder for window logic updates.
   */
  void
  update();
  
  /**
   * @brief Placeholder for window rendering (if GDI were used).
   */
  void
  render();
  
  /**
   * @brief Destroys the window handle.
   */
  void
  destroy();

public:
  HWND m_hWnd = nullptr;    ///< Handle to the window.
  unsigned int m_width;     ///< Window client width.
  unsigned int m_height;    ///< Window client height.

private:
  HINSTANCE m_hInst = nullptr; ///< Application instance handle.
  RECT m_rect;                 ///< Window rectangle dimensions.
  std::string m_windowName = "PandoraCoreEngine"; ///< Title bar text.
};