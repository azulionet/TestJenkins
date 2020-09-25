package com.ncsoft.gpa;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

public class GPAPlugin
{
    static public int NativeGetBatteryTemperature(Context context)
    {
        IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = context.registerReceiver(null, intentFilter);

        return batteryStatus.getIntExtra(BatteryManager.EXTRA_TEMPERATURE, 0);
    }  
}