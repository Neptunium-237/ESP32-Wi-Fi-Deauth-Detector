# ESP32 Wi-Fi Deauth Detector

Bu proje, ESP32 kullanarak ortamdaki Wi-Fi Deauth (bağlantı kesme) saldırılarını tespit eden bir güvenlik aracıdır. Saldırı algılandığında 0.96" OLED ekran üzerinden görsel uyarı verir.

## Özellikler
- **Gerçek Zamanlı Tarama:** 1-13 arası tüm Wi-Fi kanallarını sürekli tarar.
- **Saldırı Tespiti:** Havada uçuşan `0xC0` (Deauth) ve `0xA0` (Disassociation) paketlerini yakalar.
- **OLED Görsel Uyarı:** Saldırı anında ekranda büyük bir uyarı mesajı ve paket sayısı gösterilir.

## Donanım Gereksinimleri
- ESP32 WROOM 32 Geliştirme Kartı
- 0.96" I2C OLED Ekran (SSD1306)
- Jumper Kablolar

## Bağlantı Şeması
| OLED Pin | ESP32 Pin |
| :--- | :--- |
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## Kurulum
1. Arduino IDE üzerinden `Adafruit SSD1306` ve `Adafruit GFX` kütüphanelerini kurun.
2. `deauth_detector.ino` dosyasını ESP32'nize yükleyin.
3. Yükleme sırasında **BOOT** tuşuna basılı tutmayı unutmayın.
