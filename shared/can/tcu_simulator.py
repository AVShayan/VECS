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
last_bcm_rx = time.monotonic()
expected_bcm_counter = None
timeout_reported = False

print("TCU simulator started.")

next_tx = time.monotonic()

try:
    while True:

        now = time.monotonic()

        # -------------------------------
        # TCU_Request transmission
        # -------------------------------

        if now >= next_tx:

            data = tcu_request.encode({
                "RemoteHeadlightRequest": 1,
                "RemoteHazardRequest": 0,
                "TCU_RequestCounter": counter
            })

            msg = can.Message(
                arbitration_id=tcu_request.frame_id,
                data=data,
                is_extended_id=False
            )

            bus.send(msg)

            print(
                f"TCU TX  0x200  "
                f"{data.hex(' ').upper()}  "
                f"Counter={counter}"
            )

            counter = (counter + 1) % 16
            next_tx += PERIOD

        # -------------------------------
        # Receive BCM_Status
        # -------------------------------

        msg = bus.recv(timeout=0)

        if msg is not None and msg.arbitration_id == 0x100:

            decoded = bcm_status.decode(msg.data)

            received_counter = decoded["BCM_StatusCounter"]

            last_bcm_rx = now
            timeout_reported = False

            if expected_bcm_counter is not None:

                expected = (expected_bcm_counter + 1) % 8

                if received_counter != expected:
                    print(
                        f"TCU WARNING: BCM counter error "
                        f"(expected {expected}, "
                        f"received {received_counter})"
                    )

            expected_bcm_counter = received_counter

            print(
                f"TCU RX   0x100  "
                f"Counter={received_counter}  "
                f"Left={decoded['LeftIndicator']}  "
                f"Right={decoded['RightIndicator']}  "
                f"Hazard={decoded['HazardActive']}  "
                f"Headlights={decoded['HeadlightsActive']}"
            )

        # -------------------------------
        # BCM timeout
        # -------------------------------

        if now - last_bcm_rx > TIMEOUT and not timeout_reported:

            print(
                "!!! TCU ERROR: BCM_Status timeout "
                f"({TIMEOUT * 1000:.0f} ms)"
            )

            timeout_reported = True

        time.sleep(0.001)

except KeyboardInterrupt:
    print("\nTCU simulator stopped.")

finally:
    bus.shutdown()