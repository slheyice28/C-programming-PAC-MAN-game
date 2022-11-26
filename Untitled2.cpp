#include<iostream>
#include<conio.h>
#include<dos.h> 
#include <windows.h>
#include <time.h>
#include<MMSystem.h>// muzik ekleyebilmek icin bu kutuphaneyi kullaniriz.
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 27
#define WIN_WIDTH 70 

using namespace std; // program boyunca butun cout fonksiyonlarina erisilmesine olanak saglar.

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);//yanip sonen imleci Devc ye tanitir ve belirtilen koordinatlari kullanir.
COORD CursorPosition;

int enemyY[3];//kullanilan dusman karakterin y eksenindeki boyutunu belirtir.
int enemyX[3];//kullanilan dusman karakterin x eksenindeki boyutunu belirtir.
int enemyFlag[3];//tusa basmakla karakter yonlendirmek icin kullanilan fonksiyondur.
char car[7][7] = { ' ','±','±','±','±','±',' ',// kullanilan matrisin icine oyunda kullanacagimiz karakter cizilir.
                   ' ','±',' ','±',' ','±',' ',
                   ' ','±','±','±','±','±',' ',
                   ' ','±',' ','±',' ','±',' ',
				    }; 
                   
int carPos = WIN_WIDTH/2;// karakterin baslangic pozisyonunu belirtir.
int score = 0; //baslangic skorununun 0 oldugunu belirtir.
void gotoxy(int x, int y){//istenilen konsoldaki alana gitmek icin kullanilan x ve y  parametrelerini bulundurur.
   CursorPosition.X = x;// y parametresindeki imlec konumudur.
   CursorPosition.Y = y;// x parametresindeki imlecin konumudur.
   SetConsoleCursorPosition(console, CursorPosition);//konsoldaki imlecin konumunu ayarlamak icin kullanilir.
}
void setcursor(bool visible, DWORD size) {// imlecin kelime boyutunu ayarlamak icin kullanilir.
   if(size == 0)
       size = 20;    
   
   CONSOLE_CURSOR_INFO lpCursor;  // konsol imleci hakkinda bilgi icerir.  
   lpCursor.bVisible = visible;// imlecin gorunurlugu
   lpCursor.dwSize = size;//imlec tarafindan doldurulan karakter hucresi yuzdesini verir.
   SetConsoleCursorInfo(console,&lpCursor);
}
void drawBorder(){  //konsolda sinir cizmek icin kullanilir.
   for(int i=0; i<SCREEN_HEIGHT; i++){//ekran yuksekligini arttirmak icin kullanilan dongudur.
       for(int j=0; j<15; j++){
           gotoxy(0+j,i); cout<<"±";//yanip sonen imlecin konsolda doldurulmasi icin kullanilir.
           gotoxy(WIN_WIDTH-j,i); cout<<"±";
       }
   } 
   for(int i=0; i<SCREEN_HEIGHT; i++){// engellerden kacindikca ekran yuksekliginin arttigini belirten dongudur.
       gotoxy(SCREEN_WIDTH,i); cout<<"±";
   } 
}
void genEnemy(int ind){// karsilasilan dusman karakterin indisini belirtir.
   enemyX[ind] = 15 + rand()%(33); //dusman karakterini x ekseninde rastgele  ortaya cikarama islemidir. 
}
void drawEnemy(int ind){// dusman karakteri konsola cizdirme islemidir.
   if( enemyFlag[ind] == true ){// karakterin yonlendirme indisi dogru ise konsala cizer.
       gotoxy(enemyX[ind], enemyY[ind]);   cout<<" ±±±±±± ";  // dusman karakterin konsola cizim seklini belirtir.
       gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"±±±±±±±±"; 
       gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"± ±±±±±±"; 
       gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"±±±  ±±±";  
       gotoxy(enemyX[ind], enemyY[ind]+4); cout<<" ±   ±±"; 
   } 
}
void eraseEnemy(int ind){// gerimizde kalan dusman karakterin silinmesi islemidir.
   if( enemyFlag[ind] == true ){// dusman karakterin yönlendirme indisi dogru ise karakteri konsoldan siler.
       gotoxy(enemyX[ind], enemyY[ind]); cout<<"        ";  
       gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"        "; // dusman karakter konsoldan silinmeye baslar.
       gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"        "; 
       gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"        "; 
       gotoxy(enemyX[ind], enemyY[ind]+4); cout<<"        ";
    
  
   } 
}
void resetEnemy(int ind){//dusman karakterin indisini sifirlama islemidir.
   eraseEnemy(ind);//dusman karakterin indisini silme islemidir.
   enemyY[ind] = 1;
   genEnemy(ind);// dusman karakterin y eksenindeki indisidir.
}

