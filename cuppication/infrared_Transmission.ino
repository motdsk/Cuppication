///////////////////////////////////////// データの送信を赤外線LEDで行う処理
void SendData(int num,char data[]){
  onLED();//送信開始にLEDリボンを光らせる処理
  int i ;
  // リーダ部を送る
  PalseHigh(READ_H_USEC) ;
  delayMicroseconds(READ_L_USEC) ;
  // データを送る
  for (i=0 ; i<num ; i++) {
    PalseHigh(HIGH_USEC) ;
    if (*data == '1') delayMicroseconds(ON_USEC) ;
    else              delayMicroseconds(OFF_USEC) ;
    data++ ;
  }
  // ストップデータを送る
  PalseHigh(HIGH_USEC) ;
  delayMicroseconds(STOP_USEC) ;
  offLED();//送信成功したらLEDリボンを消す処理
  DspData(num,data) ;         // 表示する
 // Serial.println("Was transmissions.") ;  
}
//  PalseHigh() - ＨＩＧＨのパルスを作る
void PalseHigh(int cnt)
{
  int i ;

  tone(SND_PINNO, 37900) ; // 37.9KHzのON開始
  delayMicroseconds(cnt) ;
  noTone(SND_PINNO) ;      // 37.9KHzの終了

}



