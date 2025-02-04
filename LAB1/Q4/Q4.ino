#define RED 5
#define GREEN 4
#define BLUE 12

char array1[10]={'R','1','2','G','R','G','B','G','D','C'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int arraysize = sizeof(array1)/sizeof(char);
  int red_counter= 0;
  int green_counter=0;
  int blue_counter=0;

  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

  for(int i = 0; i<arraysize; i++){
    if(array1[i]=='R'){
      digitalWrite(RED,HIGH);
      delay(300);
      digitalWrite(RED,LOW);
      red_counter++;
    } else if (array1[i]=='G'){
        digitalWrite(GREEN,HIGH);
        delay(300);
        digitalWrite(GREEN,LOW);
        green_counter++;
    } else if(array1[i]=='B'){
        digitalWrite(BLUE,HIGH);
        delay(300);
        digitalWrite(BLUE,LOW);
        blue_counter++;
    } else {
      delay(150);
    }

  }
  Serial.print("The number of R:");
  Serial.println(red_counter);
  Serial.print("The number of G:");
  Serial.println(green_counter);
  Serial.print("The number of B:");
  Serial.println(blue_counter);

  


}

void loop() {
  // put your main code here, to run repeatedly:

}
