Partea 2 - Achizitie Semnal Analogic
Pentru un senzor analogic sau digital de temperatura, sa se realizeze o aplicat , ie pentru
MCU care va achizit
, iona periodic semnalul de la senzor selectat din o lista de senzori,
va filtra semnalul si va afisa valorile intermediare si cea curenta prin STDIO fie catre un
afisor LCD, fie catre o interfata seriala. Aplicat ,
ia va realiza urmatoarele functionalitati
• Achizit
, ie(SensorAcquisition): cites
, terawdatadelasenzor(ADCpentrusenzori
analogici sau interfat ,
ă I2C / SPI / UART pentru senzori digitali), la o recurent,
ă
configurabilă (ex.: 20-100 ms). Expune valoarea brută printr-o interfat, ă internă (de
ex. sensor_read()).
• Condit
, ionaresemnal(SignalConditioning): primes , tevalorileraw,aplicăoperat ,
ii
de preprocesare: saturare, filtrare „sare s , i piper” (median filter), mediere ponderată.
Rularea acestui task poate rula la aceeas ,
i recurent
ă sau la o recurent
ă derivată.
,
,
• Afis
are s
,
, i raportare (Display & Reporting): la o recurent
, ă mai mică (ex.: 500
ms)afis, eazăpeLCDprinSTDIO(printf)unraportstructuratcuvalorileprocesate,
starea sistemului s
, i eventuale alerte (praguri depăs, ite).