import cantools
import can
import time

# Load DBC
db = cantools.database.load_file("VECS_CAN_V1.dbc")

# Create TWO nodes on the same virtual CAN channel
bcm_bus = can.Bus(
    interface="virtual",
    channel="counter_fault_test",
    receive_own_messages=False
)

tcu_bus = can.Bus(
    interface="virtual",
    channel="counter_fault_test",
    receive_own_messages=False
)

BCM_STATUS_ID = 0x100

# Deliberately incorrect counter sequence
counter_sequence = [0, 1, 2, 7, 4, 5]

print("CAN counter fault-injection test")
print("---------------------------------")
print("Normal sequence:   0 → 1 → 2 → 3 → 4 → 5...")
print("Injected sequence: 0 → 1 → 2 → 7 → 4 → 5")
print()

last_counter = None

for counter in counter_sequence:

    # BCM signal values
    signals = {
        "LeftIndicator": 1,
        "RightIndicator": 0,
        "HazardActive": 0,
        "HeadlightsActive": 1,
        "BCMFault": 0,
        "BCM_StatusCounter": counter
    }

    # Encode according to DBC
    data = db.encode_message("BCM_Status", signals)

    # BCM sends
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

    # TCU receives
    rx_msg = tcu_bus.recv(timeout=0.5)

    if rx_msg is None:
        print("!!! TCU ERROR: BCM_Status timeout")
        continue

    # Decode received frame
    decoded = db.decode_message(
        rx_msg.arbitration_id,
        rx_msg.data
    )

    received_counter = decoded["BCM_StatusCounter"]

    print(
        f"TCU RX  0x100  {rx_msg.data.hex().upper():>2}  "
        f"Counter={received_counter}"
    )

    # Counter supervision
    if last_counter is not None:

        expected_counter = (last_counter + 1) % 8

        if received_counter != expected_counter:

            print()
            print("!!! TCU ERROR: BCM_Status counter error")
            print(f"    Expected = {expected_counter}")
            print(f"    Received = {received_counter}")
            print()

    last_counter = received_counter

    time.sleep(0.2)

print()
print("Counter fault-injection test finished.")

bcm_bus.shutdown()
tcu_bus.shutdown()