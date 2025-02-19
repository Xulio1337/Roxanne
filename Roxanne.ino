/*O código controla a velocidade de um motor DC utilizando um Arduino, 
botões para aumentar e diminuir a velocidade, e um display de 7 segmentos para exibir a porcentagem da potência do motor. 
O motor pode ser ligado e desligado com um botão, e a velocidade é ajustada por PWM.*/

#define ch 8
#define up 5
#define down 6
#define speed 9
#define motorP 10

int chStt = 0;
int chPrevStt = 0;
int upStt = 0;
int upPrevStt = 0;
int downStt = 0;
int downPrevStt = 0;
bool motorOn = 0;
int freq = 10;
int rpm = 0;

bool motorLigado = false;  // Nova variável para rastrear o estado do motor

int pinos_dos_segmentos[4] = {2, 3, 4, 7};
int pino_comum_do_display[3] = {11, 12, 13};
int num = 0;
int digitos[11][4] = {
    {0, 0, 0, 0}, // 0
    {0, 0, 0, 1}, // 1
    {0, 0, 1, 0}, // 2
    {0, 0, 1, 1}, // 3
    {0, 1, 0, 0}, // 4
    {0, 1, 0, 1}, // 5
    {0, 1, 1, 0}, // 6
    {0, 1, 1, 1}, // 7
    {1, 0, 0, 0}, // 8
    {1, 0, 0, 1}, // 9
    {0, 0, 0, 0}  // -
};

void setup()
{
  pinMode(ch, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(speed, OUTPUT);
  pinMode(motorP, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(pinos_dos_segmentos[i], OUTPUT);
  }

  for (int i = 0; i < 3; i++) {
    pinMode(pino_comum_do_display[i], OUTPUT);
  }

  Serial.begin(9600);
}

void displayNumber(int number)
{
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinos_dos_segmentos[i], digitos[number][i]);
  }
}

void loop()
{
  chStt = digitalRead(ch);
  upStt = digitalRead(up);
  downStt = digitalRead(down);
  rpm = map(freq, 0, 255, 0, 9393);

  if(chStt == 1 && chPrevStt == 0){
    // Alterna o estado do motor quando o interruptor é alterado (Toggle)
    motorLigado = !motorLigado;
    digitalWrite(motorP, motorLigado);
  }
  else if(upStt == 1 && upPrevStt == 0 && freq < 255){
    freq += 10;
  }
  else if(downStt == 1 && downPrevStt == 0 && freq > 0) {
    freq -= 10;
  }

  upPrevStt = upStt;
  downPrevStt = downStt;
  chPrevStt = chStt; 
  analogWrite(speed, freq);

  // Atualizar os displays de sete segmentos com a porcentagem do motor
  int percentage = map(freq, 0, 255, 0, 999);
  displayNumber(percentage / 100);  // Hundreds
  digitalWrite(pino_comum_do_display[0], HIGH);
  delay(1);
  digitalWrite(pino_comum_do_display[0], LOW);

  displayNumber((percentage % 100) / 10);  // Tens
  digitalWrite(pino_comum_do_display[1], HIGH);
  delay(1);
  digitalWrite(pino_comum_do_display[1], LOW);

  displayNumber(percentage % 10);  // Units
  digitalWrite(pino_comum_do_display[2], HIGH);
  delay(1);
  digitalWrite(pino_comum_do_display[2], LOW);

  delay(100);
}
