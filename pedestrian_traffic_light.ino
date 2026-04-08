const int pinKendaraanMerah = 8;
const int pinKendaraanKuning = 7;
const int pinKendaraanHijau = 6;

const int pinPedMerah1 = 10;
const int pinPedHijau1 = 9;

const int pinPedMerah2 = 5;
const int pinPedHijau2 = 4;

const int pinTombol1 = 2; // Pin untuk Interrupt 0
const int pinTombol2 = 3; // Pin untuk Interrupt 1

// menggunakan 'volatile' untuk variabel yang akan diubah di dalam fungsi interupsi (ISR)
volatile bool adaPenyeberang = false;

void setup() {
  pinMode(pinKendaraanMerah, OUTPUT);
  pinMode(pinKendaraanKuning, OUTPUT);
  pinMode(pinKendaraanHijau, OUTPUT);
  
  pinMode(pinPedMerah1, OUTPUT);
  pinMode(pinPedHijau1, OUTPUT);
  pinMode(pinPedMerah2, OUTPUT);
  pinMode(pinPedHijau2, OUTPUT);

  // Menggunakan PULLUP internal agar tidak perlu resistor tambahan di tombol
  pinMode(pinTombol1, INPUT_PULLUP);
  pinMode(pinTombol2, INPUT_PULLUP);

  // Mendaftarkan fungsi interupsi. 
  // Mode FALLING berarti interupsi terpicu saat tombol ditekan (tegangan turun dari HIGH ke LOW)
  attachInterrupt(digitalPinToInterrupt(pinTombol1), fungsiInterupsiTombol, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinTombol2), fungsiInterupsiTombol, FALLING);

  setKondisiAwal();
}

void loop() {
  // Mengecek apakah ada tombol penyeberang yang ditekan (statusnya diubah oleh ISR)
  if (adaPenyeberang) {
    
    // FASE 1: Kendaraan berhenti, Penyeberang jalan
    digitalWrite(pinKendaraanHijau, LOW);
    digitalWrite(pinKendaraanMerah, HIGH);
    
    digitalWrite(pinPedMerah1, LOW);
    digitalWrite(pinPedMerah2, LOW);
    digitalWrite(pinPedHijau1, HIGH);
    digitalWrite(pinPedHijau2, HIGH);
    
    delay(5000); // Waktu untuk menyeberang

    // FASE 2: Penyeberang berhenti, transisi lampu kuning untuk kendaraan
    digitalWrite(pinPedHijau1, LOW);
    digitalWrite(pinPedHijau2, LOW);
    digitalWrite(pinPedMerah1, HIGH);
    digitalWrite(pinPedMerah2, HIGH);
    
    digitalWrite(pinKendaraanMerah, LOW);
    digitalWrite(pinKendaraanKuning, HIGH);
    
    delay(2000); // Lama lampu kuning menyala

    // FASE 3: Kembali ke kondisi awal (Kendaraan jalan)
    digitalWrite(pinKendaraanKuning, LOW);
    setKondisiAwal(); 

    // PENTING: Kembalikan status ke false agar siklus penyeberangan tidak berulang terus-menerus
    adaPenyeberang = false;
  }
}

void setKondisiAwal() {
  // Kondisi default saat tidak ada yang menyeberang
  digitalWrite(pinKendaraanHijau, HIGH);
  digitalWrite(pinKendaraanKuning, LOW);
  digitalWrite(pinKendaraanMerah, LOW);

  digitalWrite(pinPedMerah1, HIGH);
  digitalWrite(pinPedHijau1, LOW);
  digitalWrite(pinPedMerah2, HIGH);
  digitalWrite(pinPedHijau2, LOW);
}

// PENTING: Ini adalah fungsi ISR (Interrupt Service Routine).
// Fungsi ini harus secepat mungkin dieksekusi, jadi cukup gunakan untuk mengubah status variabel saja.
void fungsiInterupsiTombol() {
  adaPenyeberang = true;
}