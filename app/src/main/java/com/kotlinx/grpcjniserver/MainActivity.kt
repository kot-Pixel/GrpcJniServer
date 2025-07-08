package com.kotlinx.grpcjniserver

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.kotlinx.grpcjniserver.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        runUdpCarplayServer()
    }

    external fun runUdpCarplayServer()

    companion object {
        // Used to load the 'grpcjniserver' library on application startup.
        init {
            System.loadLibrary("grpcjniserver")
        }
    }
}