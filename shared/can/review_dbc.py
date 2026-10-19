import cantools

db = cantools.database.load_file("VECS_CAN_V1.dbc")

for message in db.messages:
    print(f"\nMessage: {message.name}")
    print(f"  CAN ID: 0x{message.frame_id:X}")
    print(f"  DLC:    {message.length} byte(s)")
    print(f"  Sender: {message.senders}")

    for signal in message.signals:
        print(
            f"  {signal.name:28} "
            f"start={signal.start:2} "
            f"length={signal.length:2} "
            f"byte_order={signal.byte_order:12} "
            f"signed={signal.is_signed} "
            f"receivers={signal.receivers}"
        )
