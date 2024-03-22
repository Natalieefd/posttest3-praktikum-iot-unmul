#include <ESP8266WiFi.h> 
#include <ThingerESP8266.h>
#include <DHT.h>

// KONFIGURASI AWAL

#define USERNAME "nataliefuad" //Username thinger.io
#define DEVICE_ID "POSTTEST2_IoT_040_050" // Lihat pada setting dari device yang sudah dibuat di thinger.io
#define DEVICE_CREDENTIAL "0f5cCj-jb5wV6lUo" // Lihat pada setting dari device yang sudah dibuat di thinger.io (Generate random saja)

#define SSID "o.o" 
#define SSID_PASSWORD "mauapakamuu"

#define DHTPIN D6 //Pin data dari DHT terhubung ke pin D3 NodeMCU
#define BUZZER D5 //Pin data dari DHT terhubung ke pin D5 NodeMCU
#define LEDPIN1 D1 //Kaki anoda dari LED MERAH terhubung ke pin D0 NodeMCU
#define LEDPIN2 D2 //Kaki anoda dari LED KUNING terhubung ke pin D1 NodeMCU
#define LEDPIN3 D3 //Kaki anoda dari LED HIJAU terhubung ke pin D2 NodeMCU
#define DHTTYPE DHT11 //Mengatur TYPE DHT (Karena ada 2 jenis [DHT11 & DHT22])

// Inisialisasi objek untuk terhubung ke Thinger.io
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Inisialisasi objek untuk DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variabel untuk menyimpan data suhu dan kelembaban
float suhu_celcius, suhu_fahrenheit, suhu_kelvin, kelembaban;

void setup() {
    pinMode(BUZZER, OUTPUT);
    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);
    pinMode(LEDPIN3, OUTPUT);

    dht.begin();
    thing.add_wifi(SSID, SSID_PASSWORD);

    // Setup sensor DHT11
    thing["dht11"] >> [](pson& out){
      out["kelembaban"] = kelembaban;
      out["suhu_c"] = suhu_celcius;
      out["suhu_f"] = suhu_fahrenheit;
      out["suhu_k"] = suhu_kelvin;
    };
}

void loop() {
    thing.handle();
    float h = dht.readHumidity(); //Membaca kelembaban
    float c = dht.readTemperature(); //Membaca suhu dalam satuan Celcius
    float f = dht.computeHeatIndex(c, h, true); //Membaca suhu dalam satuan Fahenheit
    float k = c + 273.15; //Membaca suhu dalam satuan Kelvin

    kelembaban = h; // Update nilai kelembaban
    suhu_celcius = c; // Update nilai suhu
    suhu_fahrenheit = f; // Update nilai suhu
    suhu_kelvin = k; // Update nilai suhu

    if (suhu_celcius >= 36){
      digitalWrite(LEDPIN1, HIGH);
      digitalWrite(LEDPIN2, LOW);
      digitalWrite(LEDPIN3, LOW);
      tone(BUZZER, 100);
    }
    
    if (suhu_celcius >= 30 && suhu_celcius <= 36){
      digitalWrite(LEDPIN1, LOW);
      digitalWrite(LEDPIN2, HIGH);
      digitalWrite(LEDPIN3, LOW);
      noTone(BUZZER);
    }

    if (suhu_celcius <= 30){
      digitalWrite(LEDPIN1, LOW);
      digitalWrite(LEDPIN2, LOW);
      digitalWrite(LEDPIN3, HIGH);
      noTone(BUZZER);
    }
}
