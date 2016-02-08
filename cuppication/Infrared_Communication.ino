///////////////////////////////// 赤外線を受信する処理
void ReceiveData() {
  unsigned long t ;
  int i , cnt ;

  //Serial.println("Please press the remote control.") ;
  // 受信するまでループ
  while(1) {
    t = 0 ;
    if (RCV_PIN == LOW) {
      // リーダ部のチェックを行う
      t = micros() ;                     // 現在の時刻(us)を得る
      while (RCV_PIN == LOW) ;           // HIGH(ON)になるまで待つ
      t = micros() - t ;                 // LOW(OFF)の部分をはかる
    }
    // リーダ部有りなら処理する(3.4ms以上のLOWにて判断する)
    if (t >= 3400) {
      i = 0 ;
      while(RCV_PIN == HIGH) ;           // ここまでがリーダ部(ON部分)読み飛ばす
      // データ部の読み込み
      while (1) {
        while(RCV_PIN == LOW) ;       // OFF部分は読み飛ばす
        t = micros() ;
        cnt = 0 ;
        while(RCV_PIN == HIGH) {      // LOW(OFF)になるまで待つ
          delayMicroseconds(10) ;
          cnt++ ;
          if (cnt >= 1200) break ; // 12ms以上HIGHのままなら中断
        }
        t = micros() - t ;
        if (t >= 10000) break ;                      // ストップデータ
        if (t >= 1000)  IRbitData[i] = (char)0x31 ;  // ON部分が長い
        else            IRbitData[i] = (char)0x30 ;  // ON部分が短い
        i++ ;
      }
      // データ有りなら内容を保存
      if (i != 0){
        IRbitData[i] = 0 ;
        IRbitLen     = i ;
        DspData(IRbitLen,IRbitData) ;         // 表示する
        
        //リクエスト信号だった場合自身の識別番号を送信する処理に移行
        //それ以外の場合はひたすら受信する
        //すべて0だった場合リクエスト判定
        boolean reqest = false;
        int cnt = 0;
        for(int i=0;i<IRbitLen;i++){
          if(IRbitData[i]==(char)0x30){
          cnt++;
          }
        }
        if(cnt==ReqestbitLen) {
          reqest=true;
        }
        if(reqest){
          Serial.println("Request received");          //リクエストを受け取りました
          break ;
        }
        else{
          Serial.println("Was received.") ;            // 受信しました
          //break ;
        }
      }
    }
  }
}

// 受信データをシリアルモニタ画面に送り内容を表示させる処理
void DspData(int num,char *data){
  int i , j , x , dt ;

  Serial.print(data) ;              // ビットデータで表示
  Serial.write(" ( ") ;
  x = num / 8 ;
  // ビット文字列データから数値に変換する
  for (j=0 ; j < x ; j++) {
    dt = 0 ;
    for (i=0 ; i < 8 ; i++) {
      if (*data++ == 0x31) bitSet(dt,i) ;
    }
    Serial.print(dt,HEX) ;       // ＨＥＸ(16進数)で表示
    Serial.write(' ') ;
  }
  Serial.println(')') ;
}






///////////////////////////////////////// データの送信を赤外線LEDで行う処理
void SendData(int num,char *data){
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
}
//  PalseHigh() - ＨＩＧＨのパルスを作る
void PalseHigh(int cnt)
{
  int i ;

  tone(SND_PINNO, 37900) ; // 37.9KHzのON開始
  delayMicroseconds(cnt) ;
  noTone(SND_PINNO) ;      // 37.9KHzの終了

}




