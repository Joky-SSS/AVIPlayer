package com.jokyxray.aviplayer

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import androidx.core.content.ContextCompat
import com.jokyxray.aviplayer.databinding.ActivityMainBinding
import java.io.File

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.playButton.setOnClickListener { onPlayButtonClick() }
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.M){
            requestPermissions(arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE),1)
        }
    }

    private fun onPlayButtonClick() {
        var intent:Intent? = null
        when(binding.playerRadioGroup.checkedRadioButtonId){
            R.id.bitmap_player_radio -> intent = Intent(this,BitmapPlayerActivity::class.java)
            R.id.native_window_player -> intent = Intent(this,NativeWindowPlayerActivity::class.java)
        }
        intent?.let {
            val file = File(Environment.getExternalStorageDirectory(),binding.fileNameEdit.text.toString())
            intent.putExtra(AbstractPlayerActivity.EXTRA_FILE_NAME,file.absolutePath)
            startActivity(intent)
        }
    }


}