/**
 * \file guarded_alloc.h
 * \ingroup adkgrp_galloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータAPI定義部
 *  \page adkpage_guardalloc ガードメモリアロケータ
 *  \tableofcontents
 *  \section adksct_ga_link 使用するファイル及びインクルード情報
 *    | 使用ファイル | インクルードするファイル |
 *    | :----------- | :----------------------- |
 *    | af-adk.a     | adk.h                    |
 *  \section adksct_ga_summary 1.ガードメモリアロケータについて
 *    \subsection adksubsct_ga_monitoring 1.1.メモリを監視
 *      OSが用意した既存のメモリアロケータを使用せず、メモリ操作を乗っ取り確保/解放を監視する事で、\n
 *
 *      - メモリを何処で確保したのかがわかる
 *      - メモリの残量がわかる
 *      - メモリの断片化具合がわかる
 *      - 状況によってメモリセクションを使い分ける事ができる
 *
 *      といった開発を進めていく上で便利な機能を実装する事ができる。
 *    \subsection adksubsct_ga_management 1.2.メモリを管理するということは
 *      - メモリ管理をするということは自身で責任をもつということ
 *      - メモリ関係のバグは即死系のものや、メモリ破壊によって追跡が困難なバグが多い
 *      - メモリ関係のバグで一日デバッグ作業に追われることもある
 *      - コンソールなゲームの世界ではメモリ管理ができないと話にならない
 *    \subsection adksubsct_ga_example 1.3.実用例
 *      - メモリリークの検出に、メモリブロックに名前などを付加することで容易に発見する事ができる
 *      - 今回Chaos/Gang2などネイティブの細かな断片化への対策
 *        - 本来行う解決方法ではない
 *        - しかし根本から解決するのは時間がかかりすぎるのとCocosを使用している限り限りなく不可能に近い
 *        - そこでメモリプールを設け、サイズ毎に確保する領域を決めてしまう事で、断片化をかなり軽減する事ができる
 *        - 起動時にメモリプール用の大きなバッファを確保する必要があるのが難点
 *      - メモリ管理はとても便利だが危険
 *        - メモリ管理のメリットは沢山あるが、ややこしさが増すので必要性がないならオススメしない
 *        - 上手く実装すれば、ものすごく便利なものとなるので積極的に活用すべき
 *  \section adksct_ga_native 2.ネイティブアプリで起こっている事
 *    \subsection adksubsct_ga_native_fragmantaiton 2.1.メモリのフラグメンテーション
 *      ネイティブで長時間アプリを起動し続けるとハングアップするという現象の一つにメモリのフラグメンテーション問題が挙げられます。\n
 *      これは一見メモリの空き領域は足りていても、メモリの一単位が、そのままでは利用できない状態になる事を言います。\n
 *      以下のようにハードウェアにはメモリというものが有り、アプリケーションはこのメモリ上に様々なデータを展開して利用しています。\n
 *      \image html galloc_01.png
 *      そして不要になったデータは解放処理を行い以下のように、次に備えた空き領域になるわけです。\n
 *      \image html galloc_02.png
 *      ですが一見大きな空き領域が出来上がったように見えますが実際には、元のメモリブロックの位置を動かす事は出来ないため、この空き領域ほどの大きさのデータを確保する事ができません
 *      \image html galloc_03.png
 *      こういった現象が起こる例としてGZipUtil.ccの圧縮データの解凍時があります。\n
 *      以下の図のように、\n
 *      -# データ解凍用のテンポラリメモリを用意する
 *      -# データを解凍する
 *      -# 不要になったテンポラリメモリを解放する
 *
 *      これだけの手順で大きなフラグメンテーションが発生しています。
 *      \image html galloc_04.png
 *      また参考までにChaosでマイページまで移動した際のメモリ情報を出力したテキストファイル <a href="memdump-_08_CCBSceneMypage1_8.txt"><b>「MyPageメモリ情報」</b></a> を用意しました。\n
 *      この情報を見ますと1MBを超えるフラグメンテーションなメモリ領域がいくつも点在してしまっています。\n
 *      \n
 *      こういったフラグメンテーションを根本的に解決するには、いくつかの方法があります。\n
 *      例えばテンポラリメモリをメモリの後ろに確保してしまい、解凍後のデータはこれまで通り前から確保するなどです。\n
 *      そうする事でフラグメンテーションが発生せずメモリを扱う事ができます。\n
 *      \image html galloc_05.png
 *    \subsection adksubsct_ga_native_countermeasure 2.2.フラグメンテーションへの対策
 *      長期的に起動し続けるアプリケーションや大きなメモリ領域を使用するゲームでは、上記した方法等によりメモリのフラグメンテーションを事前に防ぐコーディングが望ましい。\n
 *      ですが、現状のネイティブに変更を加えようとすると全体のフロー管理から見直して修正しなければならないため作業工数からも、すぐに実装するには現実的ではありません。\n
 *      そこで根本的な解決にはなっていませんが、ADKでは次のような方法でフラグメンテーションへの対策を行いました。\n
 *      \n
 *      \image html galloc_06.png
 *      まず、\n
 *      -# メモリ上に確保するサイズに応じたメモリプールを用意
 *      -# メモリの確保時に対応した確保サイズのメモリプールからメモリを確保する
 *
 *      こうする事でサイズ単位でメモリが管理されるためフラグメンテーションが起こりづらくなります。\n
 *      しかしこれでも大きなメモリの確保には、こういったプール領域ではなくこれまで通りのメモリ操作を行うので根本的な解決にはなっていません。\n
 *      ですが、現状のネイティブでは小さなメモリ(プールに割り当てられるもの)が、かなり大量に存在するため、この要な手法が応急処置としては妥当かと考えられます。\n
 *      \n
 *      それでも根本的な解決にはならないので落ちないアプリケーションを作るために最終的には、いつの日か全体のフローから組み直していけたらと考えられます。
 *  \section adksct_ga_howtouse 3.ガードアロケータの使い方
 *    \subsection adksubsct_ga_impl 3.1.ガードアロケータの実装方法
 *      -# プロジェクト(Xcode)の「Resource」に"adk_galloc_info.txt"を追加する。\n
 *        "adk_galloc_info.txt"ファイルについては同梱の"resource"ディレクトリ内にあります。\n
 *        詳細は \subpage adkpage_alloc_info を参照して下さい
 *      -# あとは基本的にADKフレームワークをプロジェクトに追加した時点でガードアロケータの機能は動いています。
 *      -# ADKのガードアロケータが機能しているかの確認方法\n
 *        アプリケーションが起動するとコンソール出力にガードアロケータが作成したメモリプール情報が流れる事で確認することができます。
 *    \subsection adksubsct_ga_valid 3.2.ガードアロケータの有無
 *      ガードアロケータを有効または無効にする切り替えは、上記した「Resource」ディレクトリ内へ配置した"adk_galloc_info.txt"を組み込むか組み込まないかで動作が変わります。\n
 *      - adk_galloc_info.txt を組み込んだ場合
 *        - ガードアロケータ機能が働きます
 *      - adk_galloc_info.txt を組み込まなかった場合
 *        - ガードアロケータ機能が無効になります
 *    \subsection adksubsct_ga_foxsdk 3.3.FOXSDKへの対応
 *      現在、残念なことにFOXSDKのメモリ操作に対応出来ていないため、ハングアップしてしまいます。\n
 *      <i>(こちらのバグについては現在FOXSDKを作成したチームへ問い合わせ中です)</i>\n
 *      \n
 *      そこでFOXSDKを使用している以下の箇所で、 \ref ADK_EnableGuardAllocator() / \ref ADK_DisableGuardAllocator() を組み込む事で応急処置を行っています
 *      組み込んだ箇所については「 \subpage adkpage_native_modification_foxsdk 」を参照して下さい
 *    \subsection adksubsct_ga_debug 3.4.ガードアロケータによるデバッグ機能
 *      - 現在のメモリ状況の出力\n
 *        現在のメモリの状況を \ref ADK_OutputMemoryListForTXT() 関数を使う事によりTXT形式で出力します。\n
 *        出力先はコンソールに出力されますので、そちらを参照して下さい。
 *      - メモリのカテゴリ化\n
 *        一単位毎のメモリをブロックというカテゴリに分ける事で、どの範囲で確保したメモリなのかを把握する機能です。\n
 *        前途した \ref ADK_outputMemoryListForTXT() 関数を用いてメモリ状況を出力しても、メモリの数は大量に存在するためメモリリーク等を検出するのは困難です。\n
 *        そこで \ref ADK_BeginMemoryBlock() / \ref ADK_EndMemoryBlock() を使えば、マイページ中に確保したメモリなのか、ギルドバトル中に確保したものなのか等のメモリのカテゴリ化を行う事ができ、出力情報にメモリのカテゴリを付加する事ができます。\n
 *        以下に、シーン読み込みを境にメモリのカテゴリ化を切り替える事でシーン単位のメモリ状況を把握する処理を実装したサンプルを記載します。
\code
// Util.cc
void Util::loadSceneFadeOut(
    const char* ccbiName,
    CCNodeLoader* loader,
    float fadeTime,
    bool cacheClear) {
  static ADK_MEMBLOCK_HANDLE block_handle = 0;  // カテゴリハンドル
  static int count = 0;
  char listname[64] = {0};

  // 現在のメモリ情報を出力する
  sprintf(listname,"_%02d_%s",count++,ccbiName);
  ADK_OutputMemoryListForTXT(true,listname);

  // 既にハンドルが存在していたらメモリのカテゴリ化を終了してからログを出力する
  if (block_handle!=0) {
    ADK_EndMemoryBlock(block_handle);
  }
  // メモリのカテゴリ化を開始する
  block_handle = ADK_BeginMemoryBlock(ccbiName);
  .
  .
  .
 */
