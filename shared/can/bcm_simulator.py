import can
import cantools
import time

PERIOD = 0.100
TIMEOUT = 0.300

db = cantools.database.load_file("VECS_CAN_V1.dbc")
bcm_status = db.get_message_by_name("BCM_Status")
tcu_request = db.get_message_by_name("TCU_Request")

bus = can.Bus(
    interface="virtual",
    channel="VECS_CAN"
)

counter = 0
last_tcu_rx = time.monotonic()
expected_tcu_counter = None
timeout_reported = False

print("BCM simulator started.")

next_tx = time.monotonic()

try:
    while True:

        now = time.monotonic()

        # -------------------------------
        # BCM_Status transmission
        # -------------------------------

        if now >= next_tx:

            data = bcm_status.encode({
                "LeftIndicator": 1,
                "RightIndicator": 0,
                "HazardActive": 0,
                "HeadlightsActive": 1,
                "BCMFault": 0,
                "BCM_StatusCounter": counter
            })

            msg = can.Message(
                arbitration_id=bcm_status.frame_id,
                data=data,
                is_extended_id=False
            )

            bus.send(msg)

            print(
                f"BCM TX  0x100  "
                f"{data.hex(' ').upper()}  "
                f"Counter={counter}"
            )

            counter = (counter + 1) % 8
            next_tx += PERIOD

        # -------------------------------
        # Receive TCU_Request
        # -------------------------------

        msg = bus.recv(timeout=0)

        if msg is not None and msg.arbitration_id == 0x200:

            decoded = tcu_request.decode(msg.data)

            received_counter = decoded["TCU_RequestCounter"]

            last_tcu_rx = now
            timeout_reported = False

            if expected_tcu_counter is not None:

                expected = (expected_tcu_counter + 1) % 16

                if received_counter != expected:
                    print(
                        f"BCM WARNING: TCU counter error "
                        f"(expected {expected}, "
                        f"received {received_counter})"
                    )

            expected_tcu_counter = received_counter

            print(
                f"BCM RX   0x200  "
                f"Counter={received_counter}  "
                f"Headlight={decoded['RemoteHeadlightRequest']}  "
                f"Hazard={decoded['RemoteHazardRequest']}"
            )

        # -------------------------------
        # TCU timeout
        # -------------------------------

        if now - last_tcu_rx > TIMEOUT and not timeout_reported:

            print(
                "!!! BCM ERROR: TCU_Request timeout "
                f"({TIMEOUT * 1000:.0f} ms)"
            )

            timeout_reported = True

        time.sleep(0.001)

except KeyboardInterrupt:
    print("\nBCM simulator stopped.")

finally:
    bus.shutdown()