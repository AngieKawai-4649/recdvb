# dvbデバイスを使用するLinux用チューナーアプリケーション  

ベース　recdvb_ver1.3.1  

対象デバイス  
DVB device TV-tuner  
pt1,pt2,pt3用Linux DVBドライバーが自動的にカーネルに組み込まれるので  
カーネルモジュール(ドライバー)をインストールする必要がない  
カーネルモジュールearth-pt1(pt1,pt2用) earth_pt3(pt3用)はCanonicalによって署名されているので  
セキュアブート環境でも使用可能である  

## 【改造箇所】
１．コンパイル時のワーニング・エラー除去  
２．BS/CSチャンネルデータをプログラムコンスタント→外部ファイル(bscs_ch.conf)から読み込むように修正  
　ファイル名称、フォーマットはrecsanpakunと共通  
　実行ファイルrecdvbと同じディレクトリに配置(通常は/usr/local/bin)  
３．gcc version 11.2.0でのbuildエラーを修正  
　channel.h,preset.c  
４．bscs_ch.confをフルパスで指定する環境変数 BSCSCHPATH を設定  
　BSCSCHPATH設定時はそのディレクトリパス配下にbscs_ch.confを置く  
　BSCSCHPATHが設定されていない時はrecdvbと同じディレクトリ(通常は/usr/local/bin)にbscs_ch.confを置く  
5. 上記2 4 を全て廃止
 チャンネル情報はchannelconfから取得するように修正  

## 【ビルド方法】  
b25デコードをする場合、事前に https://github.com/AngieKawai-4649/libarib25 を導入する  
事前に https://github.com/AngieKawai-4649/channelconf を導入する（必須）  
取得  
git clone https://github.com/AngieKawai-4649/recdvb.git  
ビルド方法はMakefileのコメントを参照  

## 【使用方法】  
$ ./recdvb [--b25 [--round N] [--strip] [--EMM]] [--dev devicenumber] [--lnb voltage] [--tsid TSID] channel rectime destfile  

--b25 : B25解除する  B25解除を行わないでビルドした場合は指定不可  
　以下は--b25指定時のサブオプション  
　　--round N ラウンド数を指定  
　　--strip   Nullパケットを除去  
　　--EMM     EMM(契約者情報)を受信時カードを更新する  

--dev : DVBデバイスを指定する  
例： /dev/dvb/adapter0 を指定する場合  --dev 0  

--lnb voltage : LNB voltage (0, 11, 15, default 0)  
--tsid TSID: TSIDを指定 Hexは0xを付ける  

channel: チャンネルを指定する  
注:bscs_ch.confで設定するチャンネル名称(BS15_0,CS24等)はmirakurunのchannels.yml内のchannel:XXX と合わせなければならない  

rectime: 録画時間を秒単位で指定する  ( - 指定時は何時までも出力する)  
destfile: TS出力ファイル名を指定する ( - 指定時は標準出力）  

例: NHK東京 デバイスを指定して30秒録画する  
　　$ ./recdvb --b25 --dev 3 27 30 aaa.ts  
　WOWOWプライム デバイスを指定して60秒録画する  
　　$ ./recdvb --b25 --dev 0 BS03_0 60 bbb.ts  

## 【デバイスの固定】  

デフォルトではPT2 BS/CS adapter0 2 地上波 adapter1 3  
PT3 BS/CS adapter0 1 地上波 adapter2 3  
PT2,PT3を刺しているとadapterがかち合ってしまいブートする度に変わってしまうので固定する  
/etc/modprobe.d/options-dvb.conf を作成し、衛星0 地上0 衛星1 地上1の順に指定する  

## 【PT2 2枚 PT3 1枚 取り付けてあるPCの options-dvb.confファイル内容例】  
options earth_pt1 adapter_nr=0,1,2,3,4,5,6,7  
options earth_pt3 adapter_nr=8,9,10,11  

この場合は以下のデバイス配置となる  
PT2-1 BS/CS 0 2  地上波 1 3  
PT2-2 BS/CS 4 6  地上波 5 7  
PT3-1 BS/CS 8 10 地上波 9 11  

確認方法  
/var/log/kern.log  
earth-pt1,earth_pt3の各チューナーがどのアダプタに接続されているか確認できる  

## 【mirakurun tuners.ymlの内容例】  

\- name: PT2-S1  
  types:  
    \- BS  
    \- CS  
  command: recdvb --dev 0 --b25 <channel> - -  
  decoder:  
  isDisabled: false  

\- name: PT2-S2  
  types:  
    \- BS  
    \- CS  
  command: recdvb --dev 2 --b25 <channel> - -  
  decoder:  
  isDisabled: false  

\- name: PT2-T1  
  types:  
    \- GR  
  command: recdvb --dev 1 --b25 <channel> - -  
  decoder:  
  isDisabled: false  

\- name: PT2-T2  
  types:  
    \- GR  
  command: recdvb --dev 3 --b25 <channel> - -  
  decoder:
  isDisabled: false



