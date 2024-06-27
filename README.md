＜初めに
お手数ですが,3Dモデルのcube.obj が送ることができなかったのでテキストファイルとして送りしているので
ComponetFrameWorkProjectのasset/model/block内のcube.obj.txtの .txt 部分を消していただくことと
ソリューションエクスプローラー内のcube.obj.txtを除外していただくとビルドができます。

デバッグビルドですと20秒ぐらいリリースビルドだと6秒ぐらいお待ちいただくと地形が生成されプレイできます。

＜操作について
〇HumanMode
水平方向へ移動　w,a,s,d　キー
ジャンプ　　　　space キー
ダッシュ　　　　ctrl キー

〇PhantomMode
水平方向へ移動　w,a,s,d　キー
上昇　　　　　　space キー
下降　　　　　　rightShift キー

Modeの変更はclass Sceneウィンドウの
class Player内のPlayModeで
Human か Phantomを左クリックで変更できます。

＜使用したパッケージ
ImGui
Implot
directxtk_desktop_win10.2024.6.5.1
