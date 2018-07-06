1. .../proj.ios_mac/ios/main.mの開始直後に以下のコードを入れます。

 > #include "../../Classes/memory/memory_pool.h"
 > 
 > int main(int argc, char *argv[]) {
 >     mp_init(0, 1000*1024*1024,16);
 >     
 >     NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
 >     int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
 >     [pool release];
 >     return retVal;
 > }
2. ../../libs/Yggdrasil/native/iOS/CryptoHelper/CryptoWrapper.mmを開いて、以下の関数内のmallocとfreeをoriginal_mallocとoriginal_freeに置き換えます。
 >                  :
 >                  :
 > #include "../../Classes/memory/memory_pool.h"
 >                  :
 >                  :
 > std::string CryptoWrapper::decryptAES256ECB(const char* key, size_t keyLen, const char* cData, size_t dataLen)
 > {
 >                  :
 >                  :
 > void *buffer = malloc_original(bufferSize);
 >                  :
 >                  :
 >             }else{
 >                 free_original(buffer);
 >             }
