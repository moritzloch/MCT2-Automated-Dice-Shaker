# Automated Dice Shaker
***
Project for "Introduction to Microcomputers 2" by Marco Schweizer & Moritz Loch

***
## Konzept

Die Entwicklung des Konzeptes f�r dieses Projekt erfolgte mithilfe von ChatGPT und dem Prompt "Lustige Arduino Projekte". Die Idee eines automatisierten W�rfelbechers wurde zum "M�xle-Automat" erweitert, ein Automat der das beliebte W�rfel-(Trink-)spiel spielen kann.
In der urspr�nglichen Planung war angedacht, das automatisierte W�rfeln durch eine rotierende Grundplatte zu realisieren. Der W�rfel sollte durch den Drehimpuls gew�rfelt werden. Zur Erkennung des W�rfelergebnisses sollte ein Farbsensor die Farbe eines Farbw�rfels registrieren.

Zu Beginn des Projektes zeigte sich, dass der TCS3200 Farbsensor nicht zuverl�ssige die Farbe des Farbw�rfels identifizieren kann. Grund daf�r ist unter anderem, dass keine gleichm��ige Belichtung des W�rfels sichergestellt werden konnte und die Farben des W�rfels zu nah beieinander liegen, dass h�ufige Verwechslungen auftraten.
Deshalb wurde entschieden, Software und Hardware separat zu entwickeln. Beide Komponenten sind so aufeinander abgestimmt, dass eine zuk�nftige zusammenf�hrung problemlos ist. Die notwendige Schnittstelle dazwischen, ein anderer Sensor ist zuk�nftig zu Betrachten.

***
## Software

Die Software zum Projekt umfasst eine Finite State Machine (FSM) mit 26 Zust�nde. Diese erm�glicht eine umfangreiche Men�steuerung, sowie die Konfiguration und Durchf�hrung von drei verschiedene Spielmodi. Die Informationsausgabe erfolgt mit einem 16x2 LCD-Display. Der Spieler kann durch Drehung eines Rotary Encoders Werte ausw�hlen. Zustands-bergh�nge geschehen bei Knopfdruck des Rotary Encoders. Ein weiterer Knopf setzt den Zustandsautomaten zur�ck.
Der Zustandsautomat ist durch ein Switch-Case-Statement implementiert. Dabei besitzt jeder Zustand einer eigene Funktion. In dieser ist sowohl die auszuf�hrende Aktivit�t als auch der �bergang zum n�chsten Zustand geregelt. 

Ausgangspunkt des Programms bildet das Hauptmen� zur Auswahl der Spielmodi. Dieses und alle weiteren Scrollmen�s sind als Funktion 
`uint8_t lcdScrollMenu(MenuProperties* menuProperties, const char** menuItemNames, bool* firstFrame)`
implementiert. Diese erm�glicht die Darstellung eines beliebigen Scrollmen�s.

Vom Hauptmen� aus gelangt der Spieler in die Nebenspielmodi "W�rfeln" und "Noten w�rfeln". Beide k�nnen durch Einstellung der W�rfel- und Augenzahl bzw. der maximalen Bestnote konfiguriert werden. Alle Spieleinstellungen und -daten werden in der Struktur
[Code von unten hier rein]

Anschlie�end liefert das Programm mit jedem Knopfdruck ein neues W�rfelergebnis bzw. eine neue Note.

