================================================================
【 　名称 　】Spinning Cube
【 製作者名 】日本電子専門学校　キムチャンモク(個人製作)
【 動作環境 】DirectX10、ゲームパッド（XBOX）
【バージョン】1.96
【　作成日　】平成30年12月15日
【 制作環境 】VisualStudio2017,
	   C++,
 	   DirectX10 WindowsSDK10.10586.212 DirectX SDK(June2010),
	   学校ライブラリ
================================================================
注）音声が出ます。音量をご確認ください。

◾︎　ゲームの概要
3D空間の中に、面々が小さい迷路になっているでかいキューブがあります。
その中には青いボールが重力だけで動いています。
プレイヤはキューブをうまく回転して、ボールを旗(ゴール)まで転がせる
3Dのパズルゲームです。

◾   アピールポイント(ゲーム)
現在の迷路ゲームで、プレイヤが迷路を動かして、脱出を試みるゲームは
私の知っている限り、ビデオゲームで相当するゲームはありません。
そういったところで、3次元の回転で空間知覚能力が上がることと、
思ったままには動かないボールをコントロールすることからくる楽しさが
このゲームのアピールポイントだと思います。

◾︎　アピールポイント(プログラム)
今回の作品は学校内のライブラリーに既にあるあたり判定では、
やろうとした物理運動をさせることは無理があると考えて、
あたり判定を最初から自分で作ってみました。
点と三角形のあたり判定で、三角形に内包している点なら、
その点を始点で、三角形の頂点にベクトルを引いたら、
その3つのベクトルの内角の和は絶対360度になることを
あたり判定の仕組みで使っています。
こちらが実装されているCollision classを見ていただきたいと思います。
物理演算は反射ベクトルと斜め加速演算を行っています。
実装はPhysics classの中にありますので、そちらも見てください。

◾︎　製作期間
約6ヶ月

◾︎　一人で作ったソースコード
”一人で作ったソースコード”のフォルダーに分けておりましたので
そちらをご確認ください
GitHubでダウンロードした場合はソリューションファイルの
フィルターでMyPgの中でTasksで分けているもの以外は
1から一人で作ったソースコードです。

◾︎　インストール方法
実行フォルダを任意の場所へコピーしてください。

◾︎　アンインストール方法
コピーしたフォルダを削除してください。

◾︎　起動方法
実行フォルダ内にある.exeファイルをダブルクリックしてください。
もし使えるGPUが2つ以上なら、コンソール画面で選択するようになっていますので、
相当する番号を入力してEnterキーを押すと起動します。

◾︎　操作方法
※XBOX用ゲームパッドのボタン配置に対応しています。

［タイトル画面］
メニュー（START）ボタン・・・決定
Bボタン(デュアルショックの○)・・・決定
Aボタン(デュアルショックのX)・・・取り消し
十字キー上下・・・メニュー選択

［ゲーム本編画面］
Lstick上下・・・X軸回転
Lstick左右・・・Y軸回転
Ltriger,Rtriger(デュアルショックのL2,R2)・・・Z軸回転
Bボタン(デュアルショックの〇)・・・回転の微調整
Yボタン(デュアルショックの△)・・・カメラをボールがある面に移動させる
STARTボタンとSELECTボタンの同時押し・・・ゲームを諦めてリザルトへ行く

［リザルト画面］
メニュー（START）ボタン・・・タイトルに戻る

◾︎　GitHubのアドレス
https://github.com/Tails-Korea-Lumine/Physics_Maze

もし実行とかに何か問題があったらこちらで確認してください
