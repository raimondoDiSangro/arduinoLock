/////////////////////////////////////////////////////////////
//Progetto di MMSA: Vincenzo Maria Giulio Martemucci       //
//Prof: Donato Impedovo                                    //
//Serratura azionabile con impronta digitale               //
/////////////////////////////////////////////////////////////


#include <Adafruit_Fingerprint.h>

#include <Servo.h>
Servo myservo;
int pos = 0;

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(2, 3);

#else

#define mySerial Serial1

#endif



const int buzzerPin = 10; // declaring the PWM pin</p><p>void setup() {
  

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  myservo.attach(9); 
 
  Serial.begin(9600);
  Serial.begin(8600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Impronta digitale trovata!");
  } else {
    Serial.println("Impronta digitale non trovata :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Lettura dei parametri del sensore"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Il sensore non contiene alcuna impronta digitale. Utilizza la libreria 'enroll'.");
  }
  else {
    Serial.println("In attesa di un impronta valida...");
      Serial.print("Il sensore contiene "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
pinMode(13,OUTPUT);
}

void loop()                     // loop di getFinger per l'acquisizione dell'impronta
{
  getFingerprintID();
  delay(50);            

}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Immagine dito acquisita");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Nessun impronta rilevata");
      delay(500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Errore di imaging");
      return p;
    default:
      Serial.println("Errore sconosciuto");
      return p;
  }

  // OK immagine acquisita con successo

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Immagine convertita");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Immagine troppo sfocata");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Errore di acquisizione, riprovare");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Errore di acquisizione, riprovare");
      return p;
    default:
      Serial.println("Errore sconosciuto");
      return p;
  }

  // OK immagine convertita!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Impronta trovata!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Errore di comunicazione");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Nessuna corrispondenza");
    pinMode(buzzerPin, OUTPUT); //addigning pin to Output mode</p><p>}</p><p>void loop() {
    tone(buzzerPin, 150);
    delay(100);
    noTone(buzzerPin);
    delay(100);

    return p;
  } else {
    Serial.println("Errore sconosciuto");
    return p;
  }

  // found a match!
  Serial.print("Immagine trovata con #ID"); Serial.print(finger.fingerID);
  Serial.print(" confidenza di: "); Serial.println(finger.confidence);

  for (pos = 0; pos <= 90; pos += 1) {
  myservo.write(pos);              
  delay(5);                       
  }
  delay(5000);
  for (pos = 90; pos >= 0; pos -= 1) {
  myservo.write(pos);              
  delay(5);                       
  }
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

 
  return finger.fingerID;
}
