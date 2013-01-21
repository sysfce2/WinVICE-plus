/*
 *  Copyright (C) 2011-2012 Locnet (android.locnet@gmail.com)
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

package com.locnet.vice;

import java.nio.Buffer;
import java.nio.ByteBuffer;

import com.locnet.vice.Globals;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;


public class DosBoxLauncher extends Activity {
	public static final int SPLASH_TIMEOUT_MESSAGE = -1;
	public static final int SHOW_IM_PICKER_MESSAGE = -2;
	public static final int TURBO_MESSAGE = -3;
	public static final int RESET_JOY_BUTTON_MESSAGE = -4;
	public static final int RESET_JOY_DIRECTION_MESSAGE = -5;
	
	public static native void nativeInit();
	public static native void nativeShutDown();
	public static native int nativeSetOption(int option, int value, String value2);
	public native void nativeStart(Bitmap bitmap, int width, int height);
	public static native void nativePause(int state);
	public static native void nativeStop();
	public static native void nativeReset();
	public static native void nativeSaveState(String filename, int num);
	public static native void nativeLoadState(String filename, int num);	
	public static native int nativeAudioWriteBuffer(short audioBuffer[]);

	/*static {
		System.loadLibrary("uae");
	}*/
	
	public DosBoxSurfaceView mSurfaceView = null;
	public DosBoxAudio mAudioDevice = null;
	public DosBoxThread mDosBoxThread = null;
	
	
	public boolean mPrefRefreshHackOn = true;
	public boolean mPrefCycleHackOn = true;
	public boolean mPrefScaleFilterOn = false;
	public boolean mPrefSoundModuleOn = true;
	public boolean mPrefOnscreenButtonOn = true;
	public boolean mPrefHardkeyOn = true;
	public String mPrefKeyMapping = DosBoxMenuUtility.DEFAULT_KEY_MAPPING;
	public int mPrefCycles = 0; 
	public int mPrefFrameskip = 2; 
	public int mPrefMemorySize = 4; 
	public int mPrefScaleFactor = 100;
	public boolean mPrefTapClickOn = true;
	public int mPrefMouseSensitivity = DosBoxMenuUtility.DEFAULT_MOUSE_SENSITIVITY;
	public float mMouseScale = 1.0f;
	public boolean mPrefForceKeyboardOn = false;
	public boolean mPrefUseLeftAltOn = true;
	public boolean mTurboOn = false;
	public boolean mPrefSoundOn = true; 
	public boolean mPrefShowInfoOn = false; 
	public boolean mPrefFastMouseOn = false; 
	public int mPrefJoyKeySet = DosBoxMenuUtility.JOYKEY_SET_DPAD; 
	public boolean mPrefAlwaysShowJoystick = false; 
	public boolean mPrefLargeJoystick = false;
	public boolean mPrefTrueDriveOn = false; 
	public boolean mPrefKeyUpDelay = true;
	public boolean mPrefBetterVideoOn = false; 
	public int mPrefScaleMode = DosBoxSurfaceView.SCALE_MODE_CUSTOM;
	public int mJoyPort = 0; 
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		{
			//SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
			//boolean famec = sp.getBoolean(PreConfig.PREF_KEY_FAMEC_ON, getResources().getBoolean(R.bool.pref_default_famec_on));
			System.loadLibrary("vice");
		}

		mSurfaceView = new DosBoxSurfaceView(this);
		setContentView(mSurfaceView);
		registerForContextMenu(mSurfaceView);

		//BitmapDrawable splash = (BitmapDrawable) getResources().getDrawable(R.drawable.splash);
		//splash.setTargetDensity(120);
		//splash.setGravity(Gravity.CENTER);		
		//mSurfaceView.setBackgroundDrawable(splash);
		
		DosBoxMenuUtility.loadPreference(this);				
		//TODO
		DosBoxMenuUtility.copyConfigFile(this);
		
		initDosBox();
		startDosBox();

		//don't know whether one more handler will hurt, so abuse key handler
		//mSurfaceView.mKeyHandler.sendEmptyMessageDelayed(SPLASH_TIMEOUT_MESSAGE, 1000);

		//should show credit for Locnet too
	}

	@Override
	protected void onDestroy() {
		/*InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
		if (imm != null) {
			imm.hideSoftInputFromWindow(getCurrentFocus().getWindowToken(), 0);
		}*/

		shutDownDosBox();

		mSurfaceView.shutDown();
		mSurfaceView = null;

		super.onDestroy();
		
		//System.exit(0);
	}

	@Override
	protected void onPause() {
		pauseDosBox(true);
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		pauseDosBox(false);
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);		
		return DosBoxMenuUtility.doCreateOptionsMenu(menu);
	}
	
	@Override
	public boolean onPrepareOptionsMenu (Menu menu) {
		super.onPrepareOptionsMenu(menu);
		return DosBoxMenuUtility.doPrepareOptionsMenu(this, menu);
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item)	{
		//onResume();
		if (DosBoxMenuUtility.doOptionsItemSelected(this, item))
			return true;
	    return super.onOptionsItemSelected(item);	    
	}	

    @Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		//onPause();
		return super.onMenuOpened(featureId, menu);
	}

	@Override
	public void onOptionsMenuClosed(Menu menu) {
		//onResume();
		super.onOptionsMenuClosed(menu);
	}

	@Override
	public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
		  super.onCreateContextMenu(menu, v, menuInfo);
		  DosBoxMenuUtility.doCreateContextMenu(this, menu, v, menuInfo);
	}
	
	@Override
	public boolean onContextItemSelected(MenuItem item) {
		if (DosBoxMenuUtility.doContextItemSelected(this, item))
			return true;
	    return super.onOptionsItemSelected(item);	    
	}	
	
	 @Override
	 protected void onActivityResult(final int requestCode, final int resultCode, final Intent extras) {
		 String path = null;

		 if (requestCode == Globals.PREFKEY_F1_INT) {
			 path = (resultCode == RESULT_OK)?extras.getStringExtra("currentFile"):"/sdcard/df0.adf";
			 nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, 1, path);
		 } else if (requestCode == Globals.PREFKEY_F2_INT) {
			 path = (resultCode == RESULT_OK)?extras.getStringExtra("currentFile"):"/sdcard/df1.adf";
			 nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, 2, path);
		 } else if (requestCode == Globals.PREFKEY_F3_INT) {
			 path = (resultCode == RESULT_OK)?extras.getStringExtra("currentFile"):"/sdcard/df2.adf";
			 nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, 3, path);
		 } else if (requestCode == Globals.PREFKEY_F4_INT) {
			 path = (resultCode == RESULT_OK)?extras.getStringExtra("currentFile"):"/sdcard/df3.adf";
			 nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, 4, path);
		 }
	 }
	
	void pauseDosBox(boolean pause) {
		if (pause) {
			mDosBoxThread.mDosBoxRunning = false;
			nativePause(1);
			if (mAudioDevice != null)
				mAudioDevice.pause();			
		}
		else {
			nativePause(0);
			mDosBoxThread.mDosBoxRunning = true;
			//will auto play audio when have data
			//if (mAudioDevice != null)
			//	mAudioDevice.play();		
		}
	}
	
	void initDosBox() {
		mAudioDevice = new DosBoxAudio(this);
		
		nativeInit();

		//TODO
		//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CYCLES, mPrefCycles, null);
		nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CYCLES, 0, null);
		nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_FRAMESKIP, mPrefFrameskip, null);
		//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_MEMORY_SIZE, mPrefMemorySize);
		//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_SOUND_MODULE_ON, (mPrefSoundModuleOn)?1:0);
		//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_REFRESH_HACK_ON, (mPrefRefreshHackOn)?1:0);
		//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CYCLE_HACK_ON, (mPrefCycleHackOn)?1:0);
		nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_SOUND_ON, (mPrefSoundOn)?1:0, null);
		nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_SHOW_INFO_ON, (mPrefShowInfoOn)?1:0, null);
		{
		    SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
	    	String rPath = sp.getString(Globals.PREFKEY_ROM, null);
	    	String f1P = sp.getString(Globals.PREFKEY_F1, null);
	    	String f2P = sp.getString(Globals.PREFKEY_F2, null);
	    	String f3P = sp.getString(Globals.PREFKEY_F3, null);
	    	String f4P = sp.getString(Globals.PREFKEY_F4, null);
	    	boolean ntsc = sp.getBoolean(PreConfig.PREF_KEY_NTSC_ON, getResources().getBoolean(R.bool.pref_default_ntsc_on));
			//int screenHeight = Integer.valueOf(sp.getString(PreConfig.PREF_KEY_SCREEN_HEIGHT, getString(R.string.pref_default_screen_height)));
			boolean trueDrive =sp.getBoolean(PreConfig.PREF_KEY_TRUE_DRIVE_ON, getResources().getBoolean(R.bool.pref_default_true_drive_on)); 
			boolean screenBorder =sp.getBoolean(PreConfig.PREF_KEY_SCREEN_BORDER_ON, getResources().getBoolean(R.bool.pref_default_screen_border_on)); 

			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_SCREEN_BORDER_ON, (screenBorder)?1:-1, null);	    	
			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_TRUE_DRIVE_ON, (trueDrive)?1:-1, null);	    	
			//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_SCREEN_HEIGHT, screenHeight, null);
			{
				if ((rPath == null) || (rPath.length() == 0))
					rPath = "/sdcard/c64/kernal";
				int pos = rPath.lastIndexOf('/');
				if (pos >= 0)
					rPath = rPath.substring(0, pos); // /sdcard/c64
				pos = rPath.lastIndexOf('/');
				if (pos >= 0)
					rPath = rPath.substring(0, pos); // /sdcard			
				rPath = rPath + "/";
				DosBoxMenuUtility.copyDataFile(this, rPath + "c64/");
				nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_ROM, 0, rPath);
			}
			
			//TODO
			//int floppyCount = Integer.valueOf(sp.getString(PreConfig.PREF_KEY_FLOPPY_COUNT, getString(R.string.pref_default_floppy_count)));
			//nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_FLOPPY_COUNT, floppyCount, null);
			int floppyCount = 4;

			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, -1, (floppyCount >=1)?f1P:null);
			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, -2, (floppyCount >=1)?f2P:null);
			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, -3, (floppyCount >=1)?f3P:null);
			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_CHANGE_DISK, -4, (floppyCount >=1)?f4P:null);

			nativeSetOption(DosBoxMenuUtility.DOSBOX_OPTION_ID_NTSC, ntsc?1:0, null);
		}
		
		mDosBoxThread = new DosBoxThread(this);
	}
	
	void shutDownDosBox() {
		boolean retry;
		retry = true;
		while (retry) {
			try {
				mDosBoxThread.join();
				retry =	false;
			}
			catch (InterruptedException e) { // try again shutting down the thread
			}
		}
		
		nativeShutDown();

		if (mAudioDevice != null) {
			mAudioDevice.shutDownAudio();
			mAudioDevice = null;
		}
		
		mDosBoxThread = null;
	}	

	void startDosBox() {
		if (mDosBoxThread != null)
			mDosBoxThread.start();		
		
		if ((mSurfaceView != null) && (mSurfaceView.mVideoThread != null))
			mSurfaceView.mVideoThread.start();
	}
	
	void stopDosBox() {
		nativePause(0);//it won't die if not running
		
		//stop audio AFTER above
		if (mAudioDevice != null)
			mAudioDevice.pause();
		
		mSurfaceView.mVideoThread.setRunning(false);
		
		nativeStop();		
	}

	void resetDosBox() {
		nativeReset();
	}
	
	public void callbackExit() {
		if (mDosBoxThread != null)
			mDosBoxThread.doExit();
	}

	public void callbackVideoRedraw( int w, int h, int s, int e) {
		mSurfaceView.mSrc_width = w;
		mSurfaceView.mSrc_height = h;
		synchronized (mSurfaceView.mDirty) {
			if (mSurfaceView.mDirty) {
				mSurfaceView.mStartLine = Math.min(mSurfaceView.mStartLine, s);
				mSurfaceView.mEndLine = Math.max(mSurfaceView.mEndLine, e);				
			}
			else {
				mSurfaceView.mStartLine = s;
				mSurfaceView.mEndLine = e;
			}
			mSurfaceView.mDirty = true;
		}
	}

	public Bitmap callbackVideoSetMode( int w, int h) {
		mSurfaceView.mSrc_width = w;
		mSurfaceView.mSrc_height = h;
		mSurfaceView.resetScreen(false);
		Bitmap newBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
		if (newBitmap != null) {
			mSurfaceView.mBitmap = null;
			mSurfaceView.mBitmap = newBitmap;
			
			//locnet, 2011-04-28, support 2.1 or below
			mSurfaceView.mVideoBuffer = null;
			mSurfaceView.mVideoBuffer = ByteBuffer.allocateDirect(w * h * 2);
			
			return mSurfaceView.mBitmap;
		}
		return null;
	}
	
	//locnet, 2011-04-28, support 2.1 or below
	public Buffer callbackVideoGetBuffer() {
		if (mSurfaceView != null)  {
			//if (mSurfaceView.mVideoBuffer != null)
			//	mSurfaceView.mVideoBuffer.position(0);
			return mSurfaceView.mVideoBuffer;
		}
		else
			return null;
	}
	
	public int callbackAudioInit(int rate, int channels, int encoding, int bufSize) {
		if (mAudioDevice != null)
			return mAudioDevice.initAudio(rate, channels, encoding, bufSize);
		else
			return 0;
	}
	
	public void callbackAudioWriteBuffer(int size) {
		if (mAudioDevice != null)
			mAudioDevice.AudioWriteBuffer(size);		
	}

	public short[] callbackAudioGetBuffer() {
		if (mAudioDevice != null)
			return mAudioDevice.mAudioBuffer;
		else
			return null;
	}
	
	class DosBoxThread extends Thread {
		DosBoxLauncher mParent;
		public boolean	mDosBoxRunning = false;

		DosBoxThread(DosBoxLauncher parent) {
			mParent =  parent;
		}
		
		public void run() {
			mDosBoxRunning = true;
			nativeStart(mSurfaceView.mBitmap, mSurfaceView.mBitmap.getWidth(), mSurfaceView.mBitmap.getHeight());
			//will never return to here;
		}
		
		public void doExit() {			
			if (mSurfaceView != null) {
				InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
				if (imm != null) {
					imm.hideSoftInputFromWindow(mSurfaceView.getWindowToken(), 0);
				}
			}
			mDosBoxRunning = false;
			//mParent.finish();
		}		
	}
}

