controlul unui sistem prin reglarea autmata presupune mentineara unui parametru la o
valoare dorita. Sau mai in detaliu, utilizarea unor algoritmi care ajustează parametrii de
funct
, ionare ai sistemului pentru a ment , ine un anumit set point (valoare de referint, ă) în
fat
a variat
iilor s
,
,
, i perturbărilor externe.
În contextul sistemelor electronice incorporate, acest proces implică achizit ,
ia de date
de la senzori, procesarea acestor date pentru a determina eroarea fat , ă de set point, s ,
i
aplicarea unei act , iuni de control prin intermediul unui actuator pentru a corecta această
eroare.
Cele mai frecvent folosite metode de reglare automată sunt controlul ON-OFF cu
histereză s
, i controlul PID (Proport, ional-Integral-Derivativ). Le vom explora pe scurt in
83
84 Lucrarea de laborator nr. 6. Reglare automata si in sistemele incorporate
sectiunile de mai jos.
6.1.1 Control On/Off
Controlul ON-OFF este o metodă simplă de reglare automată care implică comutarea
actuatorului între două stări: ON (pornit) s, i OFF (oprit), în funct,
ie de valoarea măsurată
a parametrului fizic comparativ cu o valoare de referint ,
ă.
Schematic acest tip de control poate fi reprezentat ca in diagrama din figura 6.1.
Figure 6.1: Diagrama bloc a controlului ON-OFF[6].
unde legitatea de control se va exprima prin formula 6.1
R(t) =
  
Kc, dacă Vact(t) <Vdes(t)
0, altfel (6.1)
unde R(t) este semnalul de control ON/OFF la momentul t, Kc este valoarea de co-
mandă pentru ON (ex: 1 pentru releu), Vact(t) este valoarea măsurată la t, iar Vdes(t)
este valoarea de referint
ă la t.
,
grafic acest tip de control va avea un comportament ca in figura 6.2.
Figure 6.2: Graficul de funct , ionare al controlului ON-OFF[6].
Aceasta metoda are cateva neajunsuri printre care si faptul ca poate duce la comutari
frecvente ale actuatorului in jurul punctului de referinta, ceea ce poate duce la uzura
prematura a componentelor. Pentru a preveni acest fenomen, se poate introduce histereză
în sistemul de control.
Histereza introduce o marjă de tolerant , ă în jurul valorii de referint , ă, definite ca puncte
de Vof f si Von. Astfel, actuatorul va comuta la ON doar când valoarea măsurată scade
sub Vof f s
, i va comuta la OFF doar când valoarea măsurată depăs ,
es
,
te Von. legitatea de
control cu histereză se exprimă prin formula 6.2.
R(t) =
      
