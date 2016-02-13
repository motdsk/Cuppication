
//赤外線LED,赤外線受信モジュールの初期設定
// アナライザーで表示させた   実際の送信時間(リモコンからの受信時間)
#define ON_USEC      1138  // 1.212-1.235ms(1.235-1.237)   (1.285-1.287)
#define OFF_USEC      304  // 367.5-390us(395-397.5)       (412.5-415)
#define STOP_USEC    1500  // SHARP-TV                     Panasonic-light
#define READ_H_USEC  1605  // 3.44ms(3.45)                (3.505)
#define READ_L_USEC  1560  // 1.635ms(1.662)              (1.722)
#define HIGH_USEC     206  // 452-478us(442.5-470)        (455-457.5)
#define SND_PINNO    9     // 赤外線送信LED接続ピン番号  
#define RCV_PINNO    8    // 赤外線受信モジュール接続ピン番号          ピン変えるならここと
#define RCV_PIN    (PINB & 0x1)    // デジタル8番ピン(PORTB:PB0)                ここ
#define one         (char)0x31     //16進数の0と１を定義
#define zero         (char)0x30


char IRbitData[64] ;       // 受信データを格納する変数('0'/'1'で格納)
int  IRbitLen ;            // 受信データの長さ
char MybitData[8] = {
  one,one,zero,zero,zero,zero,zero,zero}; //自身の個体識別データを定義
int  MybitLen = 8 ;             //自身の個体識別データの長さを定義
char ReqestbitData[8] = {
  zero,zero,zero,zero,zero,zero,zero,zero}; //リクエスト信号を定義(おーる０)
int  ReqestbitLen = 8 ;             //リクエスト信号の長さを定義
int YourNum=0;               //受信したコードの番号を10進数で保存する変数
int MyNum =0;                //自身の番号を１０進数で保存する変数


char AggregatebitData[128] ;       // 受信データを集約して格納する変数('0'/'1'で格納)
int  AggregatebitLen ;            // 集約データの長さ
int lastlen=0;
//最初に１度だけ実行される関数
void setup(){
  Serial.begin(9600) ;               // シリアル通信の初期化
  pinMode(RCV_PINNO,INPUT) ;         // 赤外線受信モジュール
  pinMode(SND_PINNO,OUTPUT) ;        // 赤外線送信LED
  IRbitLen = 0 ;
  LEDinit() ;      ///LEDの初期設定
  MyNum=bitData2int(MybitData);
}


// メインの処理関数
//受信と送信をまったく同時に行うことはできないため、自信が発した赤外線を
//自信のIRモジュールで確認することは不可能。今のところ実験にはarduino2つ必要
void loop(){

  //実験用コード。リクエスト信号をのせた赤外線を送信する。
  SendData(ReqestbitLen,ReqestbitData) ;  // リクエスト送信する
  Serial.println("reqest transmissions.") ;
  ReceiveData(5000) ;    // 5秒間受信する
 // SendData(AggregatebitLen,AggregatebitData) ;  // 集約したデータを送信する。*この行は前の行で5秒間の間に受信した識別データをまとめてグルー日情報に変換、周りに送信する。
  DspData(AggregatebitLen,AggregatebitData) ; 
  Serial.println("aggregated") ;
  ReceiveData(10000);   //次のターンまで受信
}



