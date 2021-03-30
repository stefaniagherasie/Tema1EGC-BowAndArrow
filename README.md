# Tema1EGC-BowAndArrow
[Tema1 - Elemente de grafica pe calculator] 

Tema presupune implementarea unei variante simplificate a jocului Bow and Arrow. <br>

      Framework complet: https://github.com/UPB-Graphics/Framework-EGC
      Enunt: https://ocw.cs.pub.ro/courses/egc/teme/2020/01

      Arcul poate fi controlat prin:
            - W, S - deplasare sus-jos pe axa Oy
            - Directia de tragere a arcului se poate ajusta prin miscarea mouse-ului
            - Arcul va fi mereu rotit astfel incat sageata se va deplasa catre pozitia curenta a mouse-ului.
            - Viteza de deplasare a sagetii este determinata de timpul de apasare al butonului mouse-ului.

#### IMPLEMENTARE

Am inceput implementarea prin crearea obiectelor din joc. Pentru crearea arcului
am considerat triunghiuri care prin unire sa creeze cercul. Am aflat unghiul pe care
l-ar avea un triungi pentru o impartire a cercului in numTriangles. Apoi am considerat
punctul (x, y) ca fiind initial si am calculat restul de puncte aplicand rotirea 
acestuia cu unghiul gasit. S-au luat doar punctele care se afla de partea pozitiva 
a axei Ox pentru a aparea doar un semicerc. Pentru crearea sagetii am folosit o linie 
si un triunghi dispus la capatul liniei.

Pentru crearea balonului am procedat in mod asemanator, luand tot cercul de aceasta 
data si formand si codita balonului, care e o polilinie. La randarea balonului,
am aplicat o scalare pe Oy pentru a obtine aspectul alungit al balonului. Pentru 
crearea suriken-ului am dat coordonatele unor puncte care formeaza exact triunghiurile
in modul dorit, urmand sa le unesc pentru a obtine forma suriken-ului.

Pentru crearea PowerBar-ului si al ScoreBar-ului am folosit un patrat care este scalat
ulterior, iar pentru aratarea vietilor ramase am folosit un patrat rotit, obtinand
un romb, care urmeaza sa fie usor scalat.

Arcul este pozitionat la (0, 360) initial, putand fi miscat in sus si in jos pe axa
Oy la apasarea tastelor W sau S. Acesta se roteste dupa pozitia mouse-ului.
Sageata este si ea pozitionata ca arcul si se poate roti dupa mouse. Unghiul este 
calculat in SetAngleArrow() folosind functia arctan in triunghiul dreptunghic format
de coordonatele sagetii si mouse-ului. Sageata se poate elibera la apasarea pe mouse,
viteza ei crescand cand se apasa mai mult pe mouse. Pentru miscarea sagatii dupa 
pozitia mouse-ului se creste tx-ul de la translatie, ty-ul fiind tx*tan(angleArrow),
astfel cei 2 factori crescand uniform si sageata pastrandu-si orientarea.

Pentru suriken si balon se realizeaza miscarea specifica, testandu-se constant 
coliziunea cu varful sagetii sau iesirea din cadru. La coliziune cu sageata, obiectele
se scaleaza cu un factor de scalare are scade pana la o anumita limita, dand impresia
ca obiectul dipare cand e lovit. Pentru a creea un joc mai dinamic, am atribuit 
5/6 pozitii posibile de aparitie a unui obiect, care sa se aleaga random si
baloanele/suriken-ele sa apara la positii diferite. La baloane, pe pozitiile
pare in vector se afla baloanele galbene, care scad scorul. In rest se afla baloane
rosii, care maresc scorul.

PowerBar-ul se scaleaza pe axa Ox dupa scalePowerBar care creste cand se mentine 
apasat pe mouse pana la o anumita linie. In mod asemanator se scaleaza si ScoreBar-ul,
tinandu-se cont de coliziunile dintre obiecte si daca scalarea de disparitie a 
acestora s-a terminat. Pentru LifeStatus se contorizeaza cate vieti mai are 
jucatorul, disparand cate un romb cand un suriken loveste arcul.

Coliziunile dintre sageata si obiecte tinta se face punct-cerc intre varful sagetii
si cercul incadrator al obiectului. Pentru coliziunea suriken-arc se verifica centrele
incadratoare ale celor 2 obiecte.

Cand jucatorul pierde toate cele 3 vieti, jocul se incheie prin inchiderea ferestrei si 
afisarea in consola a mesajului "Game over" si a scorului obtinut.
