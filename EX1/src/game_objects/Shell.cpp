#include "Shell.h"

int Shell::getLocationX() const { return location_x; }
int Shell::getLocationY() const { return location_y; }
void Shell::setLocation(int x, int y) { location_x = x; location_y = y; }