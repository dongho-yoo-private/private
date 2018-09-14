/**
* Copyright (c) 2015.
* All Rights Reserved.
* Authorized dongho.yoo.
*/
#ifndef __IGX_UHP_H__
#define __IGX_UHP_H__

#include "network.h"


#ifdef __cplusplus
#   define UHP_EXTERN extern "C"
#else
#   define UHP_EXTERN extern
#endif
/**
* @brief セキュリティモジュールを指定します。\n
* WIN32の場合はdll, Unix系の場合はsoファイルでその中で暗号化を行います。\n
* 
*/
UHP_EXTERN bool_t igx_uhp_Initialize(const char* security_module);
/**
* @brief igx_uhpサーバへ接続します。
* @param addr 接続先のアドレス
* @param projectname プロジェクト名
* @param security_token セキュリティトークン
* @return 0以外は成功
*/
UHP_EXTERN igx_uhp_t igx_uhp_connect(igx_port_addr_t* addr,const char* projectname,const char* security_token);
/**
* @brief uhpサーバの方からpeerのリストを取得します。
* @param handle igx_uhp_connectで生成されたハンドル
* @param desired_count 希望する数
* @param peers_count 実際に受け取った数
* @return @ref igx_peer_t の配列のアドレス
*/
UHP_EXTERN igx_peer_t igx_uhp_get_peers(igx_uhp_t handle,size_t desired_count,size_t* peers_count);
/**
* @brief ブロードキャストで、特定のpeerを探します。
* @param peer_id 特定のpeer
* @return @ref igx_peer_t peerのハンドル
*/
UHP_EXTERN igx_peer_t igx_peer_search(const char* peer_id);
/**
* @brief 特定のpeerに接続します。
* @return true成功、false失敗
*/
UHP_EXTERN bool_t igx_peer_connect(igx_peer_t peer,int timeout_msecs);
/**
* @brief peerにデータを送ります。
* @param data 送るデータ
* @param size 送るデータサイズ
* @return true成功、false失敗
*/
UHP_EXTERN bool_t igx_peer_send(igx_peer_t peer,const void* data,size_t size);
/**
* @brief peerからデータを受診します。
* @param received 受信したデータのサイズ
* @return 受信したデータ
*/
UHP_EXTERN void* igx_peer_recv(igx_peer_t peer,size_t* received);
/**
*/
UHP_EXTERN void igx_uhp_close(igx_uhp_t uhp);
/**
*/
UHP_EXTERN void igx_peer_close(igx_peer_t peer);
/**
*/
UHP_EXTERN void igx_peers_close(igx_peer_t peer);

/*typedef struct {
   addr_v4_t addr;
   port_t port;
}igx_stune_protocol_t;*/



#endif /* __IGX_UHP_H__ */
