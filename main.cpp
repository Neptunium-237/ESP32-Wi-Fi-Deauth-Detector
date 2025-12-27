#include <WiFi.h>
#include <esp_wifi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Değişkenler
int deauth_count = 0;
unsigned long last_display_update = 0;

// Paket yakalama fonksiyonu
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return; // Sadece yönetim paketlerine bak

  const wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t* payload = pkt->payload;

  // 0xC0 Deauth paketinin kodudur, 0xA0 ise Disassociation (ayrılma)
  if (payload[0] == 0xC0 || payload[0] == 0xA0) {
    deauth_count++;
  }
}

void setup() {
  Serial.begin(115200);

  // Ekranı Başlat
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Sistem Aktif...");
  display.println("Saldiri Bekleniyor");
  display.display();

  // Wi-Fi'yi Promiscuous (Karma) moda sok
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // 1. kanaldan başla
}

void loop() {
  unsigned long now = millis();

  // Her 2 saniyede bir ekranı güncelle
  if (now - last_display_update > 2000) {
    display.clearDisplay();
    
    if (deauth_count > 5) { // Eğer 2 saniyede 5'ten fazla paket gelirse
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("!!! UYARI !!!");
      display.setTextSize(1);
      display.setCursor(0, 30);
      display.println("DEAUTH SALDIRISI");
      display.print("Paket: ");
      display.println(deauth_count);
      Serial.println("SALDIRI TESPIT EDILDI!");
    } else {
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Durum: GUVENDE");
      display.println("--------------------");
      display.print("Sinyal Takibi Aktif");
      deauth_count = 0; // Sayacı sıfırla
    }
    
    display.display();
    deauth_count = 0;
    last_display_update = now;

    // Kanalları otomatik gez (1'den 13'e kadar)
    static int channel = 1;
    channel++;
    if (channel > 13) channel = 1;
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  }
}
