package com.example.VECS

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowForward
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Info
import androidx.compose.material.icons.filled.Notifications
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material.icons.filled.LocationOn
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.scale
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import org.eclipse.paho.client.mqttv3.*
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence
import org.json.JSONObject
import java.util.Properties
import javax.net.ssl.SSLSocketFactory
import androidx.compose.foundation.border

class MainActivity : ComponentActivity() {

    private var mqttClient: MqttClient? = null
    private val TAG = "VECS_MQTT"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val rangeState = mutableStateOf("0")
        val batteryState = mutableStateOf(0.0f)
        val syncStatusState = mutableStateOf("Connecting...")

        startMqtt(rangeState, batteryState, syncStatusState)

        setContent {
            MaterialTheme {
                MainContainer(rangeState, batteryState, syncStatusState)
            }
        }
    }

    private fun startMqtt(
        range: MutableState<String>,
        battery: MutableState<Float>,
        status: MutableState<String>
    ) {
        val brokerUri = "ssl://b21d6f16.ala.us-east-1.emqxsl.com:8883" 
        val clientId = "VECS_Android_" + System.currentTimeMillis()
        val telemetryTopic = "vecs/vehicle/telemetry"

        // Run connection routine entirely off the main thread to prevent network freezes
        Thread {
            try {
                mqttClient = MqttClient(brokerUri, clientId, MemoryPersistence())
                
                val options = MqttConnectOptions().apply {
                    isAutomaticReconnect = true
                    isCleanSession = true
                    connectionTimeout = 30
                    keepAliveInterval = 60
                    userName = "Shayan"
                    password = "Hello@786".toCharArray()
                    
                    // Uses default secure factory; lets Android trust EMQX Cloud certificates natively
                    socketFactory = SSLSocketFactory.getDefault()
                    
                    // Passing SSL properties directly to bypass proxy/gateway timeouts
                    val sslProps = Properties()
                    sslProps.setProperty("com.ibm.ssl.protocol", "TLSv1.2")
                    sslProperties = sslProps
                }

                mqttClient?.setCallback(object : MqttCallback {
                    override fun connectionLost(cause: Throwable?) {
                        Log.e(TAG, "Connection lost: ${cause?.message}")
                        status.value = "Disconnected"
                    }

                    override fun messageArrived(topic: String?, message: MqttMessage?) {
                        val payload = message?.toString() ?: return
                        Log.d(TAG, "Inbound payload: $payload")
                        try {
                            val json = JSONObject(payload)
                            if (json.has("range")) {
                                range.value = json.getInt("range").toString()
                            }
                            if (json.has("battery")) {
                                val batVal = json.getInt("battery")
                                battery.value = batVal / 100f
                            }
                            status.value = "Synced"
                        } catch (e: Exception) {
                            Log.e(TAG, "JSON error: ${e.message}")
                        }
                    }

                    override fun deliveryComplete(token: IMqttDeliveryToken?) {}
                })

                Log.d(TAG, "Attempting connection to broker...")
                mqttClient?.connect(options)
                mqttClient?.subscribe(telemetryTopic, 0)
                Log.d(TAG, "Successfully connected and subscribed.")
                status.value = "Connected to Bike!"

            } catch (e: MqttException) {
                Log.e(TAG, "Connection failed: ReasonCode ${e.reasonCode}, Msg: ${e.message}")
                status.value = "Connection failed"
                e.printStackTrace()
            }
        }.start()
    }

    fun publishCommand(action: String) {
        val commandTopic = "vecs/vehicle/telemetry"
        
        Thread {
            try {
                val client = mqttClient
                if (client != null && client.isConnected) {
                    val payload = JSONObject().apply {
                        put("command", action)
                    }.toString()
                    
                    val message = MqttMessage(payload.toByteArray()).apply {
                        qos = 0
                    }
                    
                    Log.d(TAG, "Publishing command: $payload")
                    client.publish(commandTopic, message)
                    Log.d(TAG, "Publish successful.")
                } else {
                    Log.e(TAG, "Cannot publish: Client is disconnected.")
                }
            } catch (e: Exception) {
                Log.e(TAG, "Publish error: ${e.message}")
                e.printStackTrace()
            }
        }.start()
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            mqttClient?.disconnect()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}

