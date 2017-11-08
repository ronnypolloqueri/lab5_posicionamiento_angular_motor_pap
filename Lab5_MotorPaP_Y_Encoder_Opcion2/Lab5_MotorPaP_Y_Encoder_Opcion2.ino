int retardo = 5;
int dato_rx;
int numero_pasos = 0;
String leeCadena;

#define PIN1 8
#define PIN2 9
#define PIN3 10
#define PIN4 11

void setup() {
    Serial.begin(9600);
    pinMode(PIN4, OUTPUT); // IN4
    pinMode(PIN3, OUTPUT); // IN3
    pinMode(PIN2, OUTPUT);  // IN2
    pinMode(PIN1, OUTPUT);  // IN1
}

void loop() {
    while(Serial.available()) {
        delay(retardo);
        char c = Serial.read(); // Lee los caracteres
        leeCadena +=c; 
    }
    if (leeCadena.length() > 0) {
        dato_rx = leeCadena.toInt();
        Serial.print(dato_rx);
        Serial.println(" Grados");
        delay(retardo);
        dato_rx = ( dato_rx  * 1.4222222222); // Ajuste a 512 vueltas a los 360
    }

    while ( dato_rx > numero_pasos ) { // Giro hacia la iquierda en grados
        paso_izq();
        numero_pasos = numero_pasos + 1;
    }

    while ( dato_rx < numero_pasos ) { // Giro hacia la iquierda en grados
        paso_der();
        numero_pasos = numero_pasos - 1;
    }
    leeCadena = "";
    apagado();
} ////////////////////////// Fin del Loop /////////////////////////////

void paso_der() { // Paso a la derecha
    setDigitalWrite(LOW , LOW , HIGH, HIGH);
    delay(retardo);
    setDigitalWrite(LOW , HIGH, HIGH, LOW);
    delay(retardo);
    setDigitalWrite(HIGH, HIGH, LOW , LOW);
    delay(retardo);
    setDigitalWrite(HIGH, LOW , LOW , HIGH);
    delay(retardo);
}

void paso_izq() { // Paso a la izquierda
    setDigitalWrite(HIGH, HIGH, LOW , LOW);
    delay(retardo);
    setDigitalWrite(LOW , HIGH, HIGH, LOW);
    delay(retardo);
    setDigitalWrite(LOW , LOW , HIGH, HIGH);
    delay(retardo);
    setDigitalWrite(HIGH, LOW , LOW , HIGH);
    delay(retardo);
}

void apagado() { // Apagado del motor
    setDigitalWrite(LOW, LOW , LOW , LOW);
}

void setDigitalWrite( int v1, int v2, int v3, int v4 ){
    digitalWrite(PIN4, v1);
    digitalWrite(PIN3, v2);
    digitalWrite(PIN2 , v3);
    digitalWrite(PIN1 , v4);
}
