package jp.co.applibot.engine.androidlib;

import java.io.File;
import java.util.Locale;
import java.util.TimeZone;
import java.util.UUID;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.provider.Settings;

public class LibDeviceAndroid {
	private static Context context;
	private static String MAC_ADDRESS = null;

	public static void setContext(Context context) {
		LibDeviceAndroid.context = context;
	}
	
	public static String getDeviceName() {
		return Build.MODEL;
	}
	
	public static String getOSVersion() {
		return Build.VERSION.RELEASE;
	}

	public static String getLanguage() {
		return Locale.getDefault().getLanguage();
	}

	public static String getCountryCode() {
		return Locale.getDefault().getCountry();
	}

	public static String getDifferenceGMT() {
		float ret = 0.0f;

		TimeZone tz = TimeZone.getDefault();
		int offset = tz.getRawOffset();

		ret = offset / 1000;
		ret = ret / 60;
		ret = ret / 60;

		return String.valueOf(ret);
	}

	public static String getUuId() {
		SharedPreferences pref = context.getSharedPreferences("device_setting", Context.MODE_PRIVATE);
		return pref.getString("uuid", "");
	}
	public static String getIdentifierForVender() {
		return "";
	}

	public static String getOpenUdId() {
		return UUID.randomUUID().toString();
	}

	public static String getSecureUdId() {
		return UUID.randomUUID().toString();
	}

	public static String getMacAddress() {		
		boolean  wifiOnFlg = false;
		WifiManager wifiManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		if(MAC_ADDRESS == null) {
			MAC_ADDRESS = wifiInfo.getMacAddress();
		}

		if(MAC_ADDRESS == null || "".equals(MAC_ADDRESS)) {
			wifiManager.setWifiEnabled(true);	// wifi ON
			wifiOnFlg = true;
		}

		while(true) {
			wifiManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
			MAC_ADDRESS = wifiManager.getConnectionInfo().getMacAddress();

			if(MAC_ADDRESS != null && !"".equals(MAC_ADDRESS)) {
				if(wifiOnFlg) {
					wifiManager.setWifiEnabled(false);	// wifi off
				}
				break;
			} else {
				try {
					Thread.sleep(2000);
				} catch(Exception e) {
				}
			}
		}

		return MAC_ADDRESS;
	}
	public static String getAndroidId() {
        String androidId = null;
        androidId = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
        return androidId;        
    }
    // AdvertisingId
    private static String advertisingId = "";
    public static String getAdvertisingId() {
        return advertisingId;
    }
    public static boolean isAdid = false;
    public static String getADID() {
        AsyncTask<Void, Void, String> adTask = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                try {
                    AdvertisingIdClient.Info info = AdvertisingIdClient.getAdvertisingIdInfo(context);
                    if (info.isLimitAdTrackingEnabled()) {
                        return "isLimitAdTrackingEnabled=ON";
                    } else {
                        return info.getId();
                    }
                } catch (Exception e) {
                    isAdid = true;
                    advertisingId = "";
                    return advertisingId;
                }
            }
            
            @Override
            protected void onPostExecute(String adid) {
                advertisingId = adid;
                isAdid = true;
            }
        };
        
        adTask.execute();
        
        while(true) {
            if ( isAdid == false ) {
                try {
                    Thread.sleep(100);
                } catch(Exception e) {
                    Logger.d(TAG, e.getMessage(), e);
                }
            } else {
                break;
            }		
        }
        return advertisingId;
    }
	public static long getFileSystemSize(){
		long size = -1;

		File internalMemPath = Environment.getDataDirectory();
		if( internalMemPath != null ){
			StatFs fs = new StatFs(internalMemPath.getPath());

			long bkSize = fs.getBlockSize();
			long bkCount = fs.getBlockCount();

			size = bkSize * bkCount;
		}
		return size;
	}

	public static long getFileSystemFreeSize(){
		long size = -1;

		File internalMemPath = Environment.getDataDirectory();
		if( internalMemPath != null ){
			StatFs fs = new StatFs(internalMemPath.getPath());

			long blockSize = fs.getBlockSize();
			long availableBlockSize = fs.getAvailableBlocks();

			size = blockSize * availableBlockSize;
		}
		return size;
	}

	public static long getSystemFreeMemory(final Context context) {
		ActivityManager.MemoryInfo info = new ActivityManager.MemoryInfo();
		ActivityManager am = ((ActivityManager)context.getSystemService(Activity.ACTIVITY_SERVICE));
		am.getMemoryInfo(info);
		return info.availMem;
	}
	
	public static void setUuId(String text) {
		SharedPreferences pref = context.getSharedPreferences("device_setting", Context.MODE_PRIVATE);
		Editor edit = pref.edit();
		edit.putString("uuid", text);
		edit.commit();
	}
}
