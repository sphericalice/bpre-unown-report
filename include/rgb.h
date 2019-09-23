#ifndef GUARD_RGB_H
#define GUARD_RGB_H

#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))

#define RGB_BLACK RGB(0, 0, 0)

#endif // GUARD_RGB_H