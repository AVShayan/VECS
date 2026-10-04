import cantools
import can
import time

# Load DBC
db = cantools.database.load_file("VECS_CAN_V1.dbc")

# Virtual CAN buses
bcm_bus = can.Bus(
    interface="virtual",
    channel="ecu_restart_test",
    receive_own_messages=False
)

tcu_bus = can.Bus(
    interface="virtual",
    channel="ecu_restart_test",
    receive_own_messages=False
)

BCM_STATUS_ID = 0x100

print("CAN ECU restart / counter reset test")
print("------------------------------------")
print("BCM normal operation: 0 → 1 → 2 → 3")
print("BCM RESTART")
print("BCM after restart:    0 → 1 → 2 → 3")
print()

last_counter = None
ecu_restarted = False

# First BCM operating cycle
for counter in [0, 1, 2, 3]:

    signals = {
        "LeftIndicator": 1,
        "RightIndicator": 0,
        "HazardActive": 0,
        "HeadlightsActive": 1,
        "BCMFault": 0,
        "BCM_StatusCounter": counter
    }

    data = db.encode_message("BCM_Status", signals)

    msg = can.Message(
        arbitration_id=BCM_STATUS_ID,
        data=data,
        is_extended_id=False
    )

    bcm_bus.send(msg)

    print(
        f"BCM TX  0x100  {data.hex().upper():>2}  "
        f"Counter={counter}"
    )

    rx_msg = tcu_bus.recv(timeout=0.5)

    if rx_msg is None:
        print("!!! TCU ERROR: BCM_Status timeout")
        continue

    decoded = db.decode_message(
        rx_msg.arbitration_id,
        rx_msg.data
    )

    received_counter = decoded["BCM_StatusCounter"]

    print(
        f"TCU RX  0x100  {rx_msg.data.hex().upper():>2}  "
        f"Counter={received_counter}"
    )

    if last_counter is not None:

        expected_counter = (last_counter + 1) % 8

        if received_counter != expected_counter:
            print(
                f"!!! TCU ERROR: Counter mismatch "
                f"(Expected {expected_counter}, "
                f"Received {received_counter})"
            )

    last_counter = received_counter

    time.sleep(0.2)


# -------------------------------------------------
# Simulate BCM restart
# -------------------------------------------------

print()
print("====================================")
print("        BCM RESTART SIMULATED")
print("====================================")
print()

# In real firmware, the ECU would reset its counter
ecu_restarted = True
last_counter = None

time.sleep(0.5)


# BCM starts transmitting again from counter 0
for counter in [0, 1, 2, 3]:

    signals = {
        "LeftIndicator": 1,
        "RightIndicator": 0,
        "HazardActive": 0,
        "HeadlightsActive": 1,
        "BCMFault": 0,
        "BCM_StatusCounter": counter
    }

    data = db.encode_message("BCM_Status", signals)

    msg = can.Message(
        arbitration_id=BCM_STATUS_ID,
        data=data,
        is_extended_id=False
    )

    bcm_bus.send(msg)

    print(
        f"BCM TX  0x100  {data.hex().upper():>2}  "
        f"Counter={counter}"
    )

    rx_msg = tcu_bus.recv(timeout=0.5)

    if rx_msg is None:
        print("!!! TCU ERROR: BCM_Status timeout")
        continue

    decoded = db.decode_message(
        rx_msg.arbitration_id,
        rx_msg.data
    )

    received_counter = decoded["BCM_StatusCounter"]

    print(
        f"TCU RX  0x100  {rx_msg.data.hex().upper():>2}  "
        f"Counter={received_counter}"
    )

    if last_counter is not None:

        expected_counter = (last_counter + 1) % 8

        if received_counter != expected_counter:
            print(
                f"!!! TCU ERROR: Counter mismatch "
                f"(Expected {expected_counter}, "
                f"Received {received_counter})"
            )

    last_counter = received_counter

    time.sleep(0.2)


print()
print("ECU restart test finished.")

bcm_bus.shutdown()
tcu_bus.shutdown()