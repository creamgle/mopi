# mopi
Melancholy Operating (System) Platform Interface

**PLEASE NOTE**
- mopi is currently completely unusable in any sort of program, only use it now for fun or reference
- Im a shitty hobbiest programer
- This is mostly a learning project
- This is currently largely based on the library GLFW
but i do plan to branch away from that as i get further
and more comfortable with this project

---
**Supported Platforms**
- [ ] Windows
- [ ] Linux
- [x] macOS

---
**EXAMPLE CODE**
```c
#include <mopi/mopi.h>

int main() 
{
    // Initialises the core of 'mopi'
    // returns false if it fails
    if (!mopi_Init())
        // Error checking

    // windows are stored in a 'mopi_Window' struct
    // creates window using the supplied fields
    mopi_Window *window = mopi_CreateWindow(/width/, /height/, /title/);

    // window will be null of 'mopi_CreateWindow()' has failed 
    if (!window)
        // More error checking

    // main game loop
    // returns true if window should close
    while (!mopi_WindowShouldClose())
    {
        mopi_PollEvents();
    }

    // destroys and clears up all memory
    // associated with the window pointer
    mopi_DestroyWindow(window);

    // destroys and clears up all memory
    // associated with the core of 'mopi'
    mopi_Destroy();
}
```

### Features / Planned Features
- Windowing
  - [ ] basic window creation
  - [ ] window input
  - [ ] OpenGL support
  - Callbacks
    - [ ] messages
    - [ ] window resize
    - [ ] window closed
    - [ ] keyboard input
    - [ ] mouse input
    - [ ] minimized
    - [ ] fullscreened