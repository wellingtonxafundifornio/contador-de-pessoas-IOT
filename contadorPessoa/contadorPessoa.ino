
/*
 Inclusão da biblioteca de WI-FI e do MQTT
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>



/*
 Configuração do Wi-FI  
 */
const char* SSID = "Casa"; //Coloque o nome do seu WI-FI
const char* PASSWORD = "casa123654"; //Coloque a senha do seu WI-FI

/*
 Configuração do mqtt
 */
const char* MQTT_SERVER =  "192.168.30.20"; //Coloque o ip do servidor MQTT
int MQTT_PORT =        1883; //Coloque a portal do seu MQTT
#define MQTT_USER      "teste" //Coloque o usuário do MQTT
#define MQTT_PASSWORD  "123456" //Coloque a senha do usuário do MQTT
#define MQTT_CLIENT    "ESP8266-Client" //Coloque o Client do MQTT

//Tópicos do mqtt
#define MQTT_TOPIC_PUBLISH_ENTRADA "contadorDePessoas/entrada"
#define MQTT_TOPIC_PUBLISH_SAIDA   "contadorDePessoas/saida"

/*
 mapeamento de pinos do NodeMCU  
 */
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

//Led embutido no Sensor de presença de luminozidade, que monitora a entrada, que esta ligado à entrada do pino D0(16) do NodeMCU
const int ledSensorPresencaEntrada = D0;
//Led embutido no Sensor de presença de luminozidade, que monitora a saida, que esta ligado à entrada do pino D5(14) do NodeMCU
const int ledSensorPresencaSaida = D5;

// Led ou lazer que emite a luz de entrada. Localizado na placa NodeMcu, pino D7(13)
const int emissorLuzEntrada = D7;
// Led ou lazer que emite a luz de saida. Localizado na placa NodeMcu, pino D4(2)
const int emissorLuzSaida = D4;

int contEntrada = 0;
int contSaida = 0;
int quantidadePessoasSala = 0;

int sensorEntrada = D1; //Pino analógico(D1)5, onde sensor de entrada está conectado.
int sensorSaida = D3; //Pino analógico(D3)0, onde sensor de entrada está conectado.

int lerValorSensorEntrada=0;
int lerValorSensorSaida = 0;

/* 
Essa variavel tem o objetivo de NÃO PERMITIR que o contador 
CONTE varias vezes caso alguem fique parado 
entre a LUZ e o SENSOR de ENTRADA. Em outras palavras, 
é verificado se tem "alguem Parado No Sensor De Entrada Ou Passando"
*/

boolean luminozidadeSensorEntrada = false;
/* 
Essa variavel tem o objetivo de NÃO PERMITIR que o contador 
CONTE varias vezes caso alguem fique parado 
entre a LUZ e o SENSOR de SAIDA. Em outras palavras, 
é verificado se tem "alguem Parado No Sensor De SAIDA Ou Passando"
*/
boolean luminozidadeSensorSaida = false;

boolean acessoEntrada =false;
boolean acessoSaida =false;

WiFiClient espClient;
PubSubClient MQTT(espClient);


/*
prototypes
*/
void initMQTT();
void reconnectMQTT();
void reconnectWiFi();
void callback(char* topic, byte* payload, unsigned int length);
boolean entrada();
boolean saida();

/*
Implementação das funções/procedures
*/
void initMQTT() {
  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.setCallback(callback);
}


// Função para conexão
void reconnectMQTT() {
    while (!MQTT.connected()) {
      Serial.println("Tentado conectar ao broker MQTT: " + String(MQTT_SERVER) + ":" + String(MQTT_PORT));
      Serial.println("MQTT_CLIENT: -->"+ String(MQTT_CLIENT) +"- ..." + " MQTT_USER:  -->"+ String(MQTT_USER) + "-..." + " MQTT_PASSWORD: -->" + String(MQTT_PASSWORD)); 
      if (MQTT.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) { 
        Serial.println("conectado!!!");
        MQTT.publish("Teste", "Conectado ao MQTT (:");
      } 
      else {
        Serial.print("falha, rc=");
        Serial.print(MQTT.state());
        Serial.println(" tenta novamente após 5 segundos");
        delay(5000);
      }
    }
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
     return;
     
  Serial.println("Conectando-se na rede: " + String(SSID));
  Serial.println("Aguarde");
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for(int i = 0; i < length; i ++){
    char c = (char)payload[i];
    msg += c;
  }
  
  Serial.print(topic);
  Serial.print(" -- >");
  Serial.println(msg);
}

/*
Valores da luminosidade podem ser alterados conforme necessidade.
verifica se luz de entrada foi interrompida
*/
boolean entrada(){
  return lerValorSensorEntrada < 150;
}

/*
verifica se luz de saida foi interrompida
Valores da luminosidade podem ser alterados conforme necessidade.
*/
boolean saida(){
  return lerValorSensorSaida < 150;
}


void setup() {

  //Ativando o serial monitor que exibirá os valores lidos no sensor.
  Serial.begin(9600);

  //Definindo pinos digitais dos leds como de saída.
  pinMode (emissorLuzSaida, OUTPUT);
  pinMode (emissorLuzEntrada, OUTPUT);

  pinMode(ledSensorPresencaEntrada, OUTPUT);
  pinMode(ledSensorPresencaSaida, OUTPUT);

  reconnectWiFi();
  initMQTT();
  reconnectMQTT();
}

void loop() {
  
  //Para que os emissores de luz de ENTRADA e luz de SAIDA fiquem sempre ligados
  digitalWrite (emissorLuzSaida, HIGH);
  digitalWrite (emissorLuzEntrada, HIGH);
  
  //Lê a intensidade de luz de entrada
  lerValorSensorEntrada = analogRead(sensorEntrada);

  //Lê a intensidade da luz de saida
  lerValorSensorSaida = analogRead(sensorSaida);
  
  
   if(entrada()) {

    //Somente para indicar que tem alguem entrando
    digitalWrite (ledSensorPresencaEntrada, HIGH);

    if(luminozidadeSensorEntrada){
       contEntrada++;
       luminozidadeSensorEntrada=false;
       if(acessoSaida){
          MQTT.publish(MQTT_TOPIC_PUBLISH_SAIDA, "saida: " );
          acessoSaida=false;
       }else{
          acessoEntrada = true;
       } 
    }

     delay(220);
  }
  
  if (saida()) {
    
     digitalWrite(ledSensorPresencaSaida, HIGH); 

     if(luminozidadeSensorSaida){
     
        luminozidadeSensorSaida=false;
        contSaida++;
        if(acessoEntrada){
           MQTT.publish(MQTT_TOPIC_PUBLISH_ENTRADA, "entrada: ");
           acessoEntrada=false;
        }else{
        
           acessoSaida=true;
        }
     }
     
     delay(220);
  }

  //Quantidade atualizada de Pessoas, usada mais para fins de log no arduino para ver se está tudo certo.
  quantidadePessoasSala = contEntrada - contSaida;

  //Luminosidade alta! Não tem ninguem interrompendo a luz de ENTRADA.
  if (!entrada()) {

   digitalWrite(ledSensorPresencaEntrada, LOW);
   luminozidadeSensorEntrada=true;
   acessoEntrada=false;
  }

  //Luminosidade alta! Não tem ninguem interrompendo a luz de SAIDA.
  if (!saida()) {
    
    digitalWrite(ledSensorPresencaSaida, LOW);
    luminozidadeSensorSaida=true;
    acessoSaida=false;
  }

  reconnectWiFi();
  reconnectMQTT();
  MQTT.loop();
  
}

