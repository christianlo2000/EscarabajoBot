/*
 * Sketch .....: EscarabajoBot.ino
 * objetivo....: robot escarabajo con 4 motores servos
 * status......: se agrego funcionalidad Bluetooth, en prueba. Anda autonomo sin problemas
 * 
 * placa.......: arduino uno + shield
 * 
 */
#include <Servo.h>
#include "Radar.h"
#include <SoftwareSerial.h>

#define DIST_MAX 400
SoftwareSerial mySerial(2, 4); // RX, TX
Servo pataDelD;
Servo pataTrasD;
Servo pataDelI;
Servo pataTrasI;
// pines sonar
const int TRIG = 7;
const int ECHO = 8;
// definimos objeto Radar
Radar sonar(TRIG, ECHO, DIST_MAX); // se crea objeto Ping y pasamos parametros requeridos

void setup() {
  // put your setup code here, to run once:
  // puerto serie
  Serial.begin(9600); //9600,38400,19200,57600,115200,74880
  mySerial.begin(9600);
  pataDelD.attach(3);
  pataDelI.attach(5);
  pataTrasD.attach(10);
  pataTrasI.attach(11);
  // alineamos todos los motores a 90 grados
  detener();
  // inicializamos los pines del sonar
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  delay(10000);
  
}
/*
void loop(){
  int distancia = sonar.getDistProm_cmMl();
  Serial.println(String(distancia));
  mySerial.println(String(distancia));
  delay(10000);
}
*/
void loop() {
  // put your main code here, to run repeatedly:
  // preguntamos si hay datos en el buffer bluetooth
  String datos;
  char dato;
  int valor = -1;
  while( mySerial.available() > 0 ){
     
     //Leemos los datos y los guardamos en la variable datos[]
     datos = mySerial.readStringUntil('\n');
     //dato = mySerial.read();
     Serial.println(String(datos)+" me llego de bt");
     //valor = parsearBTString(datos);
     datos = String(datos)+" cm";
     mySerial.println(datos);
     delay(10);
     mySerial.println("");
     valor = parsearBTString(String(datos));
     //retroceder2(16);
     //pruebaMotoresDC();
  }
  
  // consultamos el valor
  switch(int(valor)){
    case 0:
      // detener
      detener();
      mySerial.println("detener Bot");
      break;
    case 1:
      // avanzar
      caminarDelante2();
      mySerial.println("avanzar Bot");
      break;
    case 2:
      // retroceder();
      retroceder2(16);
      mySerial.println("retroceder Bot");
      break;  
    case 5:
      // Imprimir distancia
      int distancia = sonar.getDistProm_cmMl();
      mySerial.println(String(distancia)+" cm");
      break;
    case 6: 
      // girar derecha
      girarDerecha(10);
      mySerial.println("girar derecha Bot");
      break;
    case 7: 
      // girar izquierda
      girarIzquierda(10);
      mySerial.println("girar izquierda Bot");
      break; 
    default:
      Serial.println("No ingresaste opcion");
      break;
  }
  // limpiamos el buffer del bluetooth
  //Serial.flush();
  // Inicia el modo autonomo
  modoAutonomo();
  /*
  int dist_ant = 0;
  caminarDelante2();
  //int distancia = activarRadar();
  // obtengo distancia con la clase Radar instancia sonar
  int distancia = sonar.getDistProm_cmMl();
  Serial.println(String(distancia));
  // validamos distancia
  if (distancia < 10){
    // primero retrocedemos
    int intervalo = 16;
    retroceder2(intervalo); // intervalo es la cantidad de iteraciones
    girar(distancia);
  }
  // validamos si la distancia es la misma al anterior
  if (distancia == dist_ant and distancia >= 10){
    // se trabo con un obstaculo, probamos de levantar la pata para superar obstaculo
    levantarPata(pataDelD);
    levantarPata(pataDelI);
  }
  dist_ant = distancia;
  */
}

