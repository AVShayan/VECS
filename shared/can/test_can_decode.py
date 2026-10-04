import cantools

# Load our DBC
db = cantools.database.load_file("VECS_CAN_V1.dbc")

# Get TCU_Request
message = db.get_message_by_name("BCM_Status")

# Simulate receiving this raw CAN frame
data = bytes.fromhex("09")

# Decode it
decoded = message.decode(data)

print("Received CAN frame:")
print("CAN ID :", hex(message.frame_id))
print("DLC    :", len(data))
print("DATA   :", data.hex(" ").upper())

print("\nDecoded signals:")

for signal, value in decoded.items():
    print(f"{signal:20} = {value}")