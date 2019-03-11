/*
 * I2C.h
 *
 * Created: 12.12.2017 23:51:50
 *  Author: User
 */ 


#ifndef I2C_H_
#define I2C_H_
uint8_t volatile sekundy_tmp, sekundy,sekundy_jednosci;
uint8_t	volatile minuty_tmp, minuty;
uint8_t volatile godziny_tmp, godziny;
void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}
void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	_delay_us(10);
}
void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}
uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
void ustaw_czas(uint8_t godz, uint8_t min,uint8_t sek)
{
	sek=((sek/10) << 4) | (sek % 10); // zmiana wartoœci czasu z formatu binarnego na BCD
	min=((min/10) << 4) | (min % 10);
	godz=((godz/10) << 4) | (godz % 10);
	TWIStart();
	TWIWrite(0xA0);
	TWIWrite(0x02);
	TWIWrite(sek); //ustawienie czasu poprzez zapis do uk³adu PCF8583
	TWIWrite(min);
	TWIWrite(godz);
	TWIStop();
}
void init_pcf8583()
{
	TWIStart();
	TWIWrite(0xA0);
	TWIWrite(0x08); // uruchomienie trybu godziny, minuty, sekundy poprzez zapis 			  // do uk³adu
	TWIWrite(0x04);
	TWIStop();
}

#endif /* I2C_H_ */