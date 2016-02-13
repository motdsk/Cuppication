///////////////////////////////// 赤外線を受信する処理
void ReceiveData(unsigned long changeTime , boolean matome) {
  unsigned long t ;
  int i , cnt ;
  lastlen=16;
  AggregatebitLen=16;  //集約データを初期化
  long beforeTime = millis();    //この時点での時刻を記録
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
        DspData(IRbitLen,IRbitData);         // 表示する
        receive_Cognition(IRbitLen,IRbitData,matome); //リクエスト信号なら返す、それ以外ならただ保存するという処理
      }
    }
    //受信開始してからchangeTimeミリ秒経過したらループ抜けする
    if(millis()-beforeTime > changeTime){
      break;
    }
  }
}
// 受信データをシリアルモニタ画面に送り内容を表示させる処理
void DspData(int num,char data[]){
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
  // Serial.print(MyNum) ;
  Serial.println(')') ;
}


void receive_Cognition(int num,char data[],boolean matome){
  //すべて0だった場合リクエスト判定
  //リクエスト信号だった場合自身の識別番号を送信する処理に移行
  //それ以外の場合はひたすら受信する
  //最初の１バイトがFFだったばあいグループ情報として処理
  boolean reqest = false;
  boolean group = false;
  int cnt0 = 0;
  int cnt1 = 0;
  for(int i=0;i<8;i++){
    if(data[i]==(char)0x30){//0または1の数をカウントする
      cnt0++;
    }
    else{
      cnt1++;
    }
  }

  if(cnt0==ReqestbitLen && num==ReqestbitLen) {//内容がリクエストと一致したらリクエスト判定
    reqest=true;
  }
  if(cnt1==8){  //グループ情報
    group=true;
  }
  if(reqest){
    Serial.println("Request received");          //リクエストを受け取りました
    delay(30*MyNum) ; //自分の識別番号＊30ミリ秒遅らせる
    SendData(MybitLen,MybitData) ;  // 自身の識別データを送信する
  }
  else if(group){
    Serial.println("GROUP received.") ;

  }
  else{
  //  Serial.println("Was received.") ;            // 受信しました

    if(matome){
      //受信したデータと自身の識別番号をまとめて１つの配列に集約する処理
      //後にこれをグループ情報として送信する
      for(int i=lastlen; i < num+lastlen; i++ ){ //順々に配列の後尾に値を格納していく
        AggregatebitLen ++;
        AggregatebitData[i] = IRbitData[i-lastlen];//aggregatebitlenの末尾~末尾+8にirbitlenの0~8を追加していく

      }
      lastlen+=num;      //次回後ろに追加していくため今回のビットデータの長さを保存 
      AggregatebitData[AggregatebitLen]=0; //文字列配列の最後にヌル文字（０）を挿入して終了
    }
  }
}

//２進数のデータを１０進数に変換(8ビットごと)
int bitData2int(char *data){
  int dt=0;
  for (int i=0 ; i < 8 ; i++) {
    if (*data++ == 0x31) bitSet(dt,i) ;
  }
  return dt;
}



















