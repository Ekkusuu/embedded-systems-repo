5.2.1 Partea 1 - Control Actuator binar
Pentru un actuator binar bec iluminare prin releu, sau altul convenit cu mentorul să se
realizeze o aplicat , ie pentru MCU care va interpreata comenzi de control de la utilizator
prin STDIO (serial sau keypad), va controla actuatorul afis, ând starea curentă pe un
display LCD. Aplicat ,
ia va realiza următoarele funct
ionalităt
i:
,
,
• Control actuator binar (Actuator Control): primes , te comenzi ON/OFF de
la utilizator prin STDIO (serial/tastatură), aplică debouncing s,
i controlează starea
actuatorului (releu, LED) la o recurent, ă configurabilă (ex.: 50-100 ms). Expune
starea actuatorului printr-o interfat , ă internă (ex. actuator_get_state()).
• Condit
, ionare semnal (Signal Conditioning): aplică operat , ii de preprocesare:
saturare, debouncing software pentru eliminarea comutărilor false, validare persis-
tentă a stării. Rularea acestui task poate fi la aceeas ,
i recurent
ă sau derivată.
,
• Afis
are s
,
, i raportare (Display & Reporting): la o recurent
, ă mai mică (ex.: 500
ms)afis, eazăpeLCDsauSTDIO(printf)unraportstructuratcustareaactuatorului
s
i eventuale alerte.


Cerint
, e suplimentare: Pentru lucrările de laborator cu actuatori, respectat ,
i urmă-
toarele recomandări pentru structură, modularitate s ,
i validare:
• Structurat
, i aplicat , ia modular, separând logica de control a actuatorului într-un
modul software dedicat.
• Utilizat
, i diagrame de stare sau de flux pentru a descrie comportamentul actuatorului
s
i condit
ionarea semnalului de comandă.
,
,
• Implementat , i debouncing software pentru comenzile binare (ex.: butoane, comuta-
ionează actuatorul.
toare) care act,
• Dacă folosit
, i FreeRTOS, creat , i taskuri dedicate pentru fiecare funct , ionalitate majoră
(interpretare comenzi, condit, ionare semnal, control actuator, afis , are).
5.3. Mersul Lucrarii 79
• Pentrucomunicareaîntretaskuri,utilizat , isemafoarebinaresaumutexpentruprotect ,
ia
variabilelor globale partajate.
• Reutilizat
, i modulele pentru actuatori, LED-uri, butoane s ,
i interfat
ă de afis
,
, are, acolo
unde este posibil.
• Testat
i fiecare funct
ionalitate s
i tranzit
ie de stare conform scenariilor de validare
,
,
,
,
definite.
• Documentat
, i clar arhitectura software, diagramele de stare/flux, interfet, ele HW/SW
s
i scenariile de testare.
,
• Consultat
, i resursele teoretice despre controlul actuatorilor, protect , ie la suprasarcină
s
, i proiectare modulară.
• Sistemul trebuie să răspundă la comenzi sau schimbări de semnal cu o latent ,
ă sub
100ms.
• Afis
at
i starea curentă a actuatorului s
,
,
, i valorile relevante pcatre STDIO prin interfat ,
a
serială sau LCD.
• utilizati instrumente de analiză s
, i vizualizare a datelor (ex.: Arduino Serial Plotter)
pentru a monitoriza performant , a sistemului în timp real.
Materiale si resurse necesare pentru realizarea lucrării:
• Componente hardware: microcontroler compatibil cu FreeRTOS (ex.: Arduino Mega,
ESP32), LED-uri (verde, ros, u sau galben) cu rezistent, ă de limitare, actuator la
alegere (releu cu optocuplor sau motor DC cu driver L298), display LCD pentru
afis
, area datelor, placă de prototipare (breadboard), cabluri de conexiune (jumper
wires), sursă de alimentare (USB sau baterie).
• Resurse software: PlatformIO, Wokwi sau Proteus pentru simulare s , i testare, em-
ulator terminal serial (ex.: Monitor Serial din PlatformIO, TeraTerm sau Putty),
instrument pentru realizarea diagramelor (ex.: draw.io, Lucidchart).
Realizarea sarcinii poate fi executată în mai multe variante, fiecare dintre ele cu un
punctaj diferit după cum urmează:

• Varianta C (100% din punctajul maxim): Implementarea sistem de control a
doi actuatori, unul binar s , i unul analogic, cu afis ,
area valorilor s
i alertelor de la ambii
,
actuatori pe display LCD.