// Funciones
// modo autonomo
void modoAutonomo(){
  int dist_ant = 0;
  //caminarDelante2(); // anda ok sin las pilas traseras, con las pila se mueve mucho menos
  caminarDelante3(); // da vueltas, debo ajustar las patas
  
  //int distancia = activarRadar();
  // obtengo distancia con la clase Radar instancia sonar
  int distancia = sonar.getDistProm_cmMl();
  Serial.println(String(distancia));
  mySerial.println(String(distancia));
  // validamos distancia
  if (distancia < 10){
    // primero retrocedemos
    int intervalo = 16;
    retroceder2(intervalo); // intervalo es la cantidad de iteraciones
    girar(distancia);
  }
  // validamos si la distancia es la misma al anterior
  if (distancia == dist_ant and distancia >= 10){
    // se trabo con un obstaculo, probamos de levantar la pata para superar obstaculo
    levantarPata(pataDelD);
    levantarPata(pataDelI);
  }
  dist_ant = distancia;
}
void avanzar(){ // muy abrupto los motores y no camina sino que se que pataleando y girando
  pataDelD.write(120);
  pataTrasD.write(120);
  pataDelI.write(120);
  pataTrasI.write(120);
  delay(300);
  pataDelD.write(60);
  pataTrasD.write(60);
  pataDelI.write(60);
  pataTrasI.write(60);
  delay(300);
  pataDelD.write(120);
  pataTrasD.write(120);
  pataDelI.write(120);
  pataTrasI.write(120);
  delay(300);
  pataDelD.write(60);
  pataTrasD.write(60);
  pataDelI.write(60);
  pataTrasI.write(60);
  delay(300);
}
void caminarDelante(){ // caminar intercalando patas delanteras y traseras
  pataDelD.write(110);
  pataDelI.write(110);
  delay(400);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
  pataDelD.write(80);
  pataDelI.write(80);
  delay(400);
  pataTrasD.write(80);  
  pataTrasI.write(80);
  delay(400);
  pataDelD.write(110);
  pataDelI.write(110);
  delay(400);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
  pataDelD.write(80);
  pataDelI.write(80);
  delay(400);
  pataTrasD.write(80);  
  pataTrasI.write(80);
  delay(400);
}
void caminarDelante1(){ // intercalando por patas derechas e izquierdas camina mas o menos bien pero necesita mejoras
  pataDelD.write(110);
  pataTrasD.write(80);
  delay(400);
  pataDelI.write(110);  
  pataTrasI.write(80);
  delay(400);
  pataDelD.write(80);
  pataTrasD.write(110);
  delay(400);
  pataDelI.write(80);  
  pataTrasI.write(110);
  delay(400);
  pataDelD.write(110);
  pataTrasD.write(80);
  delay(400);
  pataDelI.write(110);  
  pataTrasI.write(80);
  delay(400);
  pataDelD.write(80);
  pataTrasD.write(110);
  delay(400);
  pataDelI.write(80);  
  pataTrasI.write(110);
  delay(400);
}
void caminarDelante2(){ // camina para adelante mas rapido pero se ladea de costado
  pataDelD.write(110);
  pataDelI.write(110);
  pataTrasD.write(70);  
  pataTrasI.write(70);
  delay(400);
  pataDelD.write(70);
  pataDelI.write(70);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
  pataDelD.write(110);
  pataDelI.write(110);
  pataTrasD.write(70);  
  pataTrasI.write(70);
  delay(400);
  pataDelD.write(70);
  pataDelI.write(70);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
}
void caminarDelante3(){ // camina para adelante mas rapido pero se ladea de costado
  pataDelD.write(110);
  pataDelI.write(110);
  pataTrasD.write(60);  
  pataTrasI.write(60);
  delay(400);
  pataDelD.write(60);
  pataDelI.write(60);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
  pataDelD.write(110);
  pataDelI.write(110);
  pataTrasD.write(60);  
  pataTrasI.write(60);
  delay(400);
  pataDelD.write(60);
  pataDelI.write(60);
  pataTrasD.write(110);  
  pataTrasI.write(110);
  delay(400);
}

// retroceder
void avanzarMasRapido(int intervalo){ // no retrocede vamos a tener que hacer cambios radicales
  detener();
  for(int i=0; i < intervalo; i++){ // las patas estan invertidas, las delanteras son traseras OJO!!!
    pataDelD.write(60);
    pataDelI.write(60);
    pataTrasD.write(100);  
    pataTrasI.write(100);
    delay(400);
    pataDelD.write(110);
    pataDelI.write(110);
    pataTrasD.write(80);  
    pataTrasI.write(80);
    delay(400);
    pataDelD.write(60);
    pataDelI.write(60);
    pataTrasD.write(100);  
    pataTrasI.write(100);
    delay(400);
    pataDelD.write(110);
    pataDelI.write(110);
    pataTrasD.write(80);  
    pataTrasI.write(80);
    delay(400);
  }
  detener();  
}
void retroceder2(int intervalo){ // no retrocede vamos a tener que hacer cambios radicales
  detener();
  for(int i=0; i < intervalo; i++){ // las patas estan invertidas, las delanteras son traseras OJO!!!
    pataDelD.write(60);
    pataDelI.write(60);
    //pataTrasD.write(100);  
    //pataTrasI.write(100);
    delay(400);
    pataDelD.write(110);
    pataDelI.write(110);
    //pataTrasD.write(80);  
    //pataTrasI.write(80);
    delay(400);
    pataDelD.write(60);
    pataDelI.write(60);
    //pataTrasD.write(100);  
    //pataTrasI.write(100);
    delay(400);
    pataDelD.write(110);
    pataDelI.write(110);
    //pataTrasD.write(80);  
    //pataTrasI.write(80);
    delay(400);
  }
  detener();  
}

