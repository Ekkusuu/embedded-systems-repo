#include "AppConfig.h"

namespace AppConfig {
byte KeypadRows[KeypadRowCount] = {7, 6, 5, 4};
byte KeypadColumns[KeypadColumnCount] = {3, 2, A3, A2};

char KeyLayout[KeypadRowCount][KeypadColumnCount] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
}