Kc, dacă Vact(t) <Von(t)
0, dacă Vact(t) >Vof f (t)
R(t− 1), altfel
(6.2)
6.1. Analiza domeniului de aplicare 85
unde Von(t) s
,
i Vof f (t) sunt pragurile de histereză la momentul t.
Grafic legitatea histereza reprezinta o dependenta a valorii de iesire R(t) de valoarea
concurenta cu tranzitii pe valorile Von(t) s
,
i Vof f (t) si va arata ca in figura 6.3 unde se vede
profilul de crestere a valorii de iesire care este diferit de cel de descadere.
Figure 6.3: Graficul legităt , ii histereză[6].
graficul de evolutie a valorii de isire controlat de mecanismul de reglare ON-OFF cu
histereză va arata ca in figura 6.4.
Figure 6.4: Graficul de funct , ionare al controlului ON-OFF cu histereză [6].
Iar rezultatele simularii in mediu real vor arata ca in figura 6.5.
Figure 6.5: Simularea controlului ON-OFF cu histereză [6].
Aceasta metoda previne comutările frecvente relaxand astfel uzura componentelor s ,
i
asigurând o funct ,
ionare mai stabilă a sistemului de control.
86 Lucrarea de laborator nr. 6. Reglare automata si in sistemele incorporate
6.1.2 Control PID
Metoda de control PID reprezinta o mentoda de reglare mai fina, care utilizeaza un
algoritmceajusteazăcontinuusemnaluldecontrolpebazaeroriidintrevaloareamăsurată
s
i valoarea de referint
ă.
,
,
Erroarea este definită ca o diferenta dintre valoare dorita Vdes(t) si cea actiala Vact(t),
reprezentata prin formula 6.3.
e(t) = Vdes(t)− Vact(t) (6.3)
Algoritmul PID combină trei componente principale fiecare dintre ele avand un rol
specific in procesul de reglare automata.
Componenta Proportionala - P contribuie la semnalul de iesire pentru actionare
cu o valoare proportionala cu eroarea masurata intre valoarea dorita Vdes(t) si cea ac-
tiala Vact(t), reprezentata prin formula 6.3. Semnalul de control proport ,
ional este dat de
formula 6.4.
P(t) = Kp· e(t) (6.4)
unde P(t) este componenta proport ,
ională a semnalului de control la momentul t, iar Kp
este câs
, tigul proport , ional, coeficient de ajustare a sensibilităt ,
ii sistemului la eroare.
Grafic componenta proportionala va arata ca in figura 6.6.
Figure 6.6: Graficul de funct , ionare al componentei proport , ionale[6].
iar simularile unde se poate observa impactul ajustarii coeficientului Kp vor arata ca
in figura 6.7.
Figure 6.7: Simularea componentei proport , ionale[6].
Dingrafics , isimulăriseobservăcă,pemăsurăcecoeficientulKp cres
, te, valoareaactuală
se apropie de cea dorită. Totus , i, o cres ,
tere excesivă a lui Kp poate duce la oscilat ,
ii în
jurul valorii de referint , ă, deoarece sistemul devine prea sensibil la eroare s ,
i react
ionează
,
prea agresiv. Reglarea doar cu componenta proport , ională nu permite atingerea exactă
a valorii dorite, iar cres ,
terea coeficientului accentuează oscilat
iile. Această metodă este
,
utilă pentru a aduce valoarea măsurată aproape de cea dorită, dar nu asigură eliminarea
completă a erorii.
6.1. Analiza domeniului de aplicare 87
Componenta Derivativa (D) anticipează tendint , a viitoare a erorii, oferind o corect ,
ie
bazată pe rata de schimbare a erorii. Semnalul de control derivativ este dat de formula
6.5.
de(t)
D(t) = Kd·
dt (6.5)
unde D(t) este componenta derivativă a semnalului de control la momentul t, iar de(t)
dt este
derivata erorii în timp, reprezentând rata de schimbare a erorii, Kd este câs
, tigul derivativ.
Pentru calculul numeric derivatei erorii se poate utiliza o aproximare discretă re-
porezentata de formula 6.6.
de(t)
≈
dt
e(t)− e(t− ∆t)
∆t (6.6)
Iar formula de control PD este exprimata de relatia 6.7.
de(t)
e(t)− e(t− ∆t)
R(t) = P(t) + D(t) = Kp· e(t) + Kd·
dt= Kp· e(t) + Kd·
∆t (6.7)
Impactul componentei derivativa poate fi observat in graficul din figura 6.8.
Figure 6.8: Graficul de funct , ionare al componentei derivativă [6].
Din grafic s , i simulări se observă că, adăugarea componentei derivative corectează di-
namica sistemului, reducând oscilat ,
iile s
, i stabilizând răspunsul. Totus , i, această compo-
nentă nu elimină eroarea pe termen lung, ci doar atenuează variat , iile rapide.
Componenta Integrală (I): Această componentă ia în considerare suma erorilor tre-
cute, oferind o corect , ie pentru erorile persistente care nu pot fi eliminate doar cu compo-
nenta proport , ională sau derivativă. Semnalul de control integral este dat de formula:
I(t) = Ki·
t
0
e(τ)dτ (6.8)
unde I(t) este componenta integrală a semnalului de control la momentul t, Ki este
câs
, tigul integral, iar e(τ) este eroarea la momentul τ.
Pentru calculul numeric al integralei erorii se poate utiliza o aproximare discretă
reprezentate de formula 6.9.
t
e(τ)dτ ≈
0
N
n=0
e(n· ∆t)· ∆t (6.9)
Grafic, componentaintegralăareroluldeaeliminaeroareastatică(offset-ul)carepoate
rămâne prezentă în sistem atunci când se utilizează doar componentele P sau PD. Aceasta
88 Lucrarea de laborator nr. 6. Reglare automata si in sistemele incorporate
cres
, te treptat act , iunea de control până când eroarea este eliminată complet. Impactul
componentei integrale poate fi observat in graficul din figura 6.9.
Figure 6.9: Simularea controlului PID complet (P+I+D) [6].
Ca si principiu componenta I colecteaza eroarea acumulata in timp si contribuie cu
un adaos la semnalul de control pentru a elimina eroarea statica. Totusi, o valoare prea
mare a coeficientului Ki poate duce la instabilitate s ,
i oscilat
, ii, deoarece sistemul devine
prea sensibil la erorile acumulate. Odata ce eroarea acumulata in timp devine 0 rezulta
ca Sistemul se mentine la valoare dorita fara a mai avea o eroare statica.
formula completa de control PID este exprimata de relatia 6.10.
R(t) = Kp· e(t) + Ki·
t
de(t)
e(τ)dτ + Kd·
0
dt (6.10)
unde R(t) este semnalul de control la momentul t, iar Kp,Ki,Kd sunt coeficient
ii de
,
reglare pentru fiecare componentă.
iar pentru calcul numeric se poate folosi relatia 6.11.
R(t) = Kp· e(t) + Ki·
N
n=0
e(n· ∆t)· ∆t+ Kd·
e(t)− e(t− ∆t)
∆t (6.11)
Reglarea coeficient ,
ilor Kp,Ki,Kd influent
, ează direct răspunsul sistemului: rapidi-
tate, stabilitate s ,
i eliminarea erorii statice. Aces
ti coeficient
,
, i pot fi determinat , i fie prin
metode teoretice din domeniul Ingineriei Reglării Automate (IRA), fie prin algoritmi it-
erativi de PID Tuning. Pentru aplicat , ii practice, se recomandă o metodă de reglare pas
cu pas (PID Tuning) us, or de aplicat, prezentată mai jos, fiecare pas fiind formulat astfel
încât să nu depăs , ească 80 de caractere pe rând:
• Setează valoarea dorită Vd s
i init
ializează coeficient
,
,
,
ii: Kp = 0, Ki = 0, Kd = 0.
• Cres
, te treptat Kp până când sistemul începe să oscileze (răspunsul devine instabil),
apoi reduce Kp la jumătate. Acum sistemul ar trebui să răspundă rapid, dar fără
oscilat
ii mari.
,
• Cres
,
te Kd put , in câte put , in, observând dacă oscilat ,
iile se reduc s
, i răspunsul devine
mai stabil. O valoare potrivită pentru Kd va atenua vârfurile s
i va face sistemul să
,
se stabilizeze mai repede.
• Cres
,
te Ki treptat, până când eroarea statică dispare (sistemul ajunge exact la val-
oarea dorită). Dacă apar oscilat, ii, reduce Ki la jumătate sau la o treime.
• Testează sistemul cu mai multe valori ale Vd pentru a verifica dacă răspunsul rămâne
stabil s
, i rapid.
Astfel, reglarea se face experimental, observând direct comportamentul sistemului la
fiecare pas.
6.2. Sarcina de laborator 89
• Setează valoarea dorită Vd s
i init
ializează coeficient
,
,
,
ii: Kp = 0, Ki = 0, Kd = 0.
• Cres
, te treptat Kp până când sistemul începe să oscileze (răspunsul devine instabil),
apoi reduce Kp la jumătate. Acum sistemul ar trebui să răspundă rapid, dar fără
oscilat
ii mari.
,
• Cres
,
te Kd put , in câte put , in, observând dacă oscilat ,
iile se reduc s
, i răspunsul devine
mai stabil. O valoare potrivită pentru Kd va atenua vârfurile s
i va face sistemul să
,
se stabilizeze mai repede.
• Cres
,
te Ki treptat, până când eroarea statică dispare (sistemul ajunge exact la val-
oarea dorită). Dacă apar oscilat, ii, reduce Ki la jumătate sau la o treime.
• Testează sistemul cu mai multe valori ale Vd pentru a verifica dacă răspunsul rămâne
stabil s
, i rapid.
Astfel, reglarea se face experimental, observând direct comportamentul sistemului la
fiecare pas.