 int leftPinSpeed = 9; // pin od predkości lewego silnija
 int rightPinSpeed = 10; // pin od predkości prawego silnika
 int rightPinDirection = 8; // tu musisz zmienić bo na moim shieldzie mostek H to 0 lub 1 (przód tył)
 int leftPinDirection = 7; // to samo

String readedString; // zmienna pobierająca linijke z portu szeregowego

char lastInput; // zmienna przechwywująca ostatni kierunek przesłany przez bluetooth

int leftValue; // zmienna przechowywująca wartość predkości lewego silnika
int rightValue; // to samo tylko prawego

bool leftForward = true; // do przodu czy do tyły lewy silnik
bool rightForward = true; // to samo tylko prawy

void setup() {
  //ustawianie pinów
  pinMode(leftPinSpeed, OUTPUT); 
  pinMode(rightPinSpeed, OUTPUT);
  pinMode(rightPinDirection, OUTPUT);
  pinMode(leftPinDirection, OUTPUT);
  
  //ustawianie początkowo, aby silniki poruszały się do przodu
  digitalWrite(rightPinDirection, LOW);
  digitalWrite(leftPinDirection, LOW);
  
  //rozpoczęcie nasłuchiwania na szerego 9600bit
  Serial.begin(9600);
}

void loop() {
  //pętla rozpoczyna się gdy wykryje dane na porcie szeregowym
  while(Serial.available()){
    if(Serial.available() > 0){
      //jeśli tak to zczytaj znaki po kolei 
      char c = Serial.read();
      if(c == '\n'){ // jeśli znak to "enter" rozpocznij funkcje interpetującą
        interpreter();
        break;
      }
      readedString += c;
    }
  }

  //pwm setting
  analogWrite(leftPinSpeed, leftValue);
  analogWrite(rightPinSpeed, rightValue);
}

void interpreter() { 
  // przykładowy string zczytany z portu (r 133)
  
  if(readedString[0] == 'l'){ // jeżeli na początku stringa jest `l` oznacza to, że poruszamy silnik lewy
    
    lastInput = readedString[0]; // przypisauje ostatni wczytany input 
    String number; //string zawierający wartość "PWM"
    
    if(readedString[2] == '-'){ // jeżeli wartość jest ujemna odwróć kierunek silnika
      if(leftForward == true){
        digitalWrite(leftPinDirection, HIGH);
        leftForward = false;
      }
      for(int i = 3; i<readedString.length(); i++){ // zczytywanie wartości liczbowej z zapisanego stringa z portu szeregowego
      number += (String)readedString[i]; 
    }
    }else if(readedString[2] != '-' ){ // warunek przeciwny dla tego jeżeli wartość jest dodatnia
      if(leftForward != true){
        digitalWrite(leftPinDirection, LOW);
        leftForward = true;
      }
      for(int i = 1; i<readedString.length(); i++){ // to samo co na górze
      number += (String)readedString[i];  
      }
    }
    
    leftValue = atoi(number.c_str()); // funkcja atoi() zmienia ciąg "string" najpierw poprzez metode .c_str() na tablicę "char", a potem na liczbę całkowitą int 
  } else if(readedString[0] == 'r'){ // analogicznie to samo co na górze
    
    lastInput = readedString[0];
    String number;
  
    if(readedString[2] == '-'){
      if(rightForward == true){
        digitalWrite(rightPinDirection, HIGH);
        rightForward = false;
      }
      for(int i = 3; i<readedString.length(); i++){
      number += (String)readedString[i];
    }
    }else if(readedString[2] != '-'){
      if(rightForward != true){
        digitalWrite(rightPinDirection, LOW);
        rightForward = true;
      }
      for(int i = 1; i<readedString.length(); i++){
      number += (String)readedString[i];
    }
    }
    
    rightValue = atoi(number.c_str());
  }
  debug();
  //Serial.println(readedString);
  readedString = "";
}

void debug(){
  Serial.print("Input: ");
  Serial.println(lastInput);
  Serial.print("Value: ");
  if(lastInput == 'r'){
    Serial.println(rightValue);
  }else if(lastInput == 'l'){
    Serial.println(leftValue);
  }
}