/**
 * \~english
 * \~japanese
 *  \page adkpage_alloc_info 「adk_galloc_info.txt」ファイルについて
 *  \section adksct_allocinfofile adk_galloc_info.txt ファイルとは
 *    "adk_galloc_info.txt"は以下のようにガードアロケータのプール情報を記載し、ガードアロケータを起動する為の情報を持ったファイルです。\n
 *    "adk_galloc_info.txt"ファイルについては同梱の"resource"ディレクトリ内にあります。
 *    - <b>small</b>\n
 *      分割したいメモリプール毎にプールに割り当てる確保領域を設定します。\n
 *      この "small" プールには比較的小さめのメモリブロック領域を割り当てます。\n
 *      "small" プールは使用者の任意で追加/削除を行って構いません。\n
 *      <b>注意点</b>として、それぞれのプールの min/max に隙間があってはいけません。
 *    - <b>large</b>\n
 *      この "large" 領域には大きいメモリブロック用に用意されたプールです。
 *    - <b>option</b>\n
 *      実装中です。
\code
{
  "small":[
    {
      "min": 0,
      "max": 16,
      "size": 0x100000
    },
    {
      "min": 17,
      "max": 32,
      "size": 0x800000
    },
    {
      "min": 33,
      "max": 64,
      "size": 0x800000
    },
    {
      "min": 65,
      "max": 128,
      "size": 0x400000
    },
    {
      "min": 129,
      "max": 256,
      "size": 0x100000
    },
    {
      "min": 257,
      "max": 512,
      "size": 0x7000000
    },
    {
      "min": 513,
      "max": 1024,
      "size": 0x100000
    },
    {
      "min": 1025,
      "max": 2048,
      "size": 0x100000
    },
    {
      "min": 2049,
      "max": 4096,
      "size": 0x100000
    }
  ],
  "large": [
    {
      "size": 0x7000000
    }
  ],
  "option": [
    {
      "memory name": true,
      "block name": true
    }
  ]
}
\endcode
 */