void drawCar(){//karsimiza cikan dusmani konsola cizmek icin kullanilan dongudur.
   for(int i=0; i<7; i++){
       for(int j=0; j<7; j++){
           gotoxy(j+carPos, i+22); cout<<car[i][j];
       }
   }
}
void eraseCar(){//carpmadan ekarte ettigimiz dusmani konsoldan silmek icin kullanilan dongudur.
   for(int i=0; i<7; i++){
       for(int j=0; j<7; j++){
           gotoxy(j+carPos, i+22); cout<<" ";
       }
   }
}

int collision(){//karakterimizin dusman karakterle carpismasi sonucundaki durumu belirtir.
   if( enemyY[0]+4 >= 23 ){
       if( enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9  ){// karakterimizin ve dusman karakterin ayni anda ayni bolgede bulunmasi durumu carpisma olur.
           return 1;
       }
   }
   return 0;
} 
void gameover(){//eger 3 kere ust uste dusman karakterle carpisilirsa oyun biter.
   system("cls");
   cout<<endl;
   cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl;
   cout<<"\t\t±±±±±±±±±±± OYUN BITTI ±±±±±±±±±±±±±±±±"<<endl;//ekrana OYUN BITTI yazisi yazdirmak icin kullanilir.
   cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl<<endl;
   cout<<"\t\t MENUYE DONMEK ICIN HERHANGI BIR TUSA BASIN.";// kullaniciya MENUYE nasil donecegini ekrana yazdirarak gosterir.
   getch();
}
void updateScore(){// kullanicinin anlik olarak guncel skorunu olusturur.
   gotoxy(WIN_WIDTH + 7, 5);cout<<"ANLIK SKOR: "<<score<<endl;
}

void instructions(){//oyun nasil oynanacagi ile ilgili bilgilerin yazili oldugu talimatlar menusudur.
   
   system("cls");
   cout<<"           TALIMATLAR              ";
   cout<<"\n----------------";
   cout<<"\n Amblemi Sol veya sag hareket ettirin  ";
   cout<<"\n\n 'a' sol tarafa hareket icin kullan";
   cout<<"\n 'd' sag tarafa hareket icin kullan";
   cout<<"\n cikmak icin esc tusuna basin";
   cout<<"\n\nmenuye dunmek icin herhangi bir tusa basin";
   getch();
}