void detener(){
  // pone todos los motores en 90 grados
  pataDelD.write(90);
  pataTrasD.write(90);
  pataDelI.write(90);
  pataTrasI.write(90);
  delay(100);
}
// girar
void girar(int distancia){ // gira en forma random
  int rand;
  randomSeed(distancia);
  rand = random(0,9);
  if (rand > 4)
    girarDerecha(10);
  else 
    girarIzquierda(10);
  
}
// girar derecha
void girarDerecha(int iter){  
  for (int i=0; i < iter; i++){
    // detenemos patas derechas  
    pataDelD.write(90);
    pataTrasD.write(90);
    //activamos patas izquierdas para girar derecha
    pataDelI.write(110);
    pataTrasI.write(80);
    delay(400);
    pataDelI.write(80);
    pataTrasI.write(110);
    delay(400);
    pataDelI.write(110);
    pataTrasI.write(80);
    delay(400);
    pataDelI.write(80);
    pataTrasI.write(110);
    delay(400);
  }  
  detener();
}
// girar izquierda
void girarIzquierda(int iter){
  for (int i=0; i < iter; i++){
    // detenemos patas izquierda
    pataDelI.write(90);
    pataTrasI.write(90);
    //activamos patas izquierdas para girar derecha
    pataDelD.write(110);
    pataTrasD.write(80);
    delay(400);
    pataDelD.write(80);
    pataTrasD.write(110);
    delay(400);
    pataDelD.write(110);
    pataTrasD.write(80);
    delay(400);
    pataDelD.write(80);
    pataTrasD.write(110);
    delay(400);
  }  
  detener();  
}
// funciones relacionadas con trepar
void levantarPata(Servo servo){
  servo.write(180);
  delay(400);
}
void levantarPata(Servo servo, int angulo){
  servo.write(angulo);
  delay(400);
}
// uso del radar
int activarRadar(){ // funciona OK!
  unsigned int tiempo;
  delayMicroseconds(2);
  // Activamos el ultrasonido
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  // Obtenemos el tiempo que tarda el sonido en ir y volver
  tiempo = pulseIn(ECHO,HIGH);
  int distancia = tiempo/58; // Dividimos por 58, porque la señal de ultrasonido tarda 29,4 microsegundos en recorrer 1 cm ida (se redondea a 29), por eso multiplicamos x 2.
  // validamos que la distancia sea valida
  if (distancia > 500){
    // distancia invalida
    // vuelvo a medir distancia
    return activarRadar(); 
  }
  return distancia;
}
// parseo de bluetooth
// Parseo de String que llega de BT
int parsearBTString(String valorStr){
  if (valorStr.indexOf("0#") > -1){
    return 0; 
  }
  if (valorStr.indexOf("1#") > -1){
    return 1; 
  }
  if (valorStr.indexOf("2#") > -1){
    return 2; 
  }
  if (valorStr.indexOf("3#") > -1){
    return 3; 
  }
  if (valorStr.indexOf("4#") > -1){
    return 4; 
  }
  if (valorStr.indexOf("5#") > -1){
    return 5; 
  }
  if (valorStr.indexOf("6#") > -1){
    return 6; 
  }
  if (valorStr.indexOf("7#") > -1){
    return 7; 
  }
  if (valorStr.indexOf("8#") > -1){
    return 8; 
  }
  if (valorStr.indexOf("9#") > -1){
    return 9; 
  }
  return -1;
}

int parsearBTString2(String valorStr){
  if (valorStr == "*12|99|99|0#"){
    return 0; 
  }
  if (valorStr == "*12|99|99|1#"){
    return 1; 
  }
  if (valorStr == "*12|99|99|2#"){
    return 2; 
  }
  if (valorStr == "*12|99|99|3#"){
    return 3; 
  }
  if (valorStr == "*12|99|99|4#"){
    return 4; 
  }
  if (valorStr == "*12|99|99|5#"){
    return 5; 
  }
  if (valorStr == "*12|99|99|6#"){
    return 6; 
  }
  if (valorStr == "*12|99|99|7#"){
    return 7; 
  }
  if (valorStr == "*12|99|99|8#"){
    return 8; 
  }
  if (valorStr == "*12|99|99|9#"){
    return 9; 
  }
  return -1;
}
