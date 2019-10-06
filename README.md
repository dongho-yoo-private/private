# README #

このレポジトリは、自分のライブラリを集めたものです。
普段はプライベートにしていますが、面接の時のみパブリックにしたりします（笑）
以前は、自宅サーバのSVNにしていましたが、時代も時代なのでgitにまとめてアップロードしています。
当時はbitbuketしか無料のプライベートがなかったので、bitbuketに入れています。


### 構成 ###

* mylibs （自分のプライベートのライブラリを集めたところ）
** abps 
*** 以前、どこかの会社にいた時に作成したものを整理したもの、あまり大したものではない

** goyotsuya
*** 最も頑張ったほぼ全てを自作ライブラリにしたものです。
*** 定石事典、対局ソフト（P2P）、対局ソフト（pop3）、インストーラなどがあります。
*** GUIは年寄りのために伸び縮みができるようなフレームワークを自作で作成しました。
*** ボタン、コンボボックス、スクロールビュー全て自慢の自作です（バグが多いけど 笑）
*** 囲碁の仲間に配って終了状態です。

**  igotomo-engine
*** スマホ用の囲碁のアプリを作成しようと作成したもの、
*** ライブラリーやエンジンまで作成して放置状態（笑）
*** プライベートのソースの書き方はgoyotsuyaがほぼ最新の書き方
*** いくつかのNetworkプロトコル、データ構造（メモリ断片化に優しいもの）
*** 囲碁のエンジンなどが入っています。

** panda/ProjectPanda 
*** goyotsuyaのサーバで、自宅サーバを想定したP2Pのすチューンサーバ。
*** 自宅サーバなので、DBを使わずにtryの変形でユーザを管理します（爆速）
*** データ管理もファイルを使ったり、memcpy,memcmpなどをアセンブラで最適化したり。。。

** memory_pool
*** メモリプールです。
*** mallocやnewを乗っとり、プールが使える仕組みです。

* wonder_planet （今の会社で教育のためにわざわざ作成したコード）
** 大した内容ではない

* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact
