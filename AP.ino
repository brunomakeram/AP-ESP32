// Importa com as ferramentas da biblioteca...
#include <WiFi.h>

// Biblioteca com as funções nativas do ESP-IDF...
#include "esp_wifi.h"

// Ponteiros para nome e senha da rede...
const char *nome_rede = "ESP32-WIFI";
const char *senha_rede = "45928866";
// Identifica o dispositivo na rede(host)...
const char *nome_dispositivo = "esp32s3";

// Define o endereço IP local...
const IPAddress ip_Local(192,168,10,1);
// Define o endereço IP do gateway...
const IPAddress ip_gateway(192,168,10,1);
// Define a máscara de sub-rede...
const IPAddress sub_rede(255,255,255,0);

// Verifica se a rede foi ativada ou não...
bool rede_ativa = false;
// Limita o número de usuários conectados simultaneamente...
int limite_conexoes = 4;

// Estrutura para devolver a quantidade de dispositivos conectados...
wifi_sta_list_t quantidade_dispositivos;

void setup() {
  Serial.begin(115200);
  
  // Função para definir o nome do host do ponto de acesso (AP)...
  WiFi.softAPsetHostname(nome_dispositivo);
  // Função para configurar o IP como estático (fixo), gateway e a máscara de sub-rede...
  WiFi.softAPConfig(ip_Local, ip_gateway, sub_rede);
  
  delay(100);
  // Inicia o Wi-Fi como um ponto de acesso...
  rede_ativa = WiFi.softAP(nome_rede, senha_rede, 1, 0, limite_conexoes);
  delay(100);
  
  if (rede_ativa) {
    // O especificador %s indica onde a string da variável 'nome_dispositivo' será inserida...
    Serial.printf("Rede ativada com sucesso: %s\n\n", nome_rede);
 
    Serial.printf("Nome do Host: %s\n\n", nome_dispositivo);
  }
  // Tempo de espera para conectar um aparelho no Wi-Fi antes da leitura única...
  delay(10000);
}

// Armazena a quantidade da última leitura para comparação...
int quantidade_anterior = -1;

void loop() {
  // A função oficial da documentação que preenche a estrutura com os dispositivos...
  if (esp_wifi_ap_get_sta_list(&quantidade_dispositivos) == ESP_OK) {
    
    // Só imprime na Serial se o número de conexões tiver mudado...
    // O "!=" lê-se: "Se a quantidade de dispositivos for DIFERENTE da quantidade anterior"...
    if (quantidade_dispositivos.num != quantidade_anterior) {
      quantidade_anterior = quantidade_dispositivos.num;

      Serial.printf("Quantidade de dispositivos: %d\n\n", quantidade_dispositivos.num);

      // Varre a lista de dispositivos presentes na estrutura...
      for (int indice = 0; indice < quantidade_dispositivos.num; indice++) {
        wifi_sta_info_t informacao_estacao = quantidade_dispositivos.sta[indice];

        // Imprime o endereço MAC formatado em hexadecimal...
        Serial.printf("Conexões: %d - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n\n", 
          indice + 1,
          informacao_estacao.mac[0],
          informacao_estacao.mac[1],
          informacao_estacao.mac[2],
          informacao_estacao.mac[3],
          informacao_estacao.mac[4],
          informacao_estacao.mac[5]
        );
      }
      // Créditos...
      Serial.printf("===...Desenvolvido por: Bruno Maker - 2026...===\n\n");
    }
  } else {
    // Feedback de erro...
    Serial.println("Erro ao recuperar a lista de pontos de acesso!");
  }

  // Pausa necessária para alimentar o Watchdog e não travar a CPU...
  delay(1000);
}