@Composable
fun MainContainer(
    rangeState: State<String>,
    batteryState: State<Float>,
    syncStatusState: State<String>
) {
    var selectedTab by remember { mutableStateOf(0) }
    
    val navItems = listOf(
        NavigationItem("Home", Icons.Default.Home),
        NavigationItem("Settings", Icons.Default.Settings),
        NavigationItem("Support", Icons.Default.Info)
    )

    Scaffold(
        bottomBar = {
            NavigationBar(
                containerColor = Color.White,
                tonalElevation = 8.dp
            ) {
                navItems.forEachIndexed { index, item ->
                    NavigationBarItem(
                        selected = selectedTab == index,
                        onClick = { selectedTab = index },
                        label = { Text(item.title, fontSize = 11.sp) },
                        icon = { Icon(item.icon, contentDescription = item.title) },
                        colors = NavigationBarItemDefaults.colors(
                            selectedIconColor = Color.Black,
                            selectedTextColor = Color.Black,
                            indicatorColor = Color(0xFFE2F7E9),
                            unselectedIconColor = Color.Gray,
                            unselectedTextColor = Color.Gray
                        )
                    )
                }
            }
        }
    ) { innerPadding ->
        Surface(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding),
            color = Color(0xFFF3F5F7)
        ) {
            DashboardScreen(rangeState, batteryState, syncStatusState)
        }
    }
}

data class NavigationItem(val title: String, val icon: ImageVector)

