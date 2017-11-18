# Lab Iniciação Cientifica - Contador de pessoas

Experimento de criação de uma `aplicação que contada o fluxo de pessoas em um ambiente`, usando a placa `NodeMCU ESP8266` e o software `Aduino IDE` para programa-la. 

## Dentro da pasta

`...documentacao\` Tem imagens da `arquitetura do projeto`, `sensor` e a`placa usada`.

## Pré-requisitos para subir este projeto
 * Placa `NodeMCU ESP8266 e sensores`. Para mais detalhes, olhe a imgem "arquiteturaDaAplicacao.png", dentro da pasta "documentação";

 * Ter a IDE do Arduino instalada;

 * Adicionar as seguintes bibliotecas á IDE do Arduino:
    * `#include <ESP8266WiFi.h>`
    * `#include <PubSubClient.h>`

* Ter o `MQTT Mosquitto` instalado! Segue é o link que usei para instalar e  configurar o MQTT no Windows: https://www.youtube.com/watch?v=Y7NE-TpSwUw 

* Dica para subir e testar o MQTT após a instalação
    * Abra o terminal (CMD, PRONPT, SHELL) como `Administrador` e entre no diretório do MQTT `C:\Program Files (x86)\mosquitto>`;
    * Suba um subscrib para escutar tudo que chega no MQTT `mosquitto_sub -h COLOQUE_SEU_IP -u NOMEI_UM_USUARIO -P INFORME_UMA_SENHA -t "#"`;
    * Suba um publish para publicar dados no MQTT `mosquitto_pub -h COLOQUE_SEU_IP -u USUARIO_NOMEADO -P SENHA_INFORMADA -t "DIGITE_UM_TOPICO" -m "Informe a mensagem a ser enviada. Hello World"` .

* Montar o hardware conforme a imagem em `...documentacao\arquiteturaDaAplicacao.png`

### Dica sobre possiveis problemas (não conectar) com a rede WI-FI ao subir a aplicação:
 * verifique se o FIREWALL estar desligado;
 * Nas propriedades da rede no tópico `deixar o computado detectavel` deve estar como `ATIVADO`;
 * Verifique no codigo `ContadorPessoas.ino` em `Configuração do Wi-FI` se a configuração está compativel com o seu WI-FI;
 * Verifique no codigo `ContadorPessoas.ino` em `Configuração do mqtt` se a configuração do MQTT está preenchida de maneira correta (ip, porta, etc).
 

### Com os Pré-requisitos atendidos

Popule as variaveis de `configuração do WI-FI` com os dados do seu WI-FI e popular as variaveis de `Configuração do mqtt` os dados do MQTT que você instalou, com isso feito, basta fazer o upload do codigo (contadorPessoa.ino) para a placa NodeMCU via Arduino IDE.
 
 
### Para este projeto também foi desenvolvida uma aplicação Web Responsiva para apresentar os dados coletados do MQTT.  

link aqui...

### Fazendo um breve resumo sobre a aplicação

A ContadorPessoa verifica que alguem adentrou no ambiente monitorado e dispara um `publish` (publica/envia msg) para o MQTT com topico `contadorDePessoas/entrada`. E ao mesmo tempo que o MQTT recebe o `publish`, uma aplicação Web que está escutando tudo que chega no MQTT faz o `subscribe`(lê) da informação publicada (publish) pela aplicação IOT `ContadorPessoa.ino`.
