#include <FlowMeter.h>  // https://github.com/sekdiy/FlowMeter

//conectar o pino de dados do sensor fluxometro em um pino capaz de gerar interrupção
//capacidade máxima de vazão do sensor: 30 L/min
//fator k = 5,9375
FlowSensorProperties MySensor = {30.0f, 5.9375f, {0.9933774834, 0.8474576271, 0.8522727273, 0.8484162896, 0.8426966292, 0.8522727273, 0.7894736842, 0.7894736842, 0.7894736842, 0.7894736842}};
FlowMeter Meter5 = FlowMeter(5, MySensor);
FlowMeter Meter4 = FlowMeter(4, MySensor);
FlowMeter Meter0 = FlowMeter(0, MySensor);

// realiza as medidas a cada 1 segundo (250 ms)
const unsigned long periodo5 = 250;
// realiza as medidas a cada 1 segundo (250 ms)
const unsigned long periodo4 = 250;
// realiza as medidas a cada 1 segundo (250 ms)
const unsigned long periodo0 = 250;
//inicializa a flag de medida desativada
bool medida5 = false;
//inicializa a flag de medida desativada
bool medida4 = false;
//inicializa a flag de medida desativada
bool medida0 = false;
//inicializa a flag de medida pronta ativada
bool medida_terminada5 = true;
//inicializa a flag de medida pronta ativada
bool medida_terminada4 = true;
//inicializa a flag de medida pronta ativada
bool medida_terminada0 = true;
//inicializa o buffer da diferença da medida do litro anterior com o atual em zero
float diferenca_litro5 = 0.0;
//inicializa o buffer da diferença da medida do litro anterior com o atual em zero
float diferenca_litro4 = 0.0;
//inicializa o buffer da diferença da medida do litro anterior com o atual em zero
float diferenca_litro0 = 0.0;
//inicializa o contador de medidas em 1 
unsigned long contador5=1;
//inicializa o contador de medidas em 1 
unsigned long contador4=1;
//inicializa o contador de medidas em 1 
unsigned long contador0=1;

void ICACHE_RAM_ATTR MeterISR5(); 
void ICACHE_RAM_ATTR MeterISR4(); 
void ICACHE_RAM_ATTR MeterISR0(); 

void setup() 
{
  //comunicação serial
  Serial.begin(115200);

  //Habilita a interrupção pelo pino 5 como borda de subida e vincula a rotina de interrupção chamada MeterISR
  attachInterrupt(5, MeterISR5, RISING);
  attachInterrupt(4, MeterISR4, RISING);
  attachInterrupt(0, MeterISR0, RISING);

  //As vezes energizar o sensor pode gerar pulsos aleatórios que devemos ignorar
  //reseta o contador de pulsos
  Meter5.reset();
  Meter4.reset();
  Meter0.reset();
}

void loop() 
{
  //período de atualização
  delay(periodo5);
  //período de atualização
  delay(periodo4);  
  //período de atualização
  delay(periodo0);
  //processa a quantidade contada de pulsos
  Meter5.tick(periodo5);
  Meter4.tick(periodo4);
  Meter0.tick(periodo0);

  //armazena a quantidade total de litros passados numa variável auxiliar
  float auxiliar5 = (Meter5.getTotalVolume());
  float auxiliar4 = (Meter4.getTotalVolume());
  float auxiliar0 = (Meter0.getTotalVolume());

  //mostra o resultado encontrado (fluxo e qnt total de litros)
  Serial.println("|Fluxo atual 5: " + String(Meter5.getCurrentFlowrate()) + " litros/minuto ||" + " total de litros: "+ String(auxiliar5)+"|");
  Serial.println("                                                                                                                          ");
  Serial.println("|Fluxo atual 4: " + String(Meter4.getCurrentFlowrate()) + " litros/minuto ||" + " total de litros: "+ String(auxiliar4)+"|");
  Serial.println("                                                                                                                          ");
  Serial.println("|Fluxo atual 0: " + String(Meter0.getCurrentFlowrate()) + " litros/minuto ||" + " total de litros: "+ String(auxiliar0)+"|");
  Serial.println("                                                                                                                          ");


  //se o ciclo do fluxometro 5 já iniciou mas não terminou
  if ((Meter5.getCurrentFlowrate()) && (medida5))
  {
    //mostra a mensagem aguardando
    Serial.println("Aguardando fluxometro 5 . . .");
  }
  //se a medida foi finalizada
  else if ((!Meter5.getCurrentFlowrate()) && (!medida_terminada5))
  {
    //realiza a subtração da medida atual menos a medida anterior
    diferenca_litro5 = auxiliar5 - diferenca_litro5;

    //Mostra a diferença das medidas atuais e anteriores em militros
    Serial.println("------------------------------------------------------");
    Serial.println("COPO (fluxometro 5)" + (String)contador5 + ":" + (String)(diferenca_litro5*1000) + " ml");
    Serial.println("------------------------------------------------------");

    //reseta as flags para suas condições originais
    medida5 = false;
    medida_terminada5 = true;

    //guarda a medida atual para comparar na próxima varredura
    diferenca_litro5 = auxiliar5;

    //incrementa o contador de medidas
    contador5++;
  }








  //se o ciclo do fluxometro 4 já iniciou mas não terminou
  if ((Meter4.getCurrentFlowrate()) && (medida4))
  {
    //mostra a mensagem aguardando
    Serial.println("Aguardando fluxometro 4. . .");
  }
  //se a medida foi finalizada
  else if ((!Meter4.getCurrentFlowrate()) && (!medida_terminada4))
  {
    //realiza a subtração da medida atual menos a medida anterior
    diferenca_litro4 = auxiliar4 - diferenca_litro4;

    //Mostra a diferença das medidas atuais e anteriores em militros
    Serial.println("------------------------------------------------------");
    Serial.println("COPO (fluxometro 4)" + (String)contador4 + ":" + (String)(diferenca_litro4*1000) + " ml");
    Serial.println("------------------------------------------------------");

    //reseta as flags para suas condições originais
    medida4 = false;
    medida_terminada4 = true;

    //guarda a medida atual para comparar na próxima varredura
    diferenca_litro4 = auxiliar4;

    //incrementa o contador de medidas
    contador4++;
  }






  //se o ciclo do fluxometro 0 já iniciou mas não terminou
  if ((Meter0.getCurrentFlowrate()) && (medida0))
  {
    //mostra a mensagem aguardando
    Serial.println("Aguardando (fluxometro 0). . .");
  }
  //se a medida foi finalizada
  else if ((!Meter0.getCurrentFlowrate()) && (!medida_terminada0))
  {
    //realiza a subtração da medida atual menos a medida anterior
    diferenca_litro0 = auxiliar0 - diferenca_litro0;

    //Mostra a diferença das medidas atuais e anteriores em militros
    Serial.println("------------------------------------------------------");
    Serial.println("COPO (fluxometro 0)" + (String)contador0 + ":" + (String)(diferenca_litro0*1000) + " ml");
    Serial.println("------------------------------------------------------");

    //reseta as flags para suas condições originais
    medida0 = false;
    medida_terminada0= true;

    //guarda a medida atual para comparar na próxima varredura
    diferenca_litro0 = auxiliar0;

    //incrementa o contador de medidas
    contador0++;
  }
}
