# import cantools

# db = cantools.database.load_file("VECS_CAN_V1.dbc")

# message = db.get_message_by_name("BCM_Status")

# for counter in range(8):
#     data = message.encode({
#         "LeftIndicator": 1,
#         "RightIndicator": 0,
#         "HazardActive": 0,
#         "HeadlightsActive": 1,
#         "BCMFault": 0,
#         "BCM_StatusCounter": counter
#     })

#     print(
#         f"Counter = {counter}  "
#         f"DATA = {data.hex(' ').upper()}"
#     )
# import cantools

# db = cantools.database.load_file("VECS_CAN_V1.dbc")

# message = db.get_message_by_name("TCU_Request")

# for counter in range(16):
#     data = message.encode({
#         "RemoteHeadlightRequest": 1,
#         "RemoteHazardRequest": 0,
#         "TCU_RequestCounter": counter
#     })

#     print(
#         f"Counter = {counter:2}  "
#         f"DATA = {data.hex(' ').upper()}"
#     )
import can
import cantools
import time

PERIOD = 0.100
TIMEOUT = 0.300

db = cantools.database.load_file("VECS_CAN_V1.dbc")

bcm_status = db.get_message_by_name("BCM_Status")

# Two endpoints on the same virtual CAN channel
bcm_bus = can.Bus(
    interface="virtual",
    channel="VECS_CAN"
)

tcu_bus = can.Bus(
    interface="virtual",
    channel="VECS_CAN"
)

counter = 0

last_bcm_rx = None
timeout_reported = False

start = time.monotonic()
next_tx = start

print("BCM → TCU communication supervision test")
print()
print("0–2 sec : BCM transmitting")
print("2–3 sec : BCM stopped")
print("3–7 sec : BCM transmitting")
print("7–8 sec : BCM stopped")
print()

try:

    while time.monotonic() - start < 8.0:

        now = time.monotonic()
        elapsed = now - start

        # ==========================================
        # BCM TRANSMISSION
        # ==========================================

        if now >= next_tx:

            bcm_transmitting = (
                elapsed < 2.0
                or 3.0 <= elapsed < 7.0
            )

            if bcm_transmitting:

                data = bcm_status.encode({
                    "LeftIndicator": 1,
                    "RightIndicator": 0,
                    "HazardActive": 0,
                    "HeadlightsActive": 1,
                    "BCMFault": 0,
                    "BCM_StatusCounter": counter
                })

                msg = can.Message(
                    arbitration_id=0x100,
                    data=data,
                    is_extended_id=False
                )

                bcm_bus.send(msg)

                print(
                    f"BCM TX  0x100  "
                    f"{data.hex(' ').upper()}  "
                    f"Counter={counter}"
                )

                counter = (counter + 1) % 8

            next_tx += PERIOD

        # ==========================================
        # TCU RECEIVES BCM STATUS
        # ==========================================

        msg = tcu_bus.recv(timeout=0)

        if msg is not None and msg.arbitration_id == 0x100:

            decoded = bcm_status.decode(msg.data)

            received_counter = decoded["BCM_StatusCounter"]

            last_bcm_rx = now

            if timeout_reported:

                print()
                print("TCU: BCM communication restored.")
                print()

                timeout_reported = False

            print(
                f"TCU RX   0x100  "
                f"Counter={received_counter}"
            )

        # ==========================================
        # TIMEOUT DETECTION
        # ==========================================

        if (
            last_bcm_rx is not None
            and now - last_bcm_rx > TIMEOUT
            and not timeout_reported
        ):

            print()
            print(
                "!!! TCU ERROR: "
                "BCM_Status timeout "
                f"({TIMEOUT * 1000:.0f} ms)"
            )
            print()

            timeout_reported = True

        time.sleep(0.001)

finally:

    bcm_bus.shutdown()
    tcu_bus.shutdown()

print()
print("Test finished.")