Bei Auswahl des Hauptspielmodus "M�xle" ist zun�chst die Auswahl der Anzahl von Leben und die Anzahl der Spieler, die neben der CPU am Spiel teilnehmen. Bei Spielbeginn wird zuf�llig ein Spieler ausgesucht, der beginnt.
Menschliche Spieler geben ihre W�rfelsumme mittels Drehencoder ein. Die CPU w�rfelt zuf�llig. Hier kann zuk�nftig das automatisierte W�rfeln eingebunden werden. 
Der n�chste Spieler entscheidet anschlie�end mit einem Auswahlmen� ober dem vorhergehenden Spieler glaubt oder nicht. Muss die CPU entscheiden, so tut sie das zuf�llig. Falls dem w�rfelndem Spieler nicht geglaubt wird muss dieser nun aufdecken, ob er tats�chlich gelogen hat. Andernfalls beginnt ein neuer Spielzug. Ist der w�rfelnde Spieler die CPU, dann wird dieser Schritt �bersprungen. Zum Schluss wird dem l�genden oder dem falsch ratenden Spieler ein Leben abgezogen.
Ein abschlie�ender Zustand �berpr�ft ob das Spiel im vergangen Spielzug bereits gewonnen wurde und setzt die Spielzugdaten zur�ck. Im Falle des gewonnen Spiels wird der Sieger auf dem Display ausgegeben und das Spiel kehrt ins Hauptmen� zur�ck. Andernfalls beginnt der Spielzug des n�chsten Spielers.

Der vollst�ndige Zustandautomat ist hier zu sehen.
[Zustandsautomat]




```
struct FsmProperties{
    bool stateTransition = false;
    bool firstFrame = true;
    state_t currentState = ST_MENU;
    state_t nextState = ST_MENU;
    gameMode_t gameMode;

    int8_t numberOfLives;
    int8_t numberOfPlayers;
    uint8_t lifeCount[9];
    uint8_t currentPlayer;
    uint8_t nextPlayer;
    int8_t prevDiceRollIndex;
    int8_t diceRollIndex;
    bool trueNumberAnnounced;
    int8_t winningPlayer = -1;

    int8_t numberOfDice;
    int8_t numberOfEyes;

    float maxGrade = DEFAULTMAXGRADE;

    bool autoDiceRoll = DEFAULTAUTODICEROLL;
  };
  static FsmProperties fsm;
  ````
***
## Hardware

Das Projekt greift auf eine Vielzahl an Schnittstellen und Modulen zur�ck. Die Hauptkomponenten f�r die Funktion der Software sind Display und Rotary Encoder, beides wurde in ein Bedientableau eingebettet zur komfortableren Handhabung.


Neben den Bedien- und Anzeigeelementen (LCD-Display, Rotary Encoder und Taster), lag der Fokus vor allem auf dem voll- und halbautomatischem W�rfelsystem. Bestehend aus 2 Servos und einer Menge an selbst entworfenen Bauteilen wurde das System von Grund auf entwickelt und kann mit einer Ausreichenden Zuverl�ssigkeit auf Anforderung oder, falls in den Spieleinstellungen festgelegt, selbstst�ndig w�rfeln. 

Das System besteht aus 3 Bereichen: Dem W�rfelturm, dem Auffangbereich und dem Hubarm.
Der W�rfelturm besteht aus 2 schr�g angeordneten Platten, welche den W�rfel ablenken, wodurch dieser eine zuf�llige Flugbahn annimmt. Gespei�t wird er durch den Hubarm. Dieser wiederrum erh�lt den W�rfel durch einen Servo-Motor aus dem Auffangbereich, welcher so geformt ist, dass der W�rfel bei der Landung oder sp�testens bei der Bef�rderung durch den Servo mittig platizert wird, sodass dieser stets in den Beh�lter am Hubarm gelangt. Durch Toleranzen und Unperfektheiten kann der W�rfel quer stehen oder nicht beim ersten Mal in den Beh�lter passen, weshalb durch Test herausgefunden wurde, dass durch Wiederholen der Servobewegung die Zuverl�ssigkeit wesentlich erh�ht werden konnte, weshalb dies im finalen Code implementiert wurde. Befindet sich der W�rfel im Beh�ltnis des Hubarmes, ist dies der Verweilzustand (auch Ausgangszustand), in welcher der Spieler das Ergebnis vom W�rfel ablesen kann. Der Hubarm ist so designed, dass sich der W�rfel mitsamt Beh�ltnis bis zu einem Winkel von circa 150� lotrecht mitbewegt und erst kurz davor beginnt zu kippen, sodass der W�rfel in den Turm geworfen wird.
***