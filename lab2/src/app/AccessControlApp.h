#pragma once

#include "../config/AppConfig.h"
#include "../peripherals/KeypadInput.h"
#include "../peripherals/LcdDisplay.h"
#include "../peripherals/LedController.h"

class AccessControlApp {
public:
  AccessControlApp(LcdDisplay &lcdDisplayRef, KeypadInput &keypadInputRef,
                   LedController &ledControllerRef, const char *expectedCodeValue,
                   uint8_t codeLengthValue);

  void begin();
  void loop();

private:
  char readKeypadKey();
  char readDigitKey();
  void readCode(char *buffer, char firstDigit = '\0');
  void handleCodeVerification(const char *enteredCode);
  void handleCodeChange();
  bool isCodeValid(const char *enteredCode) const;
  void showPrompt();

  LcdDisplay &lcdDisplay;
  KeypadInput &keypadInput;
  LedController &ledController;
  char activeCode[AppConfig::CodeLength + 1];
  uint8_t codeLength;
};