void play(){
   carPos = -1 + WIN_WIDTH/2;
   score = 0;// skoru 0 olarak baslatma islemidir.
   enemyFlag[0] = 1;// dusman karakterin 1 veya 0 olmasýna gore ekrana gelmesi islemidir.
   enemyFlag[1] = 0;
   enemyY[0] = enemyY[1] = 1;//dusman karakterin 1 ve 0 olarak her iki durumda da 1 durumuna gider ve dusman karakter karakterimizin karsisina cikar.
     

     
   system("cls"); 
   drawBorder(); // sinir cizme islemidir.
   updateScore();//kullanicinin dusman karakterleri gecerek skorunu yukseltmesi islemidir.
   genEnemy(0);
   genEnemy(1);
   
   gotoxy(WIN_WIDTH + 7, 2);cout<<"PAC-MAN'DAN KACIS";// oyunumuzun adidir ve imlec genisligine gore ekranda yazar. 
   gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";//ekranimizdaki yazilar arasindaki siniri belli eden susleme  karakteridir.
   gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";//ekranimizdaki yazilar arasindaki siniri belli eden susleme  karakteridir.
   gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";//ekranimizda oyun kontrollerinin nasil yapilacagi hakkinda kullaniciya bilgi verir.
   gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
   gotoxy(WIN_WIDTH + 2, 14);cout<<" A TUSU - SOL";// ekranda A TUSU-SOL diye yazar.
   gotoxy(WIN_WIDTH + 2, 15);cout<<" D TUSU- SAG"; //ekranda D TUSU-SAG diye yazar.
   
   gotoxy(18, 5);cout<<"HERHANGI BIR TUSA BASIN";
   getch();
   gotoxy(18, 5);cout<<"                      ";
   
   while(1){//true(1) olarak asagidaki durumlari doner
       if(kbhit()){
           char ch = getch();
           if( ch=='a' || ch=='A' ){//A ve a tuslarina kullanici bastiginda ayni islemi yapmasi gerektigini belirten islemdir.
               if( carPos > 18 )//eger karakterin pozisyonu 18 den buyukse -4 deger alarak yeniden olusur.
                   carPos -= 4;
           }
           if( ch=='d' || ch=='D' ){//D ve d tuslarina kullanici bastiginda ayni islemi yapmasi gerektigini belirten islemdir.
               if( carPos < 50 )//eger karakterin pozisyonu 50 den kucukse +4 deger alarak yeniden olusur.
                   carPos += 4;
           } 
           if(ch==27){//eger 27 ye esit olursa dongu kirilir donguden cikar.
               break;
           }
       } 
       
       drawCar();//dusman karakter cizmek islemine yarar. 
       drawEnemy(0); //dusman karakterin cizilmesi icin true(1) ya da false (0) olmasi gerekir.
       drawEnemy(1); //dusman karakterin cizilmesi icin true(1) ya da false (0) olmasi gerekir.
       if( collision() == 1  ){//true(1) durumuna gelirse dusman karakterle carpisma meydana gelir.
           gameover();// oyun biter.
           return;
       } 
       Sleep(50);//her dongunun bitiminde 50 milisaniyelik yeni tur icin hiz kazandirma islemidir.
       eraseCar();//dusman karakterleri silme islemine yarar.
       eraseEnemy(0);//dusman karakterin silinme islemi icin true(1) ya da false(0) olmasi gerekir her iki durumda da dusman karakter silinir.
       eraseEnemy(1); //dusman karakterin silinme islemi icin true(1) ya da false(0) olmasi gerekir her iki durumda da dusman karakter silinir.  
       
       if( enemyY[0] == 10 )// dusman karakterin y eksenindeki durumunu belirtir.
           if( enemyFlag[1] == 0 )//dusman sistemi true(1) ya da false (0) islemine gore calisir.
               enemyFlag[1] = 1;//dusman sistemi true(1) ya da false (0) islemine gore calisir.
       
       if( enemyFlag[0] == 1 )//dusman karakterin false durumunda dusman karakterin y eksenindeki boyutu artar.
           enemyY[0] += 1;//dusman karakterin false durumunda dusman karakterin y eksenindeki boyutu artar.
       
       if( enemyFlag[1] == 1 )//dusman karakterin true durumunda dusman karakterin y eksenindeki boyutu artar
           enemyY[1] += 1;
        
       if( enemyY[0] > SCREEN_HEIGHT-4 ){// dusman karakterin y eksenindeki boyutu ekranin yuksekliginden buyukse yukseklige -4 deger eklenir boylece asagi hareket eder.
           resetEnemy(0);// dusman karakterin y eksenindeki boyutu ekranin yuksekliginden buyukse yukseklige -4 deger eklenir boylece asagi hareket eder.
           score++;//kullanicinin kazandigi skor artar.
           updateScore();//kullanicinin kazandigi skor artar.
       }
       if( enemyY[1] > SCREEN_HEIGHT-4 ){// dusman karakterin y eksenindeki boyutu ekranin yuksekliginden buyukse yukseklige -4 deger eklenir boylece asagi hareket eder.
           resetEnemy(1);// dusman karakterin y eksenindeki boyutu ekranin yuksekliginden buyukse yukseklige -4 deger eklenir boylece asagi hareket eder
           score++;//kullanicinin kazandigi skor artar.
           updateScore();//kullanicinin kazandigi skor artar.
       }
   }
}

