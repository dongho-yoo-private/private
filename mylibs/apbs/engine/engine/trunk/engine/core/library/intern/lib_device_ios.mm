/**
 * \file lib_device_ios.mm
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief デバイス情報に関する実装部です
 */


#include <core/library/lib_device.h>

//#inlcude <uuid/uuid.h>
#include <sys/sysctl.h>
#import <UIKit/UIKit.h>
#import <AdSupport/AdSupport.h>
#include <OpenUDID/OpenUDID.h>
#include <SecureUDID/SecureUDID.h>
#include <UICKeyChainStore/UICKeyChainStore.h>
#include <NetworkInformation/NetworkInformation.h>

#include <core/debug/logging.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>

#include <mach/mach.h>
#include <mach/mach_host.h>

#define INSTALLATION_COUNT_KEY @"installation_count"
#define UUID_KEY @"uuid"
#define MAX_UUID_SIZE (63)


ADK_BEGIN_NAMESPACE

static Char* gDeviceToken;


/* extern function implementation */
const char *DEV_GetDeviceName() {
  size_t  size;
  char  *machine;

  sysctlbyname( "hw.machine", 0, &size, 0, 0);
  ADK_ASSERT(size>0);

  machine = (char*)ADK_Malloc( size, "machine");
  ADK_ASSERT(machine);
  sysctlbyname( "hw.machine", machine, &size, 0, 0);

  NSString  *platformname = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
  ADK_Free( machine), machine = 0;

  return [platformname UTF8String];
}
const char *DEV_GetOSName() {
  NSString  *devname = [UIDevice currentDevice].systemName;
  ADK_ASSERT(devname);

  NSString  *trim = [devname stringByReplacingOccurrencesOfString:@" " withString:@""];
  ADK_ASSERT(trim);

  return [trim UTF8String];
}
const char *DEV_GetOSVersion() {
  NSString  *devver = [UIDevice currentDevice].systemVersion;
  ADK_ASSERT(devver);

  return [devver UTF8String];
}
const char *DEV_GetLanguage() {
  NSArray  *langlist = [NSLocale preferredLanguages];
  ADK_ASSERT(langlist);

  NSString  *currentlang = [langlist objectAtIndex:0];
  ADK_ASSERT(currentlang);

  return [currentlang UTF8String];
}
const Char *DEV_GetCountryCode() {
  //地域コード（ISO 3166-1）
  NSString *countryCode = [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
  return [countryCode UTF8String];
}
const char *DEV_GetDifferenceGMT() {
  NSDate  *date = [NSDate date];
  ADK_ASSERT(date);

  NSDateFormatter  *fmt = [[NSDateFormatter alloc] init];
  fmt.dateFormat = @"z";

  return [[fmt stringFromDate:date] UTF8String];
}
const char *DEV_GetUuId() {
  UICKeyChainStore* kc = [UICKeyChainStore keyChainStore];
  NSUserDefaults* ud = [NSUserDefaults standardUserDefaults];
  NSString* nsstr_uuid = [ud objectForKey:UUID_KEY];
  const char* ret = 0;
    
  /**
   * NSUserDefaults   アプリに紐付く値。アプリ削除の際に消滅する
   * UICKeyChainStore 端末に紐付く値。アプリ削除しても消えない
   * TODO: NSUserDefaults は使わないようにする
   */
  
  if (nsstr_uuid==0)
  {
    // UserDefault に無い場合は、KeyChainStore から取得
    nsstr_uuid = [UICKeyChainStore stringForKey:UUID_KEY];
    
    // UserDefault に適用
    if (nsstr_uuid!=0)
    {
      [ud setObject:[NSString stringWithCString:[nsstr_uuid UTF8String] encoding:NSUTF8StringEncoding] forKey:UUID_KEY];
      [ud synchronize];
    }
  }
    
  if (nsstr_uuid==0)
  {
    // なければ、新たに生成し、NSUserDefaultsに保存する。
    char str_uuid[MAX_UUID_SIZE+1];
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, str_uuid);
    
    [ud setObject:[NSString stringWithCString:str_uuid encoding:NSUTF8StringEncoding] forKey:UUID_KEY];
    [ud synchronize];
    
    nsstr_uuid = [ud objectForKey:UUID_KEY];
    ADK_ASSERT(nsstr_uuid);
  }
  
  // KeyChainStore へ同期
  NSString* kc_uuid = [kc stringForKey:UUID_KEY];
  if ((kc_uuid==0) || ([kc_uuid isEqualToString:nsstr_uuid] == NO))
  {
    [kc setString:[NSString stringWithCString:[nsstr_uuid UTF8String] encoding:NSUTF8StringEncoding] forKey:UUID_KEY];
    [kc synchronize];
  }
  
  ret = [nsstr_uuid UTF8String];
  return ret;
}