@Composable
fun DashboardScreen(
    rangeState: State<String>,
    batteryState: State<Float>,
    syncStatusState: State<String>
) {
    val context = LocalContext.current

    Column(
        modifier = Modifier
            .fillMaxSize()
            .verticalScroll(rememberScrollState())
            .padding(horizontal = 24.dp, vertical = 16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Spacer(modifier = Modifier.height(20.dp))

        Box(
            modifier = Modifier
                .background(Color(0xFFE2F7E9), shape = RoundedCornerShape(12.dp))
                .padding(horizontal = 16.dp, vertical = 6.dp)
        ) {
            Text(
                text = "E   Ride   S",
                color = Color(0xFF1E5E3A),
                fontWeight = FontWeight.Bold,
                fontSize = 14.sp
            )
        }

        Spacer(modifier = Modifier.height(16.dp))

        Row(
            verticalAlignment = Alignment.Bottom,
            horizontalArrangement = Arrangement.Center,
            modifier = Modifier.fillMaxWidth()
        ) {
            Text(
                text = rangeState.value,
                fontSize = 76.sp,
                fontWeight = FontWeight.Bold,
                color = Color.Black,
                lineHeight = 76.sp
            )
            Text(
                text = "km",
                fontSize = 24.sp,
                fontWeight = FontWeight.Bold,
                color = Color.Black,
                modifier = Modifier.padding(bottom = 14.dp, start = 4.dp)
            )
        }

        Spacer(modifier = Modifier.height(10.dp))

        Box(
            modifier = Modifier
                .fillMaxWidth()
                .height(6.dp)
                .background(Color(0xFFDCDCDC), shape = CircleShape)
        ) {
            Box(
                modifier = Modifier
                    .fillMaxWidth(batteryState.value.coerceIn(0.0f, 1.0f))
                    .fillMaxHeight()
                    .background(Color.Black, shape = CircleShape)
            )
        }

        Spacer(modifier = Modifier.height(8.dp))

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            val displayPercentage = (batteryState.value * 100).toInt()
            Text(text = "$displayPercentage%", fontSize = 13.sp, fontWeight = FontWeight.Bold, color = Color.Black)
            Text(text = syncStatusState.value, fontSize = 13.sp, color = Color.Gray)
        }

        Spacer(modifier = Modifier.height(24.dp))

        Card(
            modifier = Modifier.fillMaxWidth(),
            shape = RoundedCornerShape(24.dp),
            colors = CardDefaults.cardColors(containerColor = Color(0xFFC9ECD1))
        ) {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(20.dp),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Column(modifier = Modifier.weight(1f)) {
                    Text(text = "Parked", fontSize = 12.sp, color = Color(0xFF2E5B3D))
                    Spacer(modifier = Modifier.height(4.dp))
                    Text(
                        text = "Your Bike is ready",
                        fontSize = 18.sp,
                        fontWeight = FontWeight.SemiBold,
                        color = Color.Black
                    )
                    Text(
                        text = "Lets go!",
                        fontSize = 18.sp,
                        fontWeight = FontWeight.Bold,
                        color = Color.Black
                    )
                }

                IconButton(
                    onClick = { 
                        (context as? MainActivity)?.publishCommand("START_BIKE") 
                    },
                    modifier = Modifier
                        .size(48.dp)
                        .background(Color.Black, shape = RoundedCornerShape(14.dp))
                ) {
                    Icon(
                        imageVector = Icons.Default.ArrowForward,
                        contentDescription = "Go",
                        tint = Color.White
                    )
                }
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            Column(
                modifier = Modifier.weight(0.45f),
                verticalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                Card(
                    modifier = Modifier
                        .fillMaxWidth()
                        .height(90.dp),
                    shape = RoundedCornerShape(20.dp),
                    colors = CardDefaults.cardColors(containerColor = Color(0xFFF9EAE1))
                ) {
                    Box(modifier = Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
                        IconButton(
                            onClick = { 
                                (context as? MainActivity)?.publishCommand("PING") 
                            },
                        ) {
                            Icon(
                                imageVector = Icons.Default.Notifications,
                                contentDescription = "Notification Status",
                                tint = Color(0xFF171515),
                                modifier = Modifier.size(32.dp)
                            )
                        }
                    }
                }

                Card(
                    modifier = Modifier
                        .fillMaxWidth()
                        .height(90.dp),
                    shape = RoundedCornerShape(20.dp),
                    colors = CardDefaults.cardColors(containerColor = Color(0xFFF9EAE1))
                ) {
                    Box(modifier = Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
                        IconButton(
                            onClick = { 
                                (context as? MainActivity)?.publishCommand("POWER") 
                            },
                        ) {
                            Icon(
                                imageVector = Icons.Default.LocationOn,
                                contentDescription = "Ignition",
                                tint = Color(0xFF171515),
                                modifier = Modifier.size(32.dp)
                            )
                        }
                    }
                }
            }

            Card(
                modifier = Modifier
                    .weight(0.55f)
                    .height(196.dp),
                shape = RoundedCornerShape(24.dp),
                colors = CardDefaults.cardColors(containerColor = Color(0xFFF9EAE1))
            ) {
                Box(modifier = Modifier.fillMaxSize()) {
                    Image(
                        painter = painterResource(id = R.drawable.hero),
                        contentDescription = "Hero Optima",
                        modifier = Modifier
                            .fillMaxSize()
                            .offset(x = (-7).dp, y = 63.dp)
                            .scale(1.8f)
                            .padding(12.dp),
                        contentScale = ContentScale.Fit
                    )
                }
            }
        }

        // Added fake data metrics table in the empty space below the row layout
        Spacer(modifier = Modifier.height(20.dp))

        Card(
            modifier = Modifier.fillMaxWidth(),
            shape = RoundedCornerShape(20.dp),
            colors = CardDefaults.cardColors(containerColor = Color.White)
        ) {
            Column(modifier = Modifier.padding(16.dp)) {
                Text(
                    text = "Bike Data Metrics", 
                    fontSize = 14.sp, 
                    fontWeight = FontWeight.Bold, 
                    color = Color.Black
                )
                Spacer(modifier = Modifier.height(12.dp))
                
                Row(modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween) {
                    Text(text = "Daily Odometer", fontSize = 12.sp, color = Color.Gray)
                    Text(text = "18 km", fontSize = 12.sp, fontWeight = FontWeight.Bold, color = Color.Black)
                }
                Divider(modifier = Modifier.padding(vertical = 8.dp), color = Color(0xFFF0F0F0))
                
                Row(modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween) {
                    Text(text = "Motor Temp", fontSize = 12.sp, color = Color.Gray)
                    Text(text = "85°C", fontSize = 12.sp, fontWeight = FontWeight.Bold, color = Color(0xFFBD2F2F))
                }
                Divider(modifier = Modifier.padding(vertical = 8.dp), color = Color(0xFFF0F0F0))
                
                Row(modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween) {
                    Text(text = "ECU Temp", fontSize = 12.sp, color = Color.Gray)
                    Text(text = "42°C", fontSize = 12.sp, fontWeight = FontWeight.Bold, color = Color(0xFFB05A21))
                }
            }
        }
    }
}