6.2.1 Partea 1: Reglare automata cu ON-OFF control cu hstereza
Să se proiecteze s , i să se implementeze o aplicat , ie modulară pentru microcontroler (MCU)
care realizează controlul ON-OFF cu histereză asupra unui parametru fizic (de exemplu:
temperatură, umiditate sau pozit , ie rotor). Sistemul va act, iona un actuator (releu sau
driver L298) în funct, ie de valoarea măsurată a parametrului, comparată cu o valoare de
referint
, ă (Set Point) configurabilă.
Setareavaloriidereferint
, ăsevafaceprintr-unmecanismdeinteract , iuneales(potent, iometru,
butoane, tastatură (UP/DOWN) sau interfat, ă serială). Valorile parametrului măsurat,
ale Set Point-ului s
i starea actuatorului vor fi afis
,
, ate pe LCD s , i/sau prin interfat,
a serială
(STDIO). Controlul ON-OFF va utiliza histereză pentru a preveni comutările frecvente
ale actuatorului la variat
, ii mici ale parametrului controlat. Rularea functionalitatilor
achizitie, procesare, si actionare, vor fi realizate in sistemul de operare FreeRTOS fie cu
tascuri secventiale cu recurente justificate..
Sistemul va implementa una dintre următoarele variante:
• Varianta A - Control temperatură sau umiditate, pe baza valorii de la un senzor
digital (ex. DHT22), cu act, ionare prin releu.
Pentru demonstrarea controlului în timp real, datele relevante (SetPoint, Value, Output)
vor fi trimise către Arduino Serial Plotter.

Cerint
, e suplimentare: Pentru lucrările de laborator cu control automat (ON-OFF
cu histereză, PID), respectat, i următoarele recomandări pentru structură, modularitate s ,
i
validare:
• Structurat
, i aplicat , ia modular, separând logica de control (ON-OFF/PID), achizit, ie,
condit
ionare s
i act
ionare în module software dedicate.
,
,
,
• Utilizat
, i diagrame de stare s , i/sau diagrame bloc pentru a vizualiza algoritmul de
control s
i fluxul de date.
,
• Implementat , i debouncing software pentru toate intrările digitale (butoane, senzori).
• Dacă folosit
, i FreeRTOS, creat , i taskuri separate pentru achizit , ie, control, act ,
ionare
s
i afis
,
, are, fiecare cu recurent , ă justificată.
• Pentru comunicarea între taskuri, utilizat , i semafoare binare, cozi sau mutex pentru
protect , ia variabilelor partajate.
• Afis
at
,
, i valorile relevante (SetPoint, valoare măsurată, ies, ire control, stare actuator)
catre STDIO prin interfat ,
a serială sau LCD.
• Reutilizat
, i modulele pentru senzori, actuatori s ,
i interfat
ă de afis
,
, are, evitând dupli-
carea codului.
• Testat
i fiecare scenariu de control s
i tranzit
ie de stare conform criteriilor de validare
,
,
,
definite.
• Documentat
, i clar arhitectura software, diagramele de control, interfet , ele HW/SW s,
i
scenariile de testare.
• Consultat
, i resursele teoretice despre control automat, histereză, PID s , i proiectare
modulară.
• utilizati instrumente de analiză s
, i vizualizare a datelor (ex.: Arduino Serial Plotter)
pentru a monitoriza performant , a sistemului în timp real.