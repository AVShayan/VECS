import can
import cantools

# Load our DBC
db = cantools.database.load_file("VECS_CAN_V1.dbc")

# Get our two messages
bcm_status = db.get_message_by_name("BCM_Status")
tcu_request = db.get_message_by_name("TCU_Request")

# Create one virtual CAN bus
bcm_bus = can.Bus(interface="virtual", channel="VECS_CAN")
tcu_bus = can.Bus(interface="virtual", channel="VECS_CAN")

print("Virtual CAN bus started.")
print()

# --------------------------------------------------
# BCM sends its current body-electronics state
# --------------------------------------------------

bcm_data = bcm_status.encode({
    "LeftIndicator": 1,
    "RightIndicator": 0,
    "HazardActive": 0,
    "HeadlightsActive": 1,
    "BCMFault": 0,
    "BCM_StatusCounter":0
})

bcm_message = can.Message(
    arbitration_id=bcm_status.frame_id,
    data=bcm_data,
    is_extended_id=False
)

print("BCM TX:")
print(f"  ID   = 0x{bcm_message.arbitration_id:X}")
print(f"  DATA = {bcm_message.data.hex(' ').upper()}")

bcm_bus.send(bcm_message)

# --------------------------------------------------
# TCU receives BCM_Status
# --------------------------------------------------

received = tcu_bus.recv(timeout=1.0)

if received:
    decoded = bcm_status.decode(received.data)

    print("\nTCU RX:")
    print(f"  ID   = 0x{received.arbitration_id:X}")
    print(f"  DATA = {received.data.hex(' ').upper()}")

    for signal, value in decoded.items():
        print(f"  {signal:20} = {value}")
else:
    print("\nTCU did not receive the frame.")

# --------------------------------------------------
# TCU sends a request to BCM
# --------------------------------------------------

tcu_data = tcu_request.encode({
    "RemoteHeadlightRequest": 1,
    "RemoteHazardRequest": 0,
    "TCU_RequestCounter": 0
})

tcu_message = can.Message(
    arbitration_id=tcu_request.frame_id,
    data=tcu_data,
    is_extended_id=False
)

print("\nTCU TX:")
print(f"  ID   = 0x{tcu_message.arbitration_id:X}")
print(f"  DATA = {tcu_message.data.hex(' ').upper()}")

tcu_bus.send(tcu_message)

# --------------------------------------------------
# BCM receives TCU_Request
# --------------------------------------------------

received = bcm_bus.recv(timeout=1.0)

if received:
    decoded = tcu_request.decode(received.data)

    print("\nBCM RX:")
    print(f"  ID   = 0x{received.arbitration_id:X}")
    print(f"  DATA = {received.data.hex(' ').upper()}")

    for signal, value in decoded.items():
        print(f"  {signal:25} = {value}")
else:
    print("\nBCM did not receive the frame.")

# Shut down virtual buses
bcm_bus.shutdown()
tcu_bus.shutdown()