#include "Stepper.h"
#define STEPS 2048 // NUmero de pasos de una revolución de eje interno
                 // 64 -> 2048 pasos para una revolucion de eje externo
#define NUM_PASOS 85
volatile boolean TurnDetected; // need volatile for Interrups              
volatile boolean rotationdirection; // CW or CCW rotation

const int PinCLK = 2; // Generando interrupciones usando CLK signal (A encoder)
const int PinDT = 3; // Leyendo señal DT (B encoder)
const int PinSW = 4; // Leyendo Push Button switch (Pulsador)

int RotaryPosition = 0; // Para guardar la posición del Motor Paso a Paso

int PrevPosition; // Anterior valor de posicion de rotación anterior para
                  // verificar precisión

int StepsToTake;  // Cuanto mover el motor PaP

// Configurar la secuencia adecuada
// In1, In2, In3, In4 en la secuencia 1-3-2-4
Stepper small_stepper(STEPS, 8, 10, 9, 11);


// Interrumpir rutina si CLK va de ALTO a BAJO
void isr() {
    delay(4); // Delay for debouncing
    int dr_PinCLK = digitalRead(PinCLK);
    Serial.print("PinCLK: ");
    Serial.println(dr_PinCLK); 
    if( dr_PinCLK ){
        rotationdirection = digitalRead(PinDT);      
        Serial.println("isr() digitalRead(PinDT)"); 
    }
    else{
        rotationdirection = !digitalRead(PinDT);
        
        Serial.println("isr() NOT digitalRead(PinDT)"); 
    }
    TurnDetected = true;
}

void setup() {
    pinMode(PinCLK, INPUT); // PinA señal CLK 
    pinMode(PinDT, INPUT);  // PinB Señal DT
    pinMode(PinSW, INPUT);  // Push Button
    digitalWrite(PinSW, HIGH); // Pull-Up resistencia para switch
    attachInterrupt(0, isr, FALLING); // interrumpir 0 siempre conectado a pin 2
    Serial.begin(9600);
}

void loop() {
    //delay(1000);
    
    //Serial.print("TurnDetected final: ");
    //Serial.println(TurnDetected);
    small_stepper.setSpeed(5); // El maximo parece ser 700
    if( !(digitalRead(PinSW)) ){ // verificar si el botón esta presionado
      
        Serial.println("Boton switch presionado");
        if( RotaryPosition == 0 ) { // verificar si el botón estaba presionado
          
            Serial.println("Boton switch YA ESTABA presionado");
        } else {
          
            Serial.println("Boton switch NO ESTABA presionado");
            small_stepper.step( -( RotaryPosition * 50 )); // 50
            RotaryPosition = 0; // Reinicia posición a CERO
        }
    }

    // Ejecuta si la rotacion fue detectada
    if ( TurnDetected ) {
        //Serial.println("Rotacion detectada");
        PrevPosition = RotaryPosition; // Guardar anterior posicion en variable
        if( rotationdirection) {
            RotaryPosition = RotaryPosition - 1;
        }// decrementa posicion por 1
        else  {
            RotaryPosition = RotaryPosition + 1;
        }// incrementa posicion por 1


        Serial.print("Prev Pos: ");
        Serial.print(PrevPosition);
        Serial.print(" Rotary Pos: ");
        Serial.println(RotaryPosition);
        
        TurnDetected = false; // No repita loop IF hasta nueva rotacion detectada
       
        // Qué dirección mover el motor PaP 
        if (( PrevPosition + 1 ) == RotaryPosition ) { // Mueve motor CW
          
            Serial.println("Mueve CW");
            StepsToTake = NUM_PASOS; // 50  24*2 = 48   48 = 15°;  24 = 7.5° (Pasos a seguir)
            small_stepper.step(StepsToTake);
        }
    
        if (( PrevPosition - 1 ) == RotaryPosition ) { // Mueve motor CCW
          
            Serial.println("Mueve CCW");        
            StepsToTake = -NUM_PASOS; // 50
            small_stepper.step(StepsToTake);
            
        }
        //Serial.print("TurnDetected final: ");
        //Serial.println(TurnDetected);
    
    }
}
