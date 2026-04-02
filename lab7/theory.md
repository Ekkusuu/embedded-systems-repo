Sistemele electronice încorporate utilizează actuatoare pentru a interact ,
iona cu mediul
înconjurător, transformând semnale electrice de comandă în act , iuni fizice (pornire/oprie,
mis
, care, iluminare, ventilare etc.). Pentru ca aceste act,
iuni să fie realizate eficient s
, i sigur
de către microcontroler, este necesară o etapă complexă de control s ,
i condit
ionare a sem-
,
nalului de comandă, care implică generarea, filtrarea, adaptarea s , i protect ,
ia semnalului în
funct
ie de cerint
,
, ele specifice ale aplicat ,
iei s
, i de caracteristicile actuatoarelor. Acest capitol
prezintă principiile fundamentale ale act ,
ionării s
, i conversiei de putere, metodele de proce-
sare digitală a semnalului de comandă s ,
i modul în care acestea influent
, ează sigurant , a,
fiabilitatea s
, i performant ,
a sistemelor embedded moderne.
65
66 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
Fluxul de act
ionare s
,
, i conversie de putere reprezintă un lant , complex de transformări
prin care trece semnalul de comandă, de la forma sa init , ială generată de aplicat ,
ia embed-
ded, până la act , iunea fizică asupra mediului extern. Acest proces implică multiple etape
de prelucrare, atât în domeniul digital (generare, filtrare, temporizare, debouncing), cât
s
, i în cel electric (adaptare niveluri, protect, ie, amplificare), fiecare cu rolul său specific în
asigurarea funct ,
ionării corecte s
, i sigure a actuatoarelor.
Considerând funct
ia de act
ionare ca s
i funct
,
,
,
, ie complementară funct ,
iei de achizit
ie
,
s
, i semnalului, vom considera că s ,
i fluxul de condit
ionare s
i conversie a semnalului de
,
,
comandă pentru actuatori este similar cu cel al fluxului de achizit ,
ie s
i condit
ionare a
,
,
semnalului de la senzori, dar în sens invers [3], [4], .
În as
, a mod un flux tipic de act , ionare pentru un actuator controlat de un microcontroler
include:
• Expunerea interfet , elor software pentru controlul actuatorului (funct,
ii de init
, ializare,
pornire, oprire, setare parametri) 5.1.6.
• Conversie parametru fizic în semnal de putere/voltaj aplicat la actuator 5.1.5.
• Condit
, ionarea software a semnalului de comandă (filtrare, saturare, temporizare,
debouncing) 5.1.5.
• Conversie semnal de putere/voltaj în semnal digital de comandă (DAC, PWM,
ON/OFF) 5.1.4.
• Condit
, ionarea electrică a semnalului (amplificare, izolare, protect, ie la suprasarcină)
5.1.2.
• Conversiaenergieielectriceînact , iunefizicăprinintermediulconvertoarelordeenergie
(ex: încălzire, rotat, ie, iluminare) 5.1.1.
Reprezentarea grafică a acestui flux, cu componentele sale principale s ,
i transformările
specifice, este ilustrată în figura 5.1.
Figure 5.1: Fluxul de generare, condit ,
ionare s
, i conversie a semnalului de comandă pentru actuatori,
evident
, iind etapele de procesare de la semnal digital la act , iune fizică [3]
În sect
, iunile ce urmează vom analiza pe rând fiecare etapă a fluxului de act ,
ionare s
i
,
conversie a semnalului de comandă pentru actuatori.
5.1.1 Conversia energiei în act ,
iune fizică
Act
, iunea fizică a actuatoarelor este realizată ca regulă prin intermediul convertoarelor de
energie dintr-o formă în alta. As , a cum în sistemele contemporane se utilizează pe larg
energia electrică în majoritatea aplicat , iilor, conversia energiei este de regulă o conversie
din energia electrică în altă formă de energie.
De exemplu energie electrică în energie termică cu o rezistent , ă electrică, care ulte-
rior încălzes
, te un element conductor (aer, lichid, solid) de căldură către mediu pentru
realizarea unui sistem de încălzire de exemplu al încăperii. În figura 5.2 este ilustrat un
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 67
exemplu de încălzitor alimentat de la o sursă de energie.
Figure 5.2: Tipuri de încălzitoare electrice [9]
Pentru iluminare se vor folosi convertoare de energie electrică în flux luminos, cele
mai populare dispozitive fiind cu un LED, lampa halogen sau bec electric incandescent
prezentate în figura 5.3.
Figure 5.3: Tipuri de dispozitive de iluminare [29]
Pentru mis
, care se vor folosi convertoare de energie electrică în energie mecanică, aceste
convertoare purtând denumirea de motor electric. Un motor electric se poate folosi de
exemplu la punerea în mis , care a unui vehicul electric, a unui ventilator pentru răcirea
unui sistem electronic sau a unei pompe Există diverse tipuri de motoare electrice cel mai
simplu ca s ,
i construct
, ie fiind motorul de curent continuu (DC) prezentat în figura 5.4.
Figure 5.4: Motor de curent continuu (DC) [8]
68 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
Tipul convertorului utilizat depinde de cerint , ele specifice ale aplicat , iei, inclusiv de
tipul de act , iune dorită (încălzire, iluminare, mis, care), de puterea necesară, de eficient,
a
energetică s ,
i de condit
, iile de operare.
Chiar dacă cele mai multe conversii de energie se realizează din energie electrică în
alte forme de energie, asta nu exclude că sunt practicate pe larg s , i alte tipuri de conversii
de energie cum ar fi conversia energiei pneumatice sau hidraulice în energie mecanică
care ulterior se convertes
, te în energie electrică cu un generator electric care ulterior este
convertită în altă formă de energie sau energie electrică se convertes , te în energie chimică
cu o electroliză pentru a fi stocată într-o baterie sau alt tip de acumulator. Aceste cazuri
de conversii se folosesc în colectarea energiei regenerabile. Însă acest context este în afara
scopului acestei lucrări de laborator.
5.1.2 Conversias
icondit
,
, ionareaelectricăapentruactuatorbinar(ON/OFF)
Conversia semnalului electric de control în energie electrică necesară pentru act ,
ionarea
actuatorului implică mai multe etape principale, fiecare având un rol specific în asigurarea
funct
ionăriicorectes
,
, isigureasistemului. Vomanalizaaicicazulparticularedeactuatoare,
unul binar.
Un actuator binar este un dispozitiv care poate avea doar două stări: pornit sau
oprit. Exemple de actuatoare binare includ: relee, comutatoare s ,
i LED-uri. În cazul
unui actuator binar, semnalul de control este de obicei un semnal digital (ON/OFF) care
activează sau dezactivează dispozitivul.
O schema electrică tipică pentru controlul unui actuator binar (releu) este prezentată
în figura 5.5.
Figure 5.5: Schema electrică tipică pentru controlul unui releu cu un microcontroler [19]
Etapele de conversie pentru o comanda a unui actuator binar cu releu vor include
adaptarea nivelurilor de semnal, izolarea galvanică, amplificarea semnalului, protect ,
ia la
tensiune inversă s
, i controlul sarcinii, fiecare fiind detaliat mai jos:
Adaptarea nivelurilor de semnal: conversia semnalului digital de la microcontroler
(3.3 V sau 5 V) la nivelurile necesare actuatorului. În schema din figura 5.5, tensiunea
de 5 V trebuie adaptată la aprox. 1.2 V pentru dioda optocuplorului. Rezistorul R1
limitează curentul s
, i asigură căderea de tensiune necesară. Pentru un curent maxim de
10mA prin diodă s , i considerând căderea de tensiune pe dioda de protect , ie (0.7V):
5V− 1.2V− 0.7V
3.1V
R1 =
10mA=
10mA = 310 Ω
Izolarea galvanică: în schema din figura 5.5, izolarea galvanică se realizează prin op-
tocuplor, protejând microcontrolerul de supratensiuni sau zgomote electrice din circuitul
de putere. Optocuplorul transmite semnalul prin lumină, fără conexiune electrică directă
între comanda (microcontroler) s, i putere (releu/sarcină). Lumina de la dioda LED ac-
tivează fototranzistorul, care furnizează semnalul pentru etajul de amplificare s ,
i comanda
releului.
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 69
Amplificareasemnalului: pentruamplificareacurentuluinecesarpentruaactivareleul,
se utilizează un tranzistor (ex: NPN) ca întrerupător de putere. Când fototranzistorul
optocuplorului este activat, acesta alimentează baza tranzistorului prin rezistorul R2,
permit ,
ând curentului să treacă de la colector la emitor s
i să alimenteze bobina releului.
,
Protect
,
ia la tensiune inversă: când releul este dezactivat, bobina poate genera o
tensiune inversă (back-EMF) care poate deteriora tranzistorul s, i alte componente. Dioda
D1 (dioda de protect, ie) este plasată în paralel cu bobina releului pentru a oferi un traseu
sigur pentru această tensiune inversă, protejând astfel circuitul de eventuale daune.
Controlul sarcinii: releul act
, ionează ca un întrerupător electromecanic, permit ,
ând sau
întrerupând alimentarea sarcinii conectate la contactele sale. Când bobina releului este
energizată, contactele se închid, permit , ând curentului să treacă către sarcină (ex: un bec
electric, un motor).
Datorită faptului că releul reprezintă un contact mecanic, acesta poate fi supus poate
conduce curent
, i mari către sarcina de exemplu un bec electric, cum e prezentat în figura
5.6.
Figure 5.6: Exemplu de sarcină controlată printr-un releu: un bec electric [19]
În concluzie, aceste etape de conversie s ,
i condit
, ionare a semnalului asigură că actua-
torulbinar(releul)estecontrolateficients, iînsigurant , ădecătremicrocontroler, prevenind
daunele s
, i asigurând o funct , ionare fiabilă a sistemului. realizate prin transformări suc-
cesive: semnal logic → lumină → curent → energie electrică, fiecare etapă incluzând
protect , ii adecvate pentru funct , ionarea sigură s ,
i fiabilă a sistemului.
Totus
, i, contactele releului au o durată de viat ,
ă limitată din cauza uzurii mecanice.
Pentru aplicat ,
ii cu comutat
, ii frecvente sau unde este necesară o fiabilitate ridicată, se
recomandă utilizarea actuatoarelor de stare solidă (SSR - Solid State Relay) sau a altor
dispozitive fără contacte mecanice, care oferă un număr practic nelimitat de comutat ,
ii s
i
,
o rezistent
, ă sporită la medii dificile. Deci una din limitările, caracteristicile sau domeniile
de aplicare ale metodei de act , ionare cu semnal binar (ON/OFF) este controlul simplu, de
activare/dezactivare, cu o frecvent, ă mică, a unor sarcini electrice (ex: iluminare, încălzire,
motoare simple) s, i este recomandat să se ia în considerare acest aspect la proiectarea
unui sistem de act
ionare. Pentru frecvent
e mari de comutat
,
,
, ii sau pentru control fin al
parametrilor sarcinii se recomandă metode de act , ionare mai avansate, cum ar fi PWM
(modulat, ia impulsului în durată) sau controlul analogic.
5.1.3 Conversia s
i condit
,
, ionarea electrică a pentru actuator Analogic vari-
abil
Semnal de comanda analogic versus PWM
În practică, se disting două tipuri principale de comandă pentru actuatoare analogice,
PWM sau tensiune variabilă. Fiecare metodă are avantaje s , i dezavantaje specifice, iar
alegerea depinde de aplicat , ia specifică s ,
i de cerint
ele de control.,
70 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
Comandaprinsemnaldetensiunevariabilă(regimliniar). Tranzistorulfunct
ionează
,
în zona activă, unde curentul de colector este proport ,
ional cu tensiunea de comandă. Des
i
,
permite control în curent continuu, acest mod generează disipare termică semnificativă în
tranzistor (puterea disipată Pd = VCE × IC ), necesitând radiator adecvat. Se utilizează
rar în aplicat , ii embedded, fiind mai frecvent în aplicat , ii audio sau în sisteme analogice
specifice unde se impune un răspuns continuu fără comutat ,
ie.
Comanda prin semnal PWM (modulat,
ie în lăt
, ime de impuls). Tranzistorul co-
mută rapid între saturat ,
ie s
, i blocare, creând o tensiune medie variabilă care controlează
viteza motorului. Frecvent
, a PWM tipică este de 1-20 kHz, suficient de mare pentru ca
inert
ia motorului să netezească oscilat
iile mecanice. Acest mod oferă control fin al vitezei
,
,
cu eficient
, ă energetică ridicată, fiind solut , ia preferată în aplicat ,
ii embedded moderne.
Pentru aplicat , iile embedded tipice cum ar fi controlul motoarelor DC, se recomandă co-
manda PWM datorită eficient
, ei energetice ridicate s , i controlului precis al vitezei, fără
supraîncălzirea componentelor de putere.
Semnalul PWM poate fi numit un semnal quasi-analogic, deoarece prin forma lui de
undă dreptunghiulară reprezintă un semnal binar iar prin valoarea medie a tensiunii apli-
cate la sarcină reprezintă un semnal analogic variabil. Astfel semnalul PWM combină
avantajele ambelor tipuri de semnale:
• Control digital precis s , i imun la zgomot, specific semnalelor binare.
• Eficient
, ă energetică ridicată s , i disipare termică redusă, specific semnalelor digitale.
• Control fin al parametrilor sarcinii (ex: viteză motor) prin variat, ia raportului de
umplere (duty cycle).
O prezentare grafică pentru relat ,
ia între semnalul PWM s
, i tensiunea medie aplicată la
sarcină este prezentată în figura 5.7.
Figure 5.7: Relat ,
ia între semnalul PWM s
, i tensiunea medie aplicată la sarcină [18]
Control unipolar al actuatorului
Control unipolar al actuatorului este o metoda care poate avea o gamă continuă de val-
ori între două extreme, dar care operează într-o singură direct , ie. Exemple de Control
unipolar al actuatorului includ control motoare DC cu un singur sens de rotat ,
ie s
i valve
,
proport , ionale unipolare. În cazul unui Control unipolar al actuatorului, semnalul de con-
trol este de obicei un semnal în domeniu de valori continue (de exemplu, un semnal PWM
sau un semnal de tensiune variabilă) care reglează pozit, ia, viteza sau fort , a dispozitivului.
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 71
O schema electrică tipică pentru controlul unui motor DC cu un tranzistor este prezen-
tată în figura 5.8.
Figure 5.8: Schema electrică tipică pentru controlul unui motor DC cu un tranzistor [10]
Ca s
, i etape de conversie pentru o comandă a controlului unipolar al actuatorului includ
- adaptare niveluri, amplificare, transfer de putere, protect , ie la suprasarcină, fiecare fiind
detaliat mai jos:
Adaptarea nivelurilor de semnal: semnalul de control (3.3 V) este adaptat prin
rezistorul R1 pentru a activa baza tranzistorului. Pentru regimul activ valoarea lui R1
este calculată pentru un curent de bază de aproximativ 1 mA:
R1 =
Vcontrol− VBE
IB
3.3V− 0.7V
=
1mA = 2.6kΩ
unde VBE ≈ 0.7V este căderea de tensiune bază-emitor. În practică, se alege 3.3 kω.
Variat
, ia semnalului 0-3.3 V permite controlul vitezei motorului prin modularea curentului
de colector, funct , ionând în regim activ. Opt , ional, se poate aplica filtrare software pentru
o comandă mai lină.
PentruregimulPWM,R1estedimensionatpentruaasigurauncurentdebazăsuficient
pentruasaturatranzistorullatensiuneamaximădecomandă(3.3V).Deexemplu, pentru
un curent de bază de 5 mA:
R1 =
Vcontrol− VBE
IB
3.3V− 0.7V
=
5mA = 520Ω
Se poate alege 560 Ω ca valoare standard. În acest mod, tranzistorul intră în saturat ,
ie
completă când semnalul PWM este HIGH, minimizând disiparea de putere.
Amplificarea s , i transferul de putere: tranzistorul funct
, ionează ca un întrerupător
controlat electric, amplificând semnalul slab de la microcontroler pentru a comanda put-
erea necesară motorului. Când baza primes , te un semnal HIGH (prin R1), tranzistorul
intră în saturat
, ie, permit , ând curentului să circule de la colector la emitor, alimentând
astfel motorul. Curentul de colector maxim IC este limitat de sarcină (motorul) s,
i de
tensiunea de alimentare:
Vsupply
IC =
Rmotor
Tranzistorul trebuie ales astfel încât să suporte acest curent maxim, cu un coeficient de
sigurant ,
ă adecvat.
Protect
ia la tensiune inversă s
,
, i suprasarcină: motoarele DC generează tensiuni
inverse (back-EMF) atunci când sunt oprite sau când direct,
ia curentului se schimbă brusc.
Dioda D1, plasată în paralel cu motorul, oferă un traseu sigur pentru aceste tensiuni
72 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
inverse, protejând astfel tranzistorul s , i alte componente sensibile. Dioda trebuie să fie de
tipul de recuperare rapidă (fast recovery) s, i să suporte curentul maxim al motorului. În
plus,serecomandăadăugareaunuicondensatordefiltrareînparalelcusursadealimentare
pentru a reduce zgomotul electromagnetic generat de comutarea motorului s , i pentru a
proteja împotriva supratensiunilor tranzitorii.
Control bipolar al actuatorului
Controlul bipolar al actuatorului este o metodă care poate avea o gamă continuă de
valori între două extreme, operând în ambele direct , ii. Exemple de control bipolar al
actuatorilor includ motoarele cu sens de rotat
ie reversibil s
,
, i motoarele pas cu pas in
regim bipolar. În cazul acesta, semnalul de control este de obicei reprezentat de două
semnaleanalogiceaplicateladouăpoluridiferitealeactuatorului. Odiferent , ădepotent ,
ial
pozitivă va conduce actuatorul într-o direct , ie, iar o diferent , ă de potent , ial negativă va
conduce actuatorul în direct
, ia opusă. Această alternare de polaritate poate fi realizată
prin conectarea polilor actuatorului prin chei electronice fie la sursa de alimentare, fie la
ground. Circuitul care realizează această alternare poartă denumirea de punte H, datorită
schemei electrice care seamănă cu litera H. Schematic puntea H este prezentată în figura
5.9.
(a) Concept cu chei electrice (b) Implementare cu tranzistori
Figure 5.9: Schema electrică tipică pentru o punte H [26]
Considerentele de aplicat ,
ie sunt similare ca s
, i la actuatorul analogic unipolar, cu anu-
mite limitări, s ,
i anume:
• Necesitatea controlului a patru chei electronice (tranzistori) pentru a realiza al-
ternarea polarităt ,
ii.
• Necesitatea evitării scurtcircuitării sursei de alimentare prin deschiderea simultană
a cheilor din aceeas
, i ramură a punt ,
ii H.
• Necesitatea utilizării unor tranzistori de tip P pentru partea superioară a punt , ii H,
care necesită o tensiune de comandă mai mare decât tensiunea de alimentare pentru
a se satura complet.
În scop dea simplifica controlulpunt , ii H, serecomandă utilizarea solut , iei integrate cum ar
fi driverul L298, care include toate cele patru tranzistori necesari s , i circuitele de protect ,
ie
necesare într-un singur pachet.
figura 5.10 reprezintă schema electrică a driverului L298 s , i modul de conexiune pentru
controlul bipolar a două motoare DC cu câte un H-Bridge pentru fiecare motor.
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 73
Figure 5.10: Schema electrică a driverului L298 s , i conexiunea pentru controlul a două motoare DC în
mod bipolar [18]
În figura 5.11 este prezentată schema electrică conceptuală al driverului L298 prezen-
tată ca s
, i 4 amplificatoare, câte unul pentru fiecare ramură a puntii H. Acest fapt permite
controlul unipolar a 4 motoare s , i control bipolar a 2 motoare.
Figure 5.11: Schema electrica conceptuala al driverului L298 [27]
Având această abstractizare a puntii H, controlul unui motor DC bipolar devine similar
cu cel al unui motor unipolar, cu diferent , a că pentru a schimba sensul de rotat ,
ie al mo-
torului, se inversează semnalul de comandă aplicat la intrările driverului L298. Respectiv
s
, i etapele de conversie s ,
i condit
ionare a semnalului sunt similare cu cele ale motorului
,
unipolar prezentate în sect ,
iunea anterioară 5.1.3.
5.1.4 Conversie semnal de comandă software în semnal electric de comandă
(DAC, PWM, ON/OFF)
Conversia semnalului de comandă software într-un semnal electric adecvat pentru actua-
tor implică utilizarea unor tehnici specifice, în funct , ie de tipul actuatorului s ,
i de cerint
ele,
74 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
aplicat , iei. Cele mai comune metode includ utilizarea de DAC-uri (digital-to-analog con-
verters), PWM (pulse-width modulation) s, i semnale ON/OFF.
Generarea semnalului ON/OFF
Generarea semnalului ON/OFF pentru un actuator se poate realiza prin simpla comutare
a unui pin de ies ,
ire al microcontrolerului între stările HIGH s
, i LOW. Aceasta poate fi
realizată prin intermediul unei funct , ii de control care să primească ca parametru starea
dorită (pornit/oprit) s, i să seteze pinul corespunzător.
Componenta periferică a microcontrolerului care realizează această funct , ie este portul
GPIO (General Purpose Input/Output). Pentru a fi utilizat un pin GPIO ca ies, ire digi-
tală, acesta trebuie configurat corespunzător în modul de ies , ire prin intermediul regis ,
trilor
periferici ai microcontrolerului. La fel ca s , i pentru generarea de nivel digital de iesire la
pinul GPIO.
Libraria Arduino oferă funct
, ii simple pentru configurarea pinilor GPIO s , i generarea
semnalului ON/OFF, cum ar fi pinMode(...) s
,
i digitalWrite(...). unde:
• pinMode(pin, OUTPUT) configurează pinul pin ca ies
, ire digitală.
• digitalWrite(pin, HIGH/LOW) setează starea pinului pin la HIGH sau LOW.
Generarea semnalului PWM
GenerareasemnaluluiPWMpentruunactuatorsepoaterealizaprinconfigurareaunuipin
deies
irealmicrocontroleruluiînmodulPWMs
,
, isetareafactoruluideumplere(dutycycle)
dorit. Periferiile microcontrolerului implicate în realizarea acestei funct , ii sunt GPIO, tem-
porizatoarele (timers) s, i modulele PWM (Pulse Width Modulation). GPIO este utilizat
pentru a seta pinul ca ies , ire PWM, temporizatoarele sunt folosite pentru a genera sem-
naluldeceasnecesarpentruPWM,iarmodulelePWMsuntresponsabilepentrugenerarea
efectivă a semnalului PWM cu factorul de umplere (duty cycle) dorit. Configurat,
iile se
pot realiza prin intermediul regis , trilor periferici ai microcontrolerului sau prin utilizarea
unor librării software care abstractizează aceste detalii.
Libraria Arduino oferă funct
, ii simple pentru generarea semnalului PWM, cum ar fi
analogWrite(pin, value) unde value este un număr între 0 s
, i 255 care reprezintă
factorul de umplere (duty cycle) dorit. În cazul librăriei Arduino , aceasta nu oferă
flexibilitatea de a seta frecvent
, a semnalului PWM, s , i alte rafinamente care pot fi necesare
în aplicat , ii avansate. Pentru astfel de cazuri, este recomandat să se utilizeze librării
specializatedenivelprofesional sausăseconfigurezedirectperifericele microcontrollerului
prin intermediul regis ,
trilor săi.
Generarea semnalului analogic (DAC)
În cazul în care se dores
, te generarea unui semnal analogic real, (nu PWM), generarea
semnalului analogic pentru un actuator se poate realiza prin utilizarea unui convertor
digital-analogic (DAC) integrat în microcontroler sau prin utilizarea unui DAC extern
conectat la microcontroler printr-o interfat ,
ă de comunicat
, ie (precum SPI sau I2C). Per-
iferiile microcontrolerului implicate în realizarea acestei funct , ii sunt DAC-ul intern (dacă
este disponibil) sau interfat,
a de comunicat
, ie pentru DAC-ul extern. Configurarea DAC-
ului se poate realiza prin intermediul regis , trilor periferici ai microcontrollerului sau prin
utilizarea unor librării software care abstractizează aceste detalii.
O prezentare de implementare în variantă discretă cu un DAC după schema R-2R este
prezentată în figura 5.12.
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 75
Figure 5.12: Schema electrică tipică pentru un DAC R-2R [15]
Datorită ret
elei de rezistent
e R s
,
,
, i 2R, se realizează o divizare ponderată a tensiunii de
referint
, ă, undefiecarebitalcuvântuluidigitalcontribuiecuoponderediferitălatensiunea
V1. Aceasta este apoi amplificată în curent cu un amplificator operat , ional repetor pentru
a obt
ine tensiunea de ies
,
, ire analogică V2.
Librăria Arduino oferă funct
, ii simple pentru generarea semnalului analogic pe micro-
controlerecuDACintegrat(precumArduinoDuesauESP32),cumarfianalogWrite(pin,
value) aplicată pe pinii DAC dedicat , i, unde value este un număr între 0 s
i valoarea max-
,
imă suportată de rezolut , ia DAC-ului (de exemplu, 0-4095 pentru un DAC pe 12 bit, i).
Pentru microcontrolere fără DAC integrat, se poate utiliza un modul DAC extern (ex:
MCP4725) comunicând prin I2C sau SPI.
5.1.5 Condit
ionarea software a semnalului de comandă
,
Condit
, ionarea semnalului de comandă software, înainte ca acesta să prindă o formă elec-
trică adecvată pentru actuator, poate include diverse tehnici de procesare necesare pentru
a asigura o funct ,
ionare corectă s
, i sigură a sistemului de act ,
ionare.
Considerând funct
ia de act
,
, ionare ca fiind una complementară celei de achizit ,
ie de
semnal, tehnicile de condit , ionare software a semnalului de comandă pot fi similare cu
cele utilizate în achizit
, ia de semnal, dar adaptate pentru nevoile specifice ale controlului
actuatorului. Mai jos vom analiza câteva tehnici comune care pot fi aplicate în acest
context evident
, iind aspectele aplicare pentru domenul actuatorilor.
Conversia dintr-un domeniu în altul
Conversia semnalului de comandă dintr-un domeniu în altul are rolul de a transforma
mărimea fizică definită în software (ex.: viteza dorită a motorului în RPM) într-o mărime
electrică generată de microcontroler (ex.: tensiune aplicată la polii motorului). Această
etapă permite ca valorile utilizate în aplicat , ie, exprimate în unităt , i relevante pentru uti-
lizator, să fie convertite în semnale electrice compatibile cu cerint ,
ele actuatorului.
Procedeele de conversie utilizate sunt similare cu cele prezentate pentru senzori în
sect
iunile anterioare:
,
• Scalare s
, i offset — pentru relat , ii liniare simple între domenii ( 4.1.7).
• Conversie liniară prin două puncte — pentru relat , ii liniare definite de puncte de
calibrare ( 4.1.7).
• Conversie neliniară prin LUT (Look-Up Table) — pentru relat, ii complexe care nu
pot fi aproximate liniar ( 4.1.7).
Exemplu practic: Motor DC Un exemplu reprezentativ este conversia vitezei dorite
a unui motor DC în tensiunea aplicată la polii acestuia. Această relat ,
ie nu este liniară
datorită caracteristicilor electrice s
, i mecanice ale motorului (rezistent, a bobinei, pierderile
prin frecare, fort , a contra-electromotoare). Pentru un anumit motor DC, relat,
ia dintre
tensiune s
, i viteză poate fi reprezentată astfel:
76 Lucrarea de laborator nr. 5. Actuatori - Act
ionare s
,
, i conversie putere
Tensiune (V) Viteză (RPM)
0 0
1 60
2 110
3 140
4 160
5 175
Table 5.1: Exemplu de relat ,
ie nelineară între tensiune s
, i viteză pentru un motor DC
Graficul din figura 5.13 prezintă această relat , ie nelineară, evident ,
iind cres
, terea rapidă
init
, ială a vitezei pentru tensiuni mici, urmată de o saturare progresivă la tensiuni mai
mari.
Viteză (RPM)
200
150
100
50
0
Curba Tensiune-Viteză
0 1 2 3 4 5
Tensiune (V)
Figure 5.13: Relat ,
ia nelineară dintre tensiune s
, i viteză pentru un motor DC
Datorităacesteineliniariat
, i, conversiadintrevitezadorităs , itensiuneaaplicatănecesită
o abordare specială. O metodă eficientă este utilizarea unei tabele de căutare (LUT -
Look-Up Table) cu interpolare liniară între punctele definite, similar metodei prezentate
în sect
, iunea 4.1.7 pentru senzori.
Pentru conversia inversă (de la parametrul fizic dorit - viteza în RPM - la tensiunea de
aplicat în Volt , i), se va folosi aceeas,
i funct
, ie de interpolare LUT, dar cu inversarea axelor
din tabelul de căutare. Astfel, funct , ia va căuta în coloana de viteză pentru a identifica
intervalulcorespunzătorvitezeidorites , ivareturnatensiuneainterpolatăcorespunzătoare.
ImplementareaîncodCpentruaceastăconversievautilizafunct ,
ialib_sig_cond_lut_float()
prezentată anterior, adaptată pentru axele inversate (viteză →tensiune în loc de tensiune
→ viteză).
Saturarea semnalului de comanda
Saturarea semnalului de comandă se referă la limitarea valorilor acestuia într-un interval
specificat, pentru a preveni depăs , irea limitelor fizice ale actuatorului. De exemplu, dacă
un motor DC are o viteză maximă de 1000 RPM, semnalul de comandă trebuie să fie
limitat la această valoare. Mecanismul de saturare poate fi implementat printr-o funct ,
ie
simplă care verifică dacă semnalul de comandă depăs ,
es
, te limitele specificate s , i îl ajustează
corespunzător. Mecanismul de saturare a fost prezentat în sect ,
iunea 4.1.4.
Filtru impulsionar pentru semnalul de comanda
Filtrarea impulsionară a semnalului de comandă se referă la aplicarea unui filtru pentru a
elimina zgomotul sau fluctuat ,
iile nedorite din semnalul de comandă datorat unor tranzit
ii
,
interne ale sistemului în evaluarea comenzii. Filtrul median este o solut
, ie simplă pentru
această problemă. Această metodă de filtrare a fost prezentată în sect ,
iunea 4.1.4.
5.1. Analiza domeniului de aplicare al actuatorilor s , i conversiei de putere 77
Filtru trece-jos pentru semnalul de comanda
Filtrul trece-jos este utilizat pentru a elimina componentele de frecvent ,
ă înaltă din sem-
nalul de comandă, pentru a reduce schimbările brus , te aplicate actuatorului s , i pentru a
asigura o tranzit , ie mai lină între valorile comenzii. Acest lucru este deosebit de important
în cazul actuatorilor mecanici, unde schimbările brus , te pot duce la uzura prematură a
componentelor. Filtrul trece-jos poate fi implementat printr-un filtru digital de mediere
ponderată, prezentat în sect ,
iunea 4.1.4.
Mecanism de pornire lină (rampare) a semnalului de comandă
Mecanismul de pornire lină (sau rampare) a semnalului de comandă se referă la intro-
ducerea unei perioade de tranzit , ie graduală atunci când se schimbă valoarea semnalului
de comandă. Un exemplu este cres ,
terea vitezei cu accelerat
, ie constantă. care raportat la
formula de fort
, ă va limita sarcina aplicată asupra actuatorului. Acest mecanism poate fi
implementat printr-o funct , ie care ajustează treptat valoarea semnalului de comandă pe
parcursul unei perioade specificate, în loc să o schimbe brusc.
Legitatea prin care se asigură o accelerare s ,
i o decelerare lină a actuatorului este
prezentată de formula
∆v
a=
∆t
unde:
• a este accelerat
, ia (sau decelerat, ia) constantă,
• ∆v este schimbarea vitezei (diferent,
a dintre viteza finală s
i cea init
,
, ială),
• ∆t este intervalul de timp în care are loc această schimbare.
Considerând ∆t ca fiind recurent
, a taskului de control, ∆v va fi valoarea a schimbării
de viteză pe fiecare pas de control.
Un grafic care prezintă conceptul de pornire lină (rampare) este prezentat în figura
5.14.
Viteză (RPM)
120
80
40
0
∆v
∆v a =
∆t
∆t
Pornire/Oprire lină
0 10 20 30 40 50 60 70 80 90 100
Timp (ms)
Figure 5.14: Conceptul de pornire lină (rampare) s, i oprire lină (decelerare) a semnalului de comandă
pentru motor DC
5.1.6 Expunerea interfet , elor software pentru controlul actuatorului
• Init
ializarea actuatorului.
,
• Pornirea s
, i oprirea actuatorului.
• Setarea parametrilor de control (ex: viteză, direct, ie).
• Monitorizarea stării actuatorului.
Aceste interfet
, e vor fi implementate ca funct , ii C care pot fi apelate din orice parte a
aplicat , iei, asigurând modularitatea s ,
i reutilizabilitatea codului.