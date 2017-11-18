# Lab Inicia��o Cientifica - Contador de pessoas

Experimento de cria��o de uma `aplica��o que contada o fluxo de pessoas em um ambiente`, usando a placa `NodeMCU ESP8266` e o software `Aduino IDE` para programa-la. 

## Dentro da pasta

`...documentacao\` Tem imagens da `arquitetura do projeto`, `sensor` e a`placa usada`.

## Pr�-requisitos para subir este projeto
..* Placa `NodeMCU ESP8266 e sensores`. Para mais detalhes, olhe a imgem "arquiteturaDaAplicacao.png", dentro da pasta "documenta��o";

..* Ter a IDE do Arduino instalada;

..* Adicionar as seguintes bibliotecas � IDE do Arduino:
....* `#include <ESP8266WiFi.h>`
....* `#include <PubSubClient.h>`

..* Ter o `MQTT Mosquitto` instalado! Segue � o link que usei para instalar e  configurar
o MQTT no Windows: https://www.youtube.com/watch?v=Y7NE-TpSwUw 

..* Dica para subir e testar o MQTT ap�s a instala��o
....* Abra o terminal (CMD, PRONPT, SHELL) como `Administrador` e entre no diret�rio do MQTT `C:\Program Files (x86)\mosquitto>`;
....* Suba um subscrib para escutar tudo que chega no MQTT `mosquitto_sub -h COLOQUE_SEU_IP -u NOMEI_UM_USUARIO -P INFORME_UMA_SENHA -t "#"`;
....* Suba um publish para publicar dados no MQTT `mosquitto_pub -h COLOQUE_SEU_IP -u USUARIO_NOMEADO -P SENHA_INFORMADA -t "DIGITE_UM_TOPICO" -m "Informe a mensagem a ser enviada. Hello World"` .

..* Montar o hardware conforme a imagem em `...documentacao\arquiteturaDaAplicacao.png`

### Com os Pr�-requisitos atendidos, basta apenas fazer o upload do codigo (contadorPessoa.ino) para a placa NodeMCU via Arduino IDE.
 
 
### Acesse o link para baixar e executar a API Java


### Para este projeto tamb�m foi desenvolvida uma aplica��o Web Responsiva para apresentar os dados coletados do MQTT.  

link aqui...

### Fazendo um breve resumo sobre a aplica��o

A ContadorPessoa verifica que alguem adentrou no ambiente monitorado e dispara um `publish` (publica/envia msg) para o MQTT com topico `contadorDePessoas/entrada`. E ao mesmo tempo que o MQTT recebe o `publish`, uma aplica��o Web que est� escutando tudo que chega no MQTT faz o `subscribe`(l�) da informa��o publicada (publish) pela aplica��o IOT `ContadorPessoa.ino`.