const char* DEV_GetIdentifierForVender()
{
  NSString * idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
  return [idfv UTF8String];
}

const Char* DEV_GetIdentifierForAdvertiser()
{
  NSString * idfa = @"null";
  if ([ASIdentifierManager sharedManager]){
    idfa = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
  }
  return [idfa UTF8String];
}
const Char* DEV_GetAndroidId()
{
    return "";
}
const Char* DEV_GetAdvertisingId()
{
    return "";
}
const char* DEV_GetOpenUdId()
{
  return [[OpenUDID value] UTF8String];
}

const char *DEV_GetSecureUdId()
{
  return [[SecureUDID UDIDForDomain:[[NSBundle mainBundle] bundleIdentifier] usingKey:@"applibot"] UTF8String];
}

const char *DEV_GetDeviceToken()
{
  return gDeviceToken;
}

void DEV_SetDeviceToken(const unsigned char* data, unsigned int size)
{
  //ADK_ASSERT(!gDeviceToken);
  ADK_ASSERT(data);

  if (size==0)
  {
     // こういうケースがあり得る？
     return ;
  }
  
  if (gDeviceToken)
  {
    delete gDeviceToken;
  }
  
  gDeviceToken = new Char[ size*2+1 ];
  Char* out = gDeviceToken;
  
  Char buf[4];
  for( int lp = 0; lp < size; ++lp ){
    // dataはcharなので、以下の様にしないと、スタックの状態次第でゴミの値が表示されてしまう。
    sprintf(buf, "%02x", 0xFF&((unsigned int)data[lp]));
    *(out++) = buf[0];
    *(out++) = buf[1];
  }
  
  *out = 0;
}

const char *DEV_GetMacAddress()
{
  return [[NetworkInformation sharedInformation].primaryMACAddress UTF8String];
}

const char *DEV_GetEmailAddress()
{
    // iOSでは(必要ないので)Emailは取得しない
    return nil;
}

U64 DEV_GetFileSystemSize()
{
  NSError *error = nil;
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *documentsDirectory = [paths objectAtIndex:0];
  NSDictionary *dict = [[NSFileManager defaultManager] attributesOfFileSystemForPath:documentsDirectory error:&error];
  return [[dict objectForKey:NSFileSystemSize] longLongValue];
}

U64 DEV_GetFileSystemFreeSize()
{
  NSError *error = nil;
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *documentsDirectory = [paths objectAtIndex:0];
  NSDictionary *dict = [[NSFileManager defaultManager] attributesOfFileSystemForPath:documentsDirectory error:&error];
  return [[dict objectForKey:NSFileSystemFreeSize] longLongValue];
}

U64 DEV_GetSystemFreeMemory()
{
  vm_statistics_data_t vmStats;
  mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
  kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
  if( kernReturn != KERN_SUCCESS )
    return 0;
  return vm_page_size * vmStats.free_count;
}

U32 DEV_GetInstallationCount()
{
    UICKeyChainStore* kc = [UICKeyChainStore keyChainStore];
    NSUserDefaults*   ud = [NSUserDefaults standardUserDefaults];
    
    // userdefaultsから取得
    if ( U32 udRet = [ud integerForKey:INSTALLATION_COUNT_KEY] )
    {
        [kc setString:[NSString stringWithFormat:@"%d", udRet] forKey:INSTALLATION_COUNT_KEY];
        [kc synchronize];
        return udRet;
    }
    
    U32 result = 1;
    
    // キーチェーンから取得
    if ( U32 kcRet = [[UICKeyChainStore stringForKey:INSTALLATION_COUNT_KEY] intValue] )
    {
        result = kcRet + 1; // 再インストールされている
    }
    
    // 
    [ud setInteger:result forKey:INSTALLATION_COUNT_KEY];
    [ud synchronize];
    [kc setString:[NSString stringWithFormat:@"%d", result] forKey:INSTALLATION_COUNT_KEY];
    [kc synchronize];
    
    return result;
}

ADK_END_NAMESPACE