int main()
{
	
   PlaySound(TEXT("muzik.wav"),NULL,SND_SYNC| SND_ASYNC);// arka planda muzik calmasini saglar.
  
  
  
   setcursor(0,0); 
   srand( (unsigned)time(NULL)); 
    
   do{
       system("cls");// oyun ile ilgili MENU dur.ekranda belirecek olan seceneklerden birini secerek yapilmasi istenen eylemin secilmesi saglanir .
       gotoxy(10,4); cout<< "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"; 
       gotoxy(10,5); cout<< "±±                                                                                          ±±"; 
       gotoxy(10,6); cout<< "±±                              PAC-MAN'DAN KACIS                                           ±±"; 
       gotoxy(10,7); cout<< "±±                                                                                          ±±";
       gotoxy(10,8); cout<< "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"; 
       gotoxy(10,9); cout<< "±±                            - Yarisa baslamak icin 1'e basin                              ±±";
       gotoxy(10,10); cout<<"±±                            - Talimatlara gozatmak icin 2'ye basin                        ±±";     
       gotoxy(10,11); cout<<"±±                            - Cikis icin 3'e basin                                        ±±";
       gotoxy(10,12); cout<<"±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"; 
       gotoxy(10,13); cout<<"±±                                                                                          ±±";
       gotoxy(10,14); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,15); cout<<"±±                                         HIKAYE                                           ±±"; 
       gotoxy(10,16); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,17); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,18); cout<<"±±    ISTANBULUN KADIM SAVASINDA HAYALETLER OLARAK YARDIMA IHTIYACIMIZ VAR                  ±±"; 
       gotoxy(10,19); cout<<"±±    KONTROL SENIN ELINDE ONUNE GELECEK ENGELLERI TEK TEK ASMALI VE EN YUKSEK SKORU SEN    ±±"; 
       gotoxy(10,20); cout<<"±±    ALMALSIN                                                                              ±±"; 
       gotoxy(10,21); cout<<"±±    ILERLEYEN ZAMANLARDA YENI VE DAHA GUCLU DUSMANLAR (PAC-MANS) SENI BEKLIYOR OLACAK...  ±±"; 
       gotoxy(10,22); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,23); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,24); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,25); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,26); cout<<"±±                                OYUNDA EMEGI GECENLER                                     ±±"; 
       gotoxy(10,27); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,28); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,29); cout<<"±±                                 MEHMET SALIH EYICE                                       ±±"; 
       gotoxy(10,30); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,31); cout<<"±±                                                                                          ±±"; 
       gotoxy(10,32); cout<<"±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±"; 
       char op = getche();
       
       if( op=='1') play();//klavyeden 1 tusuna basarsaniz oyun baslar.
       else if( op=='2') instructions();// klavyeden 2 tusuna basarsaniz talimatlar ekrana gelir.
       else if( op=='3') exit(0);// klavyeden 3 tusuna basarsaniz oyundan cikarsiniz.
       
   }while(1);//true (1) olarak dongu doner.
   
   return 0;
}

