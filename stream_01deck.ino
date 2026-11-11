#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int BTN_GELB    = 13;  
const int BTN_ROT     = 4;   
const int BTN_GRUEN   = 16;   
const int BTN_SCHWARZ = 5;   
const int BTN_BLAU    = 17;  

unsigned long letzteAktivierung = 0;
const unsigned long entprellZeit = 200;

// Icons (16x16 Pixel)
const unsigned char iconCustom [] PROGMEM = {
  0xFF, 0xFF, 0x81, 0x81, 0xBD, 0xBD, 0xA5, 0xA5, 0xA5, 0xA5, 0xBD, 0xBD, 0x81, 0x81, 0xFF, 0xFF,
  0xFF, 0xFF, 0x81, 0x81, 0xBD, 0xBD, 0xA5, 0xA5, 0xA5, 0xA5, 0xBD, 0xBD, 0x81, 0x81, 0xFF, 0xFF
};

const unsigned char iconMute [] PROGMEM = {
  0x07, 0xE0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x07, 0xE0, 0x03, 0xC0, 0x23, 0xC4, 0x17, 0xE8,
  0x0F, 0xF0, 0x07, 0xE0, 0x01, 0x80, 0x01, 0x80, 0x0F, 0xF0, 0x00, 0x00, 0xC3, 0xC3, 0x66, 0x66
};

const unsigned char iconYouTube [] PROGMEM = {
  0x00, 0x00, 0x1F, 0xF8, 0x3F, 0xFC, 0x7E, 0xFE, 0x78, 0x1E, 0x70, 0x0E, 0x7E, 0xFE, 0x7E, 0xFE,
  0x7E, 0xFE, 0x70, 0x0E, 0x78, 0x1E, 0x7E, 0xFE, 0x3F, 0xFC, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00
};

const unsigned char iconObs2 [] PROGMEM = {
  0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0, 0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC, 0x7F, 0xFE, 0xFF, 0xFF,
  0x7F, 0xFE, 0x3F, 0xFC, 0x1F, 0xF8, 0x0F, 0xF0, 0x07, 0xE0, 0x03, 0xC0, 0x01, 0x80, 0x00, 0x00
};

const unsigned char iconObs1 [] PROGMEM = {
  0x18, 0x18, 0x3C, 0x3C, 0x7E, 0x7E, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);

  pinMode(BTN_GELB, INPUT_PULLUP);
  pinMode(BTN_ROT, INPUT_PULLUP);
  pinMode(BTN_GRUEN, INPUT_PULLUP);
  pinMode(BTN_SCHWARZ, INPUT_PULLUP);
  pinMode(BTN_BLAU, INPUT_PULLUP);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }

  showHomeScreen();
}

void loop() {
  unsigned long aktuelleZeit = millis();

  if (aktuelleZeit - letzteAktivierung > entprellZeit) {
    if (digitalRead(BTN_GELB) == LOW) {
      triggerAction("GELB: Custom", "BTN:GELB", iconCustom);
      letzteAktivierung = millis();
    } 
    else if (digitalRead(BTN_ROT) == LOW) {
      triggerAction("ROT: Mute", "BTN:ROT", iconMute);
      letzteAktivierung = millis();
    } 
    else if (digitalRead(BTN_GRUEN) == LOW) {
      triggerAction("GRUEN: Play", "BTN:GRUEN", iconYouTube);
      letzteAktivierung = millis();
    } 
    else if (digitalRead(BTN_SCHWARZ) == LOW) {
      triggerAction("SCHWARZ: OBS 2", "BTN:SCHWARZ", iconObs2);
      letzteAktivierung = millis();
    } 
    else if (digitalRead(BTN_BLAU) == LOW) {
      triggerAction("BLAU: OBS 1", "BTN:BLAU", iconObs1);
      letzteAktivierung = millis();
    }
  }

  static unsigned long lastHomeUpdate = 0;
  if (letzteAktivierung != 0 && (millis() - letzteAktivierung > 1500)) {
    showHomeScreen();
    letzteAktivierung = 0;
  }
}

void triggerAction(String actionText, String serialMsg, const unsigned char* icon) {
  Serial.println(serialMsg);

  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.fillRect(0, 0, 128, 18, SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor(4, 5);
  display.println("AKTION AUSGEFUEHRT");

  display.drawBitmap(12, 30, icon, 16, 16, SSD1306_WHITE);

  display.setTextColor(SSD1306_WHITE);
  display.setCursor(35, 34);
  display.println(actionText);

  display.display();
}

void showHomeScreen() {
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.fillRect(0, 0, 128, 18, SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor(4, 5);
  display.println("USB STREAM DECK");

  display.setTextColor(SSD1306_WHITE);
  display.setCursor(6, 28);
  display.println("Bereit (Yippii");

  display.setCursor(6, 48);
  display.println("ESP32 Stream Deck");
  display.display();
}