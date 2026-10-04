import cantools

# Load our actual DBC
db = cantools.database.load_file("VECS_CAN_V1.dbc")

# Get TCU_Request
message = db.get_message_by_name("TCU_Request")

# Encode a TCU request:
data = message.encode({
    "RemoteHeadlightRequest": 0,
    "RemoteHazardRequest": 1,
    "TCU_RequestCounter": 0
})

print("CAN ID :", hex(message.frame_id))
print("DLC    :", message.length)
print("DATA   :", data.hex(" ").upper())