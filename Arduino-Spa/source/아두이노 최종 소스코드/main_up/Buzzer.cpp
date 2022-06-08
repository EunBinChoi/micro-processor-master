#include "Buzzer.h"
void BuzzerContol(int Freq, int PlayTime)
{
tone(BUZZER_PIN,Freq,PlayTime);
}
void BuzzerStop()
{
noTone(BUZZER_PIN);
}
