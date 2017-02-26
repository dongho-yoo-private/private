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
import android.content.Intent;
import android.net.Uri;

public class LibUrlSchemeAndroid {
	private static Context context;

	public static void setContext(Context _context) {
		context = _context;
	}
		
	public static void openUrlScheme(String url) {
		context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
	}
}