/**
 * \~english
 * \~japanese
 *  \page adkpage_native_modification_foxsdk FOXSDKの対応しきれていないメモリ対策について
 *  \section adksct_modifications FOXSDKによる修正箇所一覧
 *    - AppDelegate.cc\n
 *      <i>AppDelegate::applicationDidFinishLaunching()内部</i>
\code
ADK_DisableGuardAllocator();
CCLog("FoxPlugin::sendConversion");
FoxPlugin::sendConversion("joker://");
FoxPlugin::sendStartSession();
ADK_EnableGuardAllocator();
\endcode
 *      <i>AppDelegate::applicationWillEnterForeground()内部</i>
\code
ADK_DisableGuardAllocator();
FoxPlugin::sendStartSession();
ADK_EnableGuardAllocator();
\endcode
 *    - ApiTutorialEnd.cc\n
 *      <i>ApiTutorialEnd::onHttpRequestResponseData()内部</i>
\code
// チュートリアル突破計測
ADK_DisableGuardAllocator();
CCLOG("FoxPlugin::sendLtv 1665");
FoxPlugin::sendLtv(1665,Chaos::information::UuidGenerator::sharedUuidGenerator()->getUuid()->getCString());
ADK_EnableGuardAllocator();
\endcode
 *    - CCBSceneTutorialRegistration.cc\n
 *      <i>CCBSceneTutorialRegistration::getUserRegisterResponse()内部</i>
\code
// FOXSDK ID登録成果通知
ADK_DisableGuardAllocator();
CCLog("FoxPlugin::sendLtv 691");
FoxPlugin::sendLtv(691,Chaos::information::UuidGenerator::sharedUuidGenerator()->getUuid()->getCString());
ADK_EnableGuardAllocator();
\endcode
 *    - CCBSceneTutorialShort03Registration.cc\n
 *      <i>CCBSceneTutorialShort03Registration::getUserRegisterResponse()内部</i>
\code
// FOXSDK ID登録成果通知
ADK_DisableGuardAllocator();
CCLog("FoxPlugin::sendLtv 691");
FoxPlugin::sendLtv(691,Chaos::information::UuidGenerator::sharedUuidGenerator()->getUuid()->getCString());
ADK_EnableGuardAllocator();
\endcode
 *    - CCBSceneUserRegistration.cc\n
 *      <i>CCBSceneUserRegistration::getUserRegisterResponse()内部</i>
\code
// FOXSDK ID登録成果通知
ADK_DisableGuardAllocator();
CCLog("FoxPlugin::sendLtv 691");
FoxPlugin::sendLtv(691,Chaos::information::UuidGenerator::sharedUuidGenerator()->getUuid()->getCString());
ADK_EnableGuardAllocator();
\endcode
 */

#ifndef GUARDED_ALLOC_H_
#define GUARDED_ALLOC_H_

#include <core/memory_allocation/guarded_alloc_types.h>
#include <core/memory_allocation/guarded_alloc-c_api.h>

#endif /* GUARDED_ALLOC_H_ */

