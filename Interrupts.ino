//definir uma rotina de interrupção para cada sensor fluxometro que usar
void ICACHE_RAM_ATTR MeterISR5()
{
  //conta a quantidade de pulsos recebida
  Meter5.count();
  //habilita a flag de medida
  medida5 = true;
  //desabilita a flag de medida pronta
  medida_terminada5 = false;
}

//definir uma rotina de interrupção para cada sensor fluxometro que usar
void ICACHE_RAM_ATTR MeterISR4()
{
  //conta a quantidade de pulsos recebida
  Meter4.count();
  //habilita a flag de medida
  medida4 = true;
  //desabilita a flag de medida pronta
  medida_terminada4 = false;
}

//definir uma rotina de interrupção para cada sensor fluxometro que usar
void ICACHE_RAM_ATTR MeterISR0()
{
  //conta a quantidade de pulsos recebida
  Meter0.count();
  //habilita a flag de medida
  medida0= true;
  //desabilita a flag de medida pronta
  medida_terminada0 = false;
}
