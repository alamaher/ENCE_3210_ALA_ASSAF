#define RED 5
#define GREEN 4
char test1[]={'1','2','3','4','1','3','1','1','1','2','3','1','2','4','1','2','3'}; // pattern is '1','2','3'
char Stored_array[] = {'1','2','3'};
int detection_times (char arr[], char chosen_pattern[], int size){
  int counter = 0;
  for ( int i = 0; i<size-1; i++){
    if( arr[i-1]==chosen_pattern[0] && arr[i]==chosen_pattern[1] && arr[i+1]==chosen_pattern[2]){
      digitalWrite(GREEN,HIGH);
      counter++;
    }
  }
  if (counter == 0){
    digitalWrite(RED,HIGH);
  }
  return counter;
}



void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    int arraysize = sizeof(test1)/sizeof(char);
    pinMode(RED,OUTPUT);
    pinMode(GREEN,OUTPUT);
    digitalWrite(RED,LOW);
    digitalWrite(GREEN,LOW);
    int returned_count = detection_times(test1, Stored_array,arraysize);
    Serial.println(returned_count);

}

void loop() {
  // put your main code here, to run repeatedly:

}
