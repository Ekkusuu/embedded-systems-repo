#include "AccessControlApp.h"

#include <stdio.h>
#include <string.h>

#include "../config/AppConfig.h"
#include "../io/StdioBridge.h"

AccessControlApp::AccessControlApp(LcdDisplay &lcdDisplayRef,
                                   KeypadInput &keypadInputRef,
                                   LedController &ledControllerRef,
                                   const char *expectedCodeValue,
                                   uint8_t codeLengthValue)
    : lcdDisplay(lcdDisplayRef), keypadInput(keypadInputRef),
      ledController(ledControllerRef), codeLength(codeLengthValue) {
  strncpy(activeCode, expectedCodeValue, AppConfig::CodeLength);
  activeCode[AppConfig::CodeLength] = '\0';
}

void AccessControlApp::begin() {
  lcdDisplay.begin();
  ledController.begin();

  StdioBridge::begin(AppConfig::SerialBaudRate, &lcdDisplay.getRawLcd(),
                     &keypadInput.getRawKeypad());

  printf("Lab 1.2 started. Enter %u digits on keypad.\n", codeLength);
  printf("Press A to change the code.\n");
  showPrompt();
}

void AccessControlApp::loop() {
  char enteredCode[AppConfig::CodeLength + 1] = {0};
  char firstKey = readKeypadKey();

  if (firstKey == 'A') {
    handleCodeChange();
    delay(2000);
    ledController.clear();
    showPrompt();
    return;
  }

  StdioBridge::syncLcdCursor(0, 1);

  enteredCode[0] = firstKey;
  fprintf(StdioBridge::lcdStream(), "%c", enteredCode[0]);

  for (uint8_t index = 1; index < codeLength; index++) {
    enteredCode[index] = readDigitKey();
    fprintf(StdioBridge::lcdStream(), "%c", enteredCode[index]);
  }

  enteredCode[codeLength] = '\0';

  handleCodeVerification(enteredCode);

  delay(2000);
  ledController.clear();
  showPrompt();
}

char AccessControlApp::readKeypadKey() {
  char key = 0;
  fscanf(StdioBridge::keypadStream(), "%c", &key);
  return key;
}

char AccessControlApp::readDigitKey() {
  char key = readKeypadKey();
  while (key < '0' || key > '9') {
    key = readKeypadKey();
  }
  return key;
}

void AccessControlApp::readCode(char *buffer, char firstDigit) {
  uint8_t startIndex = 0;

  if (firstDigit >= '0' && firstDigit <= '9') {
    buffer[0] = firstDigit;
    fprintf(StdioBridge::lcdStream(), "%c", buffer[0]);
    startIndex = 1;
  }

  for (uint8_t index = startIndex; index < codeLength; index++) {
    buffer[index] = readDigitKey();
    fprintf(StdioBridge::lcdStream(), "%c", buffer[index]);
  }

  buffer[codeLength] = '\0';
}

void AccessControlApp::handleCodeVerification(const char *enteredCode) {
  lcdDisplay.clear();
  StdioBridge::syncLcdCursor(0, 0);
  fprintf(StdioBridge::lcdStream(), "Code: %s", enteredCode);
  printf("Entered code: %s\n", enteredCode);

  StdioBridge::syncLcdCursor(0, 1);
  if (isCodeValid(enteredCode)) {
    ledController.showAccessGranted();
    fprintf(StdioBridge::lcdStream(), "Access Granted  ");
    printf("Result: VALID\n");
  } else {
    ledController.showAccessDenied();
    fprintf(StdioBridge::lcdStream(), "Access Denied   ");
    printf("Result: INVALID\n");
  }
}

void AccessControlApp::handleCodeChange() {
  char oldCode[AppConfig::CodeLength + 1] = {0};
  char newCode[AppConfig::CodeLength + 1] = {0};

  lcdDisplay.clear();
  StdioBridge::syncLcdCursor(0, 0);
  fprintf(StdioBridge::lcdStream(), "Old Code:");
  StdioBridge::syncLcdCursor(0, 1);
  readCode(oldCode);

  if (!isCodeValid(oldCode)) {
    ledController.showAccessDenied();
    lcdDisplay.clear();
    StdioBridge::syncLcdCursor(0, 0);
    fprintf(StdioBridge::lcdStream(), "Wrong Old Code");
    StdioBridge::syncLcdCursor(0, 1);
    fprintf(StdioBridge::lcdStream(), "Code Unchanged");
    printf("Code change failed: wrong old code.\n");
    return;
  }

  lcdDisplay.clear();
  StdioBridge::syncLcdCursor(0, 0);
  fprintf(StdioBridge::lcdStream(), "New Code:");
  StdioBridge::syncLcdCursor(0, 1);
  readCode(newCode);

  strcpy(activeCode, newCode);
  ledController.showAccessGranted();

  lcdDisplay.clear();
  StdioBridge::syncLcdCursor(0, 0);
  fprintf(StdioBridge::lcdStream(), "Code Updated");
  StdioBridge::syncLcdCursor(0, 1);
  fprintf(StdioBridge::lcdStream(), "Saved: %s", activeCode);

  printf("Code changed successfully. New code: %s\n", activeCode);
}

bool AccessControlApp::isCodeValid(const char *enteredCode) const {
  return strcmp(enteredCode, activeCode) == 0;
}

void AccessControlApp::showPrompt() {
  lcdDisplay.clear();
  StdioBridge::syncLcdCursor(0, 0);
  fprintf(StdioBridge::lcdStream(), "Enter Code:");
  StdioBridge::syncLcdCursor(0, 1);
  fprintf(StdioBridge::lcdStream(), "                ");
  StdioBridge::syncLcdCursor(0, 1